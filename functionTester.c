#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main(){
    char tempbuf[100]={0};    
    chmod("./test.txt",S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
    int testFd = open("./test.txt",O_CREAT|O_APPEND,S_IRWXU|S_IRWXG|S_IRWXO);
    while(read(testFd,tempbuf,99)>0){
        printf("%s",tempbuf);
        memset(tempbuf,0,sizeof(tempbuf));
    }
    return 0;
}