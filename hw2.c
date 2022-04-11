#include <dlfcn.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#define bufferSize 100
static int (*old_open)(const char *pathname, int flags, mode_t mode) = NULL;
static ssize_t (*old_read)(int fd, void *buf, size_t count) = NULL;
static int (*old_chmod)(const char *pathname, mode_t mode);
static int (*old_chown)(const char *pathname, uid_t owner, gid_t group);
static int (*old_close)(int fd);
static int (*old_creat)(const char *pathname, mode_t mode);
static int (*old_remove)(const char *pathname);
static int (*old_rename)(const char *oldpath, const char *newpath);
static ssize_t (*old_write)(int fd, const void *buf, size_t count);
static FILE* (*old_tmpfile)(void);
static FILE* (*old_fopen)(const char *pathname, const char *mode);
static int (*old_fclose)(FILE *stream);
static size_t (*old_fread)(void *ptr, size_t size, size_t nmemb, FILE *stream);
static size_t (*old_fwrite)(const void *ptr, size_t size, size_t nmemb, FILE *stream);

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
	char linkName[bufferSize]={};
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

int chmod(const char *pathname, mode_t mode){
	if(old_chmod == NULL){
		void *handle = dlopen("libc.so.6",RTLD_LAZY);
		if(handle != NULL){
			old_chmod = dlsym(handle, "chmod");
		}else{
			printf("handle == NULL\n");
		}
	}

	int rtv = old_chmod(pathname,mode);
	char* abs_path = realpath(pathname,NULL);
	if(abs_path ==NULL){ // FAIL
		printf("[logger] chmod(untouched, %o) = %d\n", mode, rtv);
	}else{ // SUCCESS
		printf("[logger] chmod(\"%s\", %o) = %d\n", abs_path, mode, rtv);
	}
	return rtv;
}

int chown(const char *pathname, uid_t owner, gid_t group){
	if (old_chown == NULL){
		void *handle = dlopen("libc.so.6",RTLD_LAZY);
		if(handle != NULL){
			old_chown = dlsym(handle, "chown");
		}else{
			printf("handle == NULL\n");
		}
	}

	int rtv = old_chown(pathname,owner,group);
	char* abs_path = realpath(pathname,NULL);
	if(abs_path ==NULL){ // FAIL
		printf("[logger] chown(untouched, %d, %d) = %d\n", owner, group, rtv);
	}else{ // SUCCESS
		printf("[logger] chown(\"%s\", %d, %d) = %d\n", abs_path, owner, group, rtv);
	}
	return rtv;
}

int close(int fd){
	if(old_close == NULL){
		void *handle = dlopen("libc.so.6",RTLD_LAZY);
		if(handle != NULL){
			old_close = dlsym(handle, "close");
		}else{
			printf("handle == NULL\n");
		}
	}

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
	char linkName[bufferSize]={};
	int linkNameLength = readlink(fdPath, linkName, bufferSize);
	int rtv = old_close(fd);
	printf("[logger] close(\"%s\") = %d\n", linkName, rtv);
	return rtv;
}

int creat(const char *pathname, mode_t mode){
	if(old_creat == NULL){
		void *handle = dlopen("libc.so.6",RTLD_LAZY);
		if(handle != NULL){
			old_creat = dlsym(handle, "creat");
		}else{
			printf("handle == NULL\n");
		}
	}

	int rtv = old_creat(pathname,mode);
	char* abs_path = realpath(pathname,NULL);
	if(abs_path ==NULL){ // FAIL
		printf("[logger] creat(untouched, %o) = %d\n", mode, rtv);
	}else{ // SUCCESS
		printf("[logger] creat(\"%s\", %o) = %d\n", abs_path, mode, rtv);
	}
	return rtv;
}

int remove(const char *pathname){
	if (old_remove == NULL){
		void *handle = dlopen("libc.so.6",RTLD_LAZY);
		if(handle != NULL){
			old_remove = dlsym(handle, "remove");
		}else{
			printf("handle == NULL\n");
		}
	}

	char* abs_path = realpath(pathname,NULL);
	int rtv = old_remove(pathname);
	if(abs_path ==NULL){ // FAIL
		printf("[logger] remove(untouched) = %d\n", rtv);
	}else{ // SUCCESS
		printf("[logger] remove(\"%s\") = %d\n", abs_path, rtv);
	}
	return rtv;
}

int rename(const char *oldpath, const char *newpath){
	if (old_rename == NULL){
		void *handle = dlopen("libc.so.6",RTLD_LAZY);
		if(handle != NULL){
			old_rename = dlsym(handle, "rename");
		}else{
			printf("handle == NULL\n");
		}
	}

	char* abs_oldpath = realpath(oldpath,NULL);
	int rtv = old_rename(oldpath,newpath);
	char *abs_newpath = realpath(newpath, NULL);

	if(abs_oldpath == NULL){
		abs_oldpath = "untouched";
	}
	if(abs_newpath ==NULL){
		abs_newpath = "untouched";
	}
	printf("[logger] rename(\"%s\",\"%s\") = %d\n", abs_oldpath, abs_newpath, rtv);

	return rtv;
}

