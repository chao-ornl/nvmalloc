#include "nvm.h"

void nvm_checkpoint(int option)
{
	switch (option) {
		case 2:
			nvm_checkpoint_s2();
			break;
		case 3:
			nvm_Checkpoint_s3();
			break;
		default: 
			nvm_checkpoint_s1();
	}
}


void nvm_checkpoint_s1()
{
	/* Once this fun is called, it creates a checkpiont file (with a
	 * version number in a file name), and appends all mmapped files to
	 * this checkpoint file. 
	 *
	 * First checkpoint namespace data structure 
	 * Second scan the namespace and coyp valid mmapped files in the order
	 *
	 * A new checkpoint file is created every nvm_checkpoint()
	 */

#if 1
	static int ver = 0; // checkpoint file version 
	
	char chk_fname[20];

	sprintf(chk_fname, "%s-%d", "checkpoint", ver);

	ver++;

	if ( ( int fd_chk = open (chk_fname , O_RDWR | O_CREAT | O_APPEND, 0666) ) == -1 ) {
		fprintf(stderr, "cannot open %s\n", chk_fname);
		exit(0);
	}

	/* checkpoint namespace */
	int len;

	// create a header 
	struct header{
		int type; // 0: name space data structure, 1: mmapped files
		int size;
	};

	struct header tmp;

	tmp.type = 0;
	tmp.size = sizeof (var_info_table);

	// write a header
	if ( (len = write (fd_chk, &tmp, sizeof(tmp))) == -1 ) {
		fprintf(stderr, "fail to write1\n");
		exit(0);
	}

	// write actual data (checkpoint name space data structure) 
	if ( (len = write (fd_chk, var_info_table, sizeof(var_info_table)) ) == -1 ){
		fprintf(stderr, "fail to write2\n");
		exit(0);
	}

	/* checkpoint memory mapped files of variables */
	int i;
	char buffer[BUFFER_SIZE];
	for (i = 0 ; i < MAX_OPEN_FILE_CNT; i++)
	{
		if ( var_info_table[i].flag == 1 ){
			
			int fd_mmap_file;

			if ( (fd_mmap_file = open (var_info_table[i].fname, O_RDONLY) ) == -1 )
			{
				fprintf(stderr, "fail to open %s\n", var_info_table[i].fname);
				exit(0);
			}
		
			memset (buffer, 0, BUFFER_SIZE);

			int file_size = var_info_table[i].size;
			int offset = 0;

			// create a header
			struct header tmp;

			tmp.type = 1;
			tmp.size = file_size;

			// write a header
			if ( (len = write (fd_chk, &tmp, sizeof(tmp))) == -1 ) {
				fprintf(stderr, "fail to write3\n");
				exit(0);
			}

			// write actual data of memory mapped files of
			// variables 
			while (offset < file_size)
			{
				if ( read (fd_mmap_file, buffer, BUFFER_SIZE) < 0 ){
					fprintf(stderr, "fail to write4\n");
					exit(0);
				}
				if ( write (fd_chk, buffer, BUFFER_SIZE) < 0 ){
					fprintf(stderr, "fail to write5\n");
					exit(0);
				}
				offset += sizeof(buffer);
			}
			close(fd_mmap_file);
		}
	}

	close (fd_chk);
#endif
}

void nvm_checkpoint_s2()
{
#if 1
	/* This is a second scheme, which enhances the first scheme. 
	 * It only maintains the latest version of checkpoint file. 
	 * Checkpoint file is only updated with changes. 
	 */

	static int is_checkpoint_file = 0;
	int fd_chk;
	char chk_fname[] = "checkpoint";

	if ( is_checkpoint file = 0 ) { 

		if ( ( fd_chk = open (chk_fname , O_RDWR | O_CREAT | O_APPEND, 0666) ) == -1 ) {
			fprintf(stderr, "cannot open %s\n", chk_fname);
			exit(0);
		}
		 
		
	}
	else {
		if ( ( fd_chk = open (chk_fname , O_RDWR, 0666) ) == -1 ) {
			fprintf(stderr, "cannot open %s\n", chk_fname);
			exit(0);
		}
	}

#endif

	/* we need to think about this; 
	 * for example at the first chk, four variables are checkpointed and at
	 * the next chk time, only three variables need to be checkointed,
	 * which is among those three, only two are udpated from the previous
	 * chk time, and one is a new variable that needs to be checkopinted.
	 * In this case, those two could updated, however, the next one is new,
	 * so it coud updated, but now, the question is (i) are we going to
	 * support compaction to reduce the cost of memory space, or as this
	 * approach increases read and write operations of files,  this may not
	 * be necessarily required. Then, if it doesn't support the compaction,
	 * the, it needs to mark regions by regions to indicate the status of
	 * the region, makred invalid something like that, or else. (ii)
	 * another approach could be create a temporary file, and read a
	 * previous ...
	 *
	 *
	 * Compaction!!! 
	 */

	/*
	close (fd_chk);
	*/

}

void nvm_checkpoint_s3()
{
}
