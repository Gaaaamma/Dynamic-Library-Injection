#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    char *outputFile =NULL;
    char *soPath =NULL;
    int commandStartIndex =1;

    if(argc ==1){
        // No option
        fprintf(stdout,"no command given.\n");
        return 0;

    }else{
        // Check options -p -o --
        for(int i=1;i<argc;i++){
            char *minusPtr = strchr(argv[i], '-');
            if(i==1 && minusPtr==NULL){
                // argv[1] is command => Just break to ignore cmd options
                commandStartIndex =1 ;
                break;
            }

            if(minusPtr != NULL){  // Find '-' in argv[i]
                if(strlen(argv[i]) == 2){
                    if(strcmp((minusPtr+1),"o") ==0){ // Legal
                        if(i+1 <argc){
                            outputFile = argv[i + 1];
                        }
                        printf("outputFile is set: %s\n", outputFile);

                    }else if(strcmp((minusPtr+1),"p") ==0){ // Legal
                        if (i + 1 < argc){
                            soPath = argv[i + 1];
                        }
                        printf("soPath is set: %s\n", soPath);

                    }else if(strcmp((minusPtr+1),"-") ==0){ // Legal
                        if (i + 1 < argc){
                            commandStartIndex = i+1;
                        }
                        printf("command start at index: %d\n", commandStartIndex);
                        break; // since we ignore cmd options => break

                    }else{
                        // illegal option
                        printf("%s: invalid option -- \'%s\'\n",argv[0],minusPtr+1);
                        printf("usage: ./logger [-o file] [-p sopath] [--] cmd [cmd args ...]\n");
                        printf("-p: set the path to logger.so, default = ./logger.so\n");
                        printf("-o: print output to file, print to \"stderr\" if no file specified\n");
                        printf("--: separate the arguments for logger and for the command\n");
                        return 0;
                    }
                }else{
                    // Contain '-' but length isn't 2 => illegal
                    printf("%s: invalid option -- \'%s\'\n", argv[0], minusPtr + 1);
                    printf("usage: ./logger [-o file] [-p sopath] [--] cmd [cmd args ...]\n");
                    printf("-p: set the path to logger.so, default = ./logger.so\n");
                    printf("-o: print output to file, print to \"stderr\" if no file specified\n");
                    printf("--: separate the arguments for logger and for the command\n");
                    return 0;
                }
            }
        }
        // TODO
        // ...
    }
    //system("sh -c /bin/ls");
}