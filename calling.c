#include "calling.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int call(char **argv){
        int ret = -1;
        pid_t pid = fork();

        //子プロセス
        if(pid < 0){
                //生成失敗
                fprintf(stderr,"fork(2) failed.\n");
                ret = 1;
        }
        if(pid == 0){
                //子プロセス
                execvp(argv[0],argv);
                perror(argv[0]);
                ret = 99;
        }else{
                //親プロセス
                int status;
                waitpid(pid,&status,0);//wait
                 if(WIFEXITED(status)){
                }else if(WIFSIGNALED(status)){
                }else{
                        fprintf(stderr,"abnormal exit\n");
                }
                ret = 0;
        }
        return ret;
}