ssize_t write(int fd, const void *buf, size_t count){
	if(old_write == NULL){
		void *handle = dlopen("libc.so.6",RTLD_LAZY);
		if(handle != NULL){
			old_write = dlsym(handle, "write");
		}else{
			printf("handle == NULL\n");
		}
	}
	int rtv = old_write(fd,buf,count);
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
	char linkName[bufferSize]={};
	int linkNameLength = readlink(fdPath, linkName, bufferSize);

	// Handle Buffer 32Byte & Output logger
	if(rtv <=0){
		// Write Nothing Or Fail
		printf("[logger] write(\"%s\", \"\", %ld) = %d\n", linkName, count, rtv);
	}else{
		// Write Something
		char *bufResult = bufArguHandling((char*)buf, rtv);
		printf("[logger] write(\"%s\", \"%s\", %ld) = %d\n", linkName, bufResult, count, rtv);
	}
	return rtv;
}

FILE *tmpfile(void){
	if(old_tmpfile == NULL){
		void *handle = dlopen("libc.so.6",RTLD_LAZY);
		if(handle != NULL){
			old_tmpfile = dlsym(handle, "tmpfile");
		}else{
			printf("handle == NULL\n");
		}
	}
	FILE *rtv = old_tmpfile();

	printf("[logger] tmpfile() = %p\n", rtv);
	return rtv;
}

FILE *fopen(const char *pathname, const char *mode){
	if(old_fopen == NULL){
		void *handle = dlopen("libc.so.6",RTLD_LAZY);
		if(handle != NULL){
			old_fopen = dlsym(handle, "fopen");
		}else{
			printf("handle == NULL\n");
		}
	}
	FILE *rtv = old_fopen(pathname,mode);
	char* abs_path = realpath(pathname,NULL);
	if(abs_path ==NULL){ // FAIL
		printf("[logger] fopen(untouched, %s) = %p\n", mode, rtv);
	}else{ // SUCCESS
		printf("[logger] fopen(\"%s\", %s) = %p\n", abs_path, mode, rtv);
	}
	return rtv;
}

int fclose(FILE *stream){
	if (old_fclose == NULL){
		void *handle = dlopen("libc.so.6", RTLD_LAZY);
		if (handle != NULL){
			old_fclose = dlsym(handle, "fclose");
		}else{
			printf("handle == NULL\n");
		}
	}

	char fdPath[bufferSize] = {};
	char pidStr[10] ={};
	char fdStr[10] ={};

	// Make /proc/{pid}/fd/{fd}
	sprintf(pidStr,"%d",getpid());
	int fd = fileno(stream);
	sprintf(fdStr,"%d",fd);
	strcpy(fdPath,"/proc/");
	strcat(fdPath,pidStr);
	strcat(fdPath,"/fd/");
	strcat(fdPath,fdStr);

	// Handle fd NAME (readlink)
	char linkName[bufferSize]={};
	int linkNameLength = readlink(fdPath, linkName, bufferSize);
	int rtv = old_fclose(stream);
	printf("[logger] fclose(\"%s\") = %d\n", linkName, rtv);
	return rtv;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream){
	if (old_fread == NULL){
		void *handle = dlopen("libc.so.6",RTLD_LAZY);
		if(handle != NULL){
			old_fread = dlsym(handle, "fread");
		}else{
			printf("handle == NULL\n");
		}
	}
	size_t rtv = old_fread(ptr,size,nmemb,stream);

	char fdPath[bufferSize] = {};
	char pidStr[10] ={};
	char fdStr[10] ={};

	// Make /proc/{pid}/fd/{fd}
	sprintf(pidStr,"%d",getpid());
	int fd = fileno(stream);
	sprintf(fdStr,"%d",fd);
	strcpy(fdPath,"/proc/");
	strcat(fdPath,pidStr);
	strcat(fdPath,"/fd/");
	strcat(fdPath,fdStr);

	// Handle fd NAME (readlink)
	char linkName[bufferSize]={};
	int linkNameLength = readlink(fdPath, linkName, bufferSize);

	// Handle Buffer 32Byte & Output logger
	if(rtv <=0){
		// Read Nothing
		printf("[logger] fread(\"\", %ld, %ld, \"%s\") = %ld\n", size, nmemb, linkName, rtv);
	}else{
		// Read Something
		char *bufResult = bufArguHandling(ptr, rtv);
		printf("[logger] fread(\"%s\", %ld, %ld, \"%s\") = %ld\n", bufResult, size, nmemb, linkName, rtv);
	}
	return rtv;
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream){
	if (old_fwrite == NULL){
		void *handle = dlopen("libc.so.6",RTLD_LAZY);
		if(handle != NULL){
			old_fwrite = dlsym(handle, "fwrite");
		}else{
			printf("handle == NULL\n");
		}
	}
	size_t rtv = old_fwrite(ptr,size,nmemb,stream);

	char fdPath[bufferSize] = {};
	char pidStr[10] ={};
	char fdStr[10] ={};

	// Make /proc/{pid}/fd/{fd}
	sprintf(pidStr,"%d",getpid());
	int fd = fileno(stream);
	sprintf(fdStr,"%d",fd);
	strcpy(fdPath,"/proc/");
	strcat(fdPath,pidStr);
	strcat(fdPath,"/fd/");
	strcat(fdPath,fdStr);

	// Handle fd NAME (readlink)
	char linkName[bufferSize]={};
	int linkNameLength = readlink(fdPath, linkName, bufferSize);

	// Handle Buffer 32Byte & Output logger
	if(rtv <=0){
		// Write Nothing
		printf("[logger] fwrite(\"\", %ld, %ld, \"%s\") = %ld\n", size, nmemb, linkName, rtv);
	}else{
		// Write Something
		char *bufResult = bufArguHandling((char*)ptr, rtv);
		printf("[logger] fwrite(\"%s\", %ld, %ld, \"%s\") = %ld\n", bufResult, size, nmemb, linkName, rtv);
	}
	return rtv;
}