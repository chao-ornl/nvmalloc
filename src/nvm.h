#ifndef _NVMALLOC_H
#define _NVMALLOC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define MAX_OPEN_FILE_CNT 1048576
#define MAX_FILE_INDEX_LIMIT 1048576 // 1024*1024

#define BUFFER_SIZE 4096

extern void nvm_init();
extern void nvm_exit();

extern void *nvm_malloc(size_t size);
extern void *nvm_calloc(size_t nmemb, size_t size);
extern void *nvm_realloc(void *ptr, size_t size);
extern void nvm_free(void *ptr);

#define malloc nvm_malloc
#define calloc nvm_calloc
#define realloc nvm_realloc
#define free nvm_free

// for namespace
typedef struct{
	long int addr;
	int fd;	
	//char *fname;
	char fname[80];
	int flag;
	int size;
}var_info;

extern var_info var_info_table[MAX_OPEN_FILE_CNT];
extern void create_name_space();
extern void destory_name_space();

void register_name_space( long int addr, int fd, char *fname, int size );
var_info *unregister_name_space( long int addr );
var_info *check_reuse_file( size_t var_size);
void return_file_name_to_addr( long int addr, char *fname );
int return_variable_size_to_addr( long int addr );
void update_name_space( char *file_name, int file_size, long int addr);
void print_all_name_space_info();

// interface for ssd information: start

/*
 * Each SSD device has a section in the config file as follow:
 *
 * title SSD
 *         Name            : Fusion IO
 *         Device          : /dev/fioa
 *         Mounted on      : /tmp/ssd1
 *         File System     : ext4
 *
 */

#define CONFIG_FILE "./nvm_config"

struct ssd_info{
	char name[64];
	char device[64];
	char mountdir[64];
	char fs[48]; // length of "fs" could be smaller
	struct ssd_info *prev;
	struct ssd_info *next;
} ssd_info;           // but try to make the size of the struct be 2^x

struct ssd_info *ssd_head;
struct ssd_info *ssd_tail;

extern void read_ssd_info();
extern void release_ssd_info();
// interface for ssd information: end


#endif
