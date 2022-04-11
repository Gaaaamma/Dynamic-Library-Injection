#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int main(){
    char tempbuf[100]={0};    
    chmod("./test.txt",S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
    chown("./test.txt",1000,1000);
    creat("bbb.txt", S_IRWXU | S_IRWXG | S_IRWXO);

    int testFd = open("./test.txt",O_CREAT|O_APPEND,S_IRWXU|S_IRWXG|S_IRWXO);
    while(read(testFd,tempbuf,99)>0){
        //printf("%s",tempbuf);
        memset(tempbuf,0,sizeof(tempbuf));
    }
    close(testFd);

    int aaaFd = open("./aaa.txt",O_CREAT|O_APPEND|O_RDWR);
    char writeBuf[10] = {'1','2','3','4','5','\n','7','8','9','\0'};
    write(aaaFd, writeBuf,sizeof(writeBuf));
    close(aaaFd);

    rename("./bbb.txt","ccc.txt");
    remove("./ccc.txt");

    FILE *mFile =tmpfile();
    FILE *mFileOpen =fopen("./aaa.txt","a+");
    char *fwriteBuf = "fwrite\nfwrite\nfwrite\nfwrite\nfwrite\n";
    fwrite(fwriteBuf, sizeof(char), 35, mFileOpen);
    fclose(mFileOpen);

    mFileOpen =fopen("./aaa.txt","r");
    while (fread(tempbuf, sizeof(char), 40, mFileOpen)){
        //printf("%s",tempbuf);
        memset(tempbuf,0,sizeof(tempbuf));
    }
    fclose(mFile);
    fclose(mFileOpen);
    return 0;
}