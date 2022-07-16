#include "calling.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int call(char **argv){
        //pipeでつなぐやつをやる
        //必要数を検出
        int pipes = 0;
        int *pipe_pos = (int*)malloc(DEFAULT_MAXPIPES * sizeof(int));
        int pc = DEFAULT_MAXPIPES;
        for(int i=0;i<argv[i] != NULL;i++){
                if(strcmp(argv[i],"|") == 0){
                        argv[i] = NULL;
                        if(pipes > pc){
                                pc += DEFAULT_MAXPIPES;
                                pipe_pos = realloc(pipe_pos,pc);
                        }
                        pipe_pos[pipes++] = i;
                }
        }

        int **pipe_ins = (int**)malloc(pipes * sizeof(int));
        for(int i=0;i<pipes;i++){
                pipe_ins[i] = (int*)malloc(sizeof(int) * 2);
        }

        int ret = -1;
        if(pipes == 0){
                pid_t pid = fork();
                //子プロセス
                if(pid < 0){
                        //生成失敗
                        fprintf(stderr,"fork(2) failed.\n");
                        ret = 1;

                }else if(pid == 0){
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
        }

        
        for(int i=0;i<pipes;i++)free(pipe_ins[i]);
        free(pipe_ins);
        return ret;
}
