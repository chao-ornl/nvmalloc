#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "nvm.h"

int get_file_index(void);

void check_nvm_device(char *str)
{
	struct stat st;
	if (stat(str, &st) != 0 )
		perror ("NVM sandbox (for memory mapped files) does not exist\n");
}

void nvm_init(){
	read_ssd_info();

	create_name_space();
}

void nvm_exit(){
	release_ssd_info();
	destory_name_space();
}


///////////////////////////////////////////////////////////////////
// nvm_malloc
///////////////////////////////////////////////////////////////////

#define size_align 4096

#define	roundsize(x)	((size_t)( (size_align - 1 + (x)) & (~((size_t)(size_align - 1))) ))

#undef malloc
void *nvm_malloc(size_t size)
{
	if ( size == 0 ) return NULL;

	size_t variable_size = roundsize(size);

	(void) check_nvm_device(ssd_head->mountdir); // by default, use the first SSD
	unsigned int tmp = get_file_index();

	char f_name[80];
	sprintf(f_name, "%s%s%d", ssd_head->mountdir, "/file_", tmp); // by default, use the first SSD

	// check if a file exists 
	if ( access ( f_name, F_OK ) != -1 ) { perror ("File exist !!!"); printf("%s\n", f_name); exit(0); }

	int fd = open (f_name, O_RDWR | O_CREAT, 0666);
	if (fd == -1){
		perror("File open failed\n");
		exit(0);
	}
	
	posix_fallocate(fd, 0, variable_size);

	ftruncate(fd, variable_size);

	close(fd);

	fd = open(f_name, O_RDWR);

	void *addr = mmap (NULL, variable_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

	if ( addr == MAP_FAILED ) { perror("Failed mmap"); exit(0); }

	close(fd);

	// register variable in name space 
	register_name_space( (long int) addr, fd, f_name, variable_size);

	return (void *) (addr);
}
#define malloc nvm_malloc

///////////////////////////////////////////////////////////////////
// nvm_calloc
///////////////////////////////////////////////////////////////////

//#undef calloc
void *nvm_calloc(size_t nmemb, size_t size)
{
	void *addr = nvm_malloc(nmemb * size);
	memset(addr, 0, nmemb * size);

	if (addr == NULL) {
		printf("(nvm_calloc()), addr = NULL\n");
		exit(0);
	}

	return addr;
}
//#define calloc nvm_calloc

///////////////////////////////////////////////////////////////////
// nvm_realloc 
///////////////////////////////////////////////////////////////////

#undef malloc 
void *nvm_realloc(void *var, size_t size)
{
	if ( var == NULL ) {

		void *addr = nvm_malloc(size);

		if (addr == NULL) {
			printf("(nvm_malloc()), addr = NULL\n");
			exit(0);
		}

		return addr;
	}
	else if ((var != NULL) && (size == 0)) {
		nvm_free(var);
		return NULL;
	}


	size_t variable_size = roundsize(size);

	// check nvm sandbox	
	(void) check_nvm_device(ssd_head->mountdir); // by default, use the first SSD


	char *file_name = (char *) malloc ( sizeof(char) * 80 );

	(void) return_file_name_to_addr ( (long int) var, file_name);

	int fd = open (file_name, O_RDWR, 0666);
	off_t file_size = lseek (fd, 0, SEEK_END);

	munmap ( (void *) var, file_size );

	if ( variable_size > file_size) {

		posix_fallocate(fd, file_size, (variable_size - file_size));

		file_size = lseek (fd, 0, SEEK_END);

	}
	else {
		ftruncate(fd, (variable_size));

		file_size = variable_size;
	}

	void* var_addr = mmap(NULL, file_size, PROT_WRITE | PROT_READ | PROT_EXEC, MAP_SHARED, fd, 0);

	if (var_addr == MAP_FAILED) {
		perror ("mmap failed\n"); exit(0);
	}

	close(fd);

	update_name_space( file_name, file_size, (long int) var_addr );

	return var_addr;
}
#define malloc nvm_malloc

///////////////////////////////////////////////////////////////////
// nvm_free
///////////////////////////////////////////////////////////////////

#undef free
void nvm_free(void *ptr)
{
	if ( ptr == NULL ) return;

	var_info *tmp;

	int variable_size = return_variable_size_to_addr ( (long int) ptr);

	if (variable_size == -1) {
		perror("nam_free tries to free a memory not allocated before\n");
		exit(0);		
	}
	else{

		if(munmap(ptr, variable_size) != 0){
			perror("error in munmap\n");
			exit(0);
		}
	
		// de-redister variable
		tmp = unregister_name_space ( (long int) ptr);

		unlink(tmp->fname);
		return;
	}
}
#define free nvm_free

///////////////////////////////////////////////////////////////////
// Auxiliary functions
///////////////////////////////////////////////////////////////////

unsigned int f_idx = 0;

int get_file_index(void)
{
	f_idx++;	

	if ( f_idx > MAX_FILE_INDEX_LIMIT) {
		perror("Usuable file names are used up!");
		exit(0);		
	}

	return f_idx;
}
