#include <dlfcn.h>
#include <stdio.h>
#include <sys/types.h>
#include <limits.h>
#include <stdlib.h>

static int (*old_open)(const char *pathname, int flags, mode_t mode) = NULL;
static ssize_t (*old_read)(int fd, void *buf, size_t count) = NULL;

int open(const char *pathname, int flags, mode_t mode){
	if(old_open == NULL){
		void *handle = dlopen("libc.so.6",RTLD_LAZY);
		if(handle != NULL){
			old_open = dlsym(handle, "open");
		}else{
			printf("handle == NULL\n");
		}
	}

	// Mode handling
	if (mode != 448 && mode != 256 && mode != 128 && mode != 64 
	&& mode != 56 && mode != 32 && mode != 16 && mode != 8 
	&& mode != 7 && mode != 4 && mode != 2 && mode != 1){
		mode = 0;
	}
	int rtv = old_open(pathname,flags,mode);
	char* abs_path = realpath(pathname,NULL);
	if(abs_path ==NULL){ // FAIL
		printf("[logger] open(untouched, %o, %o) = %d\n", flags, mode, rtv);
	}else{ // SUCCESS
		printf("[logger] open(\"%s\", %o, %o) = %d\n", abs_path, flags, mode, rtv);
	}
	return rtv;
}

ssize_t read(int fd, void *buf, size_t count){
	printf("[logger] read is called: %d,%ld\n",fd,count);
	if(old_read == NULL){
		void *handle = dlopen("libc.so.6",RTLD_LAZY);
		if(handle != NULL){
			old_read = dlsym(handle, "read");
		}else{
			printf("handle == NULL\n");
		}
	}
	return old_read(fd,buf,count);
}

