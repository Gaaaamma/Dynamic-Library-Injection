#include <dlfcn.h>
#include <stdio.h>
#include <sys/types.h>

static int (*old_open)(const char *pathname, int flags) = NULL;
uid_t getuid(void){
	printf("[logger] getuid is called\n");
	return 0;
}

int open(const char *pathname, int flags){
	printf("[logger] open is called: %s,%d\n",pathname,flags);
	if(old_open == NULL){
		/*
		void *handle = dlopen("libc.so.6",RTLD_LAZY);
		if(handle != NULL){
			//old_open = dlsym(handle, "open");
			printf("handle GOT something\n");
		}else{
			printf("handle == NULL\n");
		}
		*/
	}
	return 100;
}

