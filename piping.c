#include "piping.h"
#include "calling.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum pipe_rw{
    pipe_read,
    pipe_write
};


int piping(char **argv){
    //リダイレクトする箇所を予めパイプで繋ぐ　リダイレクト元あるいは先がファイルであれば中身を入出力する
    int elc = 0;
    while(argv[elc] != NULL)elc++;

    fprintf(stderr,"%d\n",elc);

    //パイプの必要数の検出と出現位置の記録
    int pipes = 0;
    int pc = DEFAULT_MAXPIPES;
    unsigned int *pipe_strpos = (int*)malloc(pc * sizeof(int));
    for(int i=0;argv[i] != NULL;i++){
        if(strcmp(argv[i],"|") == 0){
            free(argv[i]);
            argv[i] = NULL;
            if(pipes > pc){
                pc += DEFAULT_MAXPIPES;
                pipe_strpos = realloc(pipe_strpos,pc);
            }
            pipe_strpos[pipes++] = i;
        }
    }

    if(pipes == 0){
        if(!waitchild(call(argv))){
            free(pipe_strpos);
            return -1;
        }
    }else{
        //必要なパイプの数分パイプを作成
        int **pipe_ins = (int**)malloc(pipes * sizeof(int));
        for(int i=0;i<pipes;i++){
            pipe_ins[i] = (int*)malloc(sizeof(int) * 2);
        }

        //pidを格納
        pid_t *child_pids = (pid_t*)malloc((pipes + 1) * sizeof(pid_t));

        //コマンドをforkして実行
        for(int i=0;i<pipes + 1;i++){
            //forkする前に最後の一回以外でパイプを作成
            if(i != pipes){
                pipe(pipe_ins[i]);
            }

            //パイプの個数+1個分(実行する数execのぶん)forkする まだ実行しない
            pid_t pid = fork();
            child_pids[i] = pid;

            if(pid < 0){
                //例外処理
                fprintf(stderr,"fork(2) failed.\n");
                break;//例外が起きたらループを抜ける

            }else if(pid == 0){
                //子プロセス
                if(i == 0){
                    //初回のコマンドは標準出力を次のパイプの入口にだけ繋げる
                    dup2(pipe_ins[0][1],1);

                    //閉じる
                    close(pipe_ins[0][0]);
                    close(pipe_ins[0][1]);


                }else if(i == pipes){
                    //最後のコマンドは標準入力を前のパイプの出口にだけ繋げる
                    dup2(pipe_ins[pipes-1][0],0);
                    
                    //閉じる
                    close(pipe_ins[pipes-1][0]);
                    close(pipe_ins[pipes-1][1]);


                }else{
                    //途中のコマンドはパイプを前も後ろも繋げる
                    //後
                    dup2(pipe_ins[i-1][0],0);
                    close(pipe_ins[i-1][0]);
                    close(pipe_ins[i-1][1]);
                    //前
                    dup2(pipe_ins[i][1],1);
                    close(pipe_ins[i][0]);
                    close(pipe_ins[i][1]);
                }

                //実行
                if(i == 0){
                    //初回は普通に実行
                    execvp(argv[0],argv);
                    exit(0);
                }else{
                    int pos = pipe_strpos[i - 1] + 1;
                    execvp(argv[pos],&argv[pos]);
                    exit(0);
                }
            }else if(i > 0){
                //親
                close(pipe_ins[i-1][0]);
                close(pipe_ins[i-1][1]);
            }
        }

        //待つ
        for(int i=0;i<pipes + 1;i++){
            waitchild(child_pids[i]);
        }


        free(pipe_strpos);
        for(int i=0;i<pipes;i++)free(pipe_ins[i]);
        free(pipe_ins);
    }

    return 0;
}

