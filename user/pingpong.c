#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char *argv[]){
  int pipe1[2]; // Send to child
  int pipe2[2]; // Receive from parent
  if (pipe(pipe1) == -1 || pipe(pipe2) == -1){
    fprintf(2,"Pipe: pipe create error");
    exit(1);
  }

  int child_pid = fork();
  if (child_pid == -1){
    fprintf(2,"Fork: fork failed\n");
    exit(1);
  }
  else if (child_pid == 0){ // Child
    close(pipe1[1]); // close the write end for pipe1
    close(pipe2[0]); // close the read end for pipe2
    //        char buf[2];
    int buf[1];
    //        memset(buf, 0, sizeof(buf));
    if (read(pipe1[0],buf,sizeof(buf)) != sizeof(buf)){
      fprintf(2,"failed to read in child\n");
      exit(1);
    }
    close(pipe1[0]); // close the read end for pipe1
    int parent_id = getpid();
    printf("%d: received ping\n",parent_id);
    if (write(pipe2[1],buf,sizeof(buf)) != sizeof(buf)){
      fprintf(2,"failed to write in child\n");
      exit(1);
    }
    close(pipe2[1]); // close the write end for pipe2
    exit(0);
  }
  else { // parent
    close(pipe1[0]); // close the read end for pipe1
    close(pipe2[1]); // close the write end for pipe2
    int buf[1];
    buf[0] = getpid();
    if (write(pipe1[1],buf,sizeof(buf)) != sizeof(buf)){
      fprintf(2,"failed to write in parent\n");
      exit(1);
    }
    close(pipe1[1]);
    wait(0);
    if (read(pipe2[0],buf,sizeof(buf)) != sizeof(buf)){
      fprintf(2,"failed to read in parent\n");
      exit(1);
    }
    printf("%d: received pong\n",buf[0]);
    close(pipe2[0]);
    exit(0);
  }
}

//int main(){
//    int pid;
//    int p[2];
//    pipe(p);
//    if (fork() == 0) // child
//    {
//        pid = getpid();
//        char buf[2];
//        if (read(p[0],buf,1) != 1){
//            fprintf(2,"Failed to read in child\n");
//            exit(1);
//        }
//        close(p[0]);
//        printf("%d: received ping\n",pid);
//        if (write(p[1],buf,1) != 1){
//            fprintf(2,"failed to write in child\n");
//            exit(1);
//        }
//        close(p[1]);
//        exit(0);
//    } else {
//        pid = getpid();
//        char info[2] = "a";
//        char buf[2];
//        buf[1] = 0;
//        if (write(p[1],info,1) != 1){
//            fprintf(2,"failed to write in parents\n");
//            exit(1);
//        }
//        close(p[1]);
//        wait(0);
//        if (read(p[0],buf,1) != 1){
//            fprintf(2,"failed to read in parent\n");
//            exit(1);
//        }
//        printf("%d: received pong\n",pid);
//        close(p[0]);
//        exit(0);
//    }
//}
//
// Created by ASUS on 2023/8/23.
//
