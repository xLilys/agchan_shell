#include "calling.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int call(char **argv){
        //pipeでつなぐやつをやる
        //必要数を検出
        int pipes = 0;
        int *pipe = (int*)malloc(DEFAULT_MAXPIPES * sizeof(int));
        int pc = DEFAULT_MAXPIPES;
        for(int i=0;i<argv[i] != NULL;i++){
                if(strcmp(argv[i],"|") == 0){
                        argv[i] = NULL;
                        if(pipes > pc){
                                pc += DEFAULT_MAXPIPES;
                                pipe = realloc(pipe,pc);
                        }
                        pipe[pipes++] = i;
                }
        }
        for(int i=0;i<pc;i++){
                fprintf(stderr,"%d ",pipe[i]);
        }


        pid_t pid;
        pid = fork();
        if(pid<0){
                fprintf(stderr,"fork(2) failed\n");
                free(pipe);
                return 1;
        }
        if(pid == 0){
                execvp(argv[0],argv);
                perror(argv[0]);
                free(pipe);
                return 99;
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
        }
        
        free(pipe);
}
