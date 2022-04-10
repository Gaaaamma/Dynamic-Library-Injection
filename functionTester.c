#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(){
    printf("getuid() -> %d\n",getuid());
    
    int testFd = open("./test.txt",O_CREAT);
    printf("open() Fd -> %d\n",testFd);
    return 0;
}