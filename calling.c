#include "calling.h"

pid_t call(char **argv){
        int ret = -1;
        pid_t pid = fork();

        //子プロセス
        if(pid < 0){
                //生成失敗
                fprintf(stderr,"fork(2) failed.\n");
        }
        if(pid == 0){
                //子プロセス実行
                execvp(argv[0],argv);
                perror(argv[0]);
                exit(99);
        }
        return pid;
}


int waitchild(pid_t pid,int *status){
        //親プロセス
        if(status != NULL){
                waitpid(pid,status,0);//wait
                if(WIFEXITED(*status)){
                }else if(WIFSIGNALED(*status)){
                        return 1;
                }else{
                        fprintf(stderr,"abnormal exit\n");
                        return -1;
                }
        }else{
                int dummy;
                waitpid(pid,&dummy,0);//wait
                if(WIFEXITED(dummy)){
                }else if(WIFSIGNALED(dummy)){
                        return 1;
                }else{
                        fprintf(stderr,"abnormal exit\n");
                        return -1;
                }
        }
        return 0;
}
