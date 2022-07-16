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

        int **pipe = (int**)malloc(pipes * sizeof(int));
        for(int i=0;i<pipes;i++){
                pipe[i] = (int*)malloc(sizeof(int) * 2);
        }


        pid_t pid;
        pid = fork();

        int ret = -1;

        if(pid<0){
                fprintf(stderr,"fork(2) failed\n");
                free(pipe);
                ret = 1;
        }
        if(pid == 0){
                execvp(argv[0],argv);
                perror(argv[0]);
                free(pipe);
                ret = 99;
        }else{
                int status;
                waitpid(pid,&status,0);
                //printf("child (PID=%d) finished; ",pid);
                if(WIFEXITED(status)){
                        //printf("exit, status=%d\n",WEXITSTATUS(status));
                }else if(WIFSIGNALED(status)){
                        //printf("signal, sig=%d\n",WTERMSIG(status));
                }else{
                        fprintf(stderr,"abnormal exit\n");
                }
                ret = 0;
        }

        
        for(int i=0;i<pipes;i++)free(pipe[i]);
        free(pipe);
        return 0;
}
