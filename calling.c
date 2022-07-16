#include "calling.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int call(char **argv){
        pid_t pid;
        pid = fork();
        if(pid<0){
                fprintf(stderr,"fork(2) failed\n");
                return 1;
        }
        if(pid == 0){
                execvp(argv[0],argv);
                perror(argv[0]);
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
                return 0;
        }
}
