#include "piping.h"

#include <unistd.h>
#include <sys/types.h>


int piping(char **argv){
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


}

