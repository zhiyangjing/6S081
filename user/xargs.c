#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int readline(char *new_argv[32],int curr_argc){
    char buf[1024];
    int n = 0;
    while (read(0,buf + n,1)){
        if (n == 1023){
            fprintf(2,"argument too long\n");
            exit(1);
        }
        if (buf[n] == '\n'){
            buf[n] = 0;
            break;
        }
        n++;
    }
//    buf[n-1] = 0;
    if (n == 0) return 0;
//    int offset = 0;
    new_argv[curr_argc] = malloc(strlen(buf) + 1);
    strcpy(new_argv[curr_argc],buf);
//    new_argv[curr_argc++] = buf + offset;
    new_argv[++curr_argc] = 0;
    return curr_argc;
}

int main(int argc,char *argv[]){
    if (argc <= 1){
        fprintf(2,"Usage: xargs command (arg ...)\n");
        exit(1);
    }
    char *command = malloc(strlen(argv[1]) + 1);
    char *new_argv[MAXARG];
    strcpy(command,argv[1]);
    // Copy the argument
    for (int i = 1;i < argc;++i){
        new_argv[i - 1] = malloc(strlen(argv[i]) + 1);
        strcpy(new_argv[i - 1],argv[i]);
    }
    int curr_argc;
    while ((curr_argc = readline(new_argv,argc - 1)) != 0){
        new_argv[curr_argc] = 0;
        if (fork() == 0){
//            printf("-]%s[-\n",command);
//            for (int t = 0;t < argc;t++){
//                printf("->%s<-\n",new_argv[t]);
//            }
            exec(command,new_argv);
            fprintf(2,"exec failed\n");
            exit(1);
        }
        wait(0);
    }
    exit(0);
}

//int main(int argc,char *argv[]){
//    char* tarargv[MAXARG];
//    char* args[64];
//    int i = 0,j = 0;
//    char c;
//    while ((c = getchr()) != EOF){
//        if (c != '\n'){
//            tarargv[i][j++] = c;
//        } else {
//            i++;
//            j=0;
//        }
//    }
//}
//
// Created by ASUS on 2023/8/23.
//
