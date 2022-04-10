#include <dlfcn.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define bufferSize 100
static int (*old_open)(const char *pathname, int flags, mode_t mode) = NULL;
static ssize_t (*old_read)(int fd, void *buf, size_t count) = NULL;

int decimalToOctal(int decimalnum){
    int octalnum = 0, temp = 1;
    while (decimalnum != 0){
    	octalnum = octalnum + (decimalnum % 8) * temp;
    	decimalnum = decimalnum / 8;
        temp = temp * 10;
    }
    return octalnum;
}

char* bufArguHandling(void* buf, int count){
	char *source = buf;
	char *result = malloc(sizeof(char)*32);
	for(int i=0;i<count;i++){
		if(i==32){break;} // up to 32byte
		if(isprint(source[i])){
			result[i] = source[i];
		}else{
			result[i] = '.';
		}
	}
	return result;
}

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
	int decimalnum = mode;
	int octalmode = decimalToOctal(mode);
	if(octalmode > 777 || octalmode <0){
		mode =0;
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
	if(old_read == NULL){
		void *handle = dlopen("libc.so.6",RTLD_LAZY);
		if(handle != NULL){
			old_read = dlsym(handle, "read");
		}else{
			printf("handle == NULL\n");
		}
	}
	int rtv = old_read(fd,buf,count);
	char fdPath[bufferSize] = {};
	char pidStr[10] ={};
	char fdStr[10] ={};

	// Make /proc/{pid}/fd/{fd}
	sprintf(pidStr,"%d",getpid());
	sprintf(fdStr,"%d",fd);
	strcpy(fdPath,"/proc/");
	strcat(fdPath,pidStr);
	strcat(fdPath,"/fd/");
	strcat(fdPath,fdStr);

	// Handle fd NAME (readlink)
	char linkName[bufferSize];
	int linkNameLength = readlink(fdPath, linkName, bufferSize);

	// Handle Buffer 32Byte & Output logger
	if(rtv ==0){
		// Read Nothing
		printf("[logger] read(\"%s\", \"\", %ld) = %d\n", linkName, count, rtv);
	}else{
		// Read Something
		char *bufResult = bufArguHandling(buf, rtv);
		printf("[logger] read(\"%s\", \"%s\", %ld) = %d\n", linkName, bufResult, count, rtv);
	}
	return rtv;
}