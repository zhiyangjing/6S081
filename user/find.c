#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
//#include <string.h>

char*
fmtname(char *path)
{
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    return buf;
}

void find(char* path, char *target){
    char back[DIRSIZ];
    strcpy(back,target);
    char buf[512],*p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path,0)) < 0){
        fprintf(2,"find: cannot open %s\n",path);
        exit(1);
    }

    if (fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n",path);
        exit(1);
    }

    switch(st.type){
        case T_FILE:
            fprintf(2,"Usage: find <path> <file_name>");
            close(fd);
            exit(1);
        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                fprintf(2,"find: path too long\n");
                close(fd);
                break;
            }
            strcpy(buf,path);
            p = buf + strlen(buf);
            *p++ = '/';
            while (read(fd,&de,sizeof(de)) == sizeof(de)){
                if (de.inum == 0)
                    continue;
                memmove(p,de.name,DIRSIZ);
                p[DIRSIZ] = 0;
                if (stat(buf,&st) < 0){
                    continue;
                }
                char* first = fmtname(buf);
                if (st.type == 2 && (strcmp(first,back) == 0)){
//                    for (int i = 0;i < 512;i++){
//                        if (buf[i] != ' '){
//                            putchar(buf[i])
//                        } else {
//                            break;
//                        }
//                        printf("\n");
//                    }
                    printf("%s\n",buf);
                } else if (st.type == 1){
                    if ((strcmp(first,"..            ") != 0) && (strcmp(first,".             ") != 0)){
                        find(buf,back);
                    }
                }
            }
    }
//    printf("%s <- 4\n",target);
    close(fd);
}


int main(int argc,char *argv[]){
//    int i;
    if (argc != 3){
        fprintf(2,"Usage: find <path> <file_name>\n");
        exit(1);
    }
    find(argv[1],fmtname(argv[2]));
//    printf("%s <- 7\n",fmtname(argv[2]));
    exit(0);
}
//
// Created by ASUS on 2023/8/20.
//

//|ls            |
//|ls            |
