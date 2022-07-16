#include "piping.h"
#include "calling.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int piping(char **argv){
    //パイプの必要数の検出と出現位置の記録
    int pipes = 0;
    int pc = DEFAULT_MAXPIPES;
    int *pipe_strpos = (int*)malloc(pc * sizeof(int));
    for(int i=0;i<argv[i] != NULL;i++){
        if(strcmp(argv[i],"|") == 0){
            argv[i] = NULL;
            if(pipes > pc){
                pc += DEFAULT_MAXPIPES;
                pipe_strpos = realloc(pipe_strpos,pc);
            }
            pipe_strpos[pipes++] = i;
        }
    }
    
    if(pipes == 0){
        waitchild(call(argv));
    }else{
        //必要なパイプの数分パイプを作成
        int **pipe_ins = (int**)malloc(pipes * sizeof(int));
        for(int i=0;i<pipes;i++){
            pipe_ins[i] = (int*)malloc(sizeof(int) * 2);
        }




        free(pipe_strpos);
        for(int i=0;i<pipes;i++)free(pipe_ins[i]);
        free(pipe_ins);
    }

    return 0;
}

