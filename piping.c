#include "piping.h"
#include "calling.h"


int piping(char **argv){
    //リダイレクトする箇所を予めパイプで繋ぐ　リダイレクト元あるいは先がファイルとして存在しているかを確認し、存在すればforkしてパイプをつなぐ
    int elc = 0;
    while(argv[elc] != NULL)elc++;


    //パイプの必要数の検出と出現位置の記録
    int pipes = 0;
    int pc = DEFAULT_MAXPIPES;
    unsigned int *pipe_strpos = (unsigned int*)malloc(pc * sizeof(unsigned int));
    for(int i=0;argv[i] != NULL;i++){
        if(strcmp(argv[i],"|") == 0){
            if(argv[i+1] == NULL){
                fprintf(stderr,"Expected a string, but found end of the statement\n");
                for(int i=0;i<elc;i++)fprintf(stderr,"%s ",argv[i]);
                fprintf(stderr,"\n");
                for(int i=0;i<elc;i++)for(int j=0;argv[j] != NULL;j++)fprintf(stderr," ");
                fprintf(stderr,"^");
                return -1;
            }
            free(argv[i]);//NULLで上書き前にfree
            argv[i] = NULL;
            if(pipes > pc){
                pc += DEFAULT_MAXPIPES;
                pipe_strpos = realloc(pipe_strpos,pc);
            }
            pipe_strpos[pipes++] = i;
        }
    }


    //リダイレクトする箇所を検出、位置の記録
    int lrdc = 0;
    int lredpipes_count = DEFAULT_MAXREDIRECTS;
    unsigned int *leftred_pos = (unsigned int*)malloc(lredpipes_count * sizeof(unsigned int));
    for(int i=0;i<elc;i++){
        if(argv[i] == NULL)continue;
        if(strcmp(argv[i],"<") == 0){
            if(argv[i+1] == NULL){
                fprintf(stderr,"Expected a string, but found end of the statement\n");
                for(int i=0;i<elc;i++)fprintf(stderr,"%s ",argv[i]);
                fprintf(stderr,"\n");
                for(int i=0;i<elc;i++)for(int j=0;argv[j] != NULL;j++)fprintf(stderr," ");
                fprintf(stderr,"^");
                return -1;
            }
            if(lrdc > lredpipes_count){
                lredpipes_count += DEFAULT_MAXREDIRECTS;
                leftred_pos = realloc(leftred_pos,lredpipes_count);
            }
            leftred_pos[lrdc++] = i;
        }
    }
    leftred_pos = realloc(leftred_pos,lrdc);

    //右
    int rrdc = 0;
    int rredpipes_count = DEFAULT_MAXREDIRECTS;
    unsigned int *rightred_pos = (unsigned int*)malloc(rredpipes_count * sizeof(unsigned int));
    for(int i=0;i<elc;i++){
        if(argv[i] == NULL)continue;
        if(strcmp(argv[i],">") == 0){
            if(argv[i+1] == NULL){
                fprintf(stderr,"Expected a string, but found end of the statement\n");
                for(int i=0;i<elc;i++)fprintf(stderr,"%s ",argv[i]);
                fprintf(stderr,"\n");
                for(int i=0;i<elc;i++)for(int j=0;argv[j] != NULL;j++)fprintf(stderr," ");
                fprintf(stderr,"^");
                return -1;
            }
            if(rrdc > rredpipes_count){
                rredpipes_count += DEFAULT_MAXREDIRECTS;
                rightred_pos = realloc(rightred_pos,rredpipes_count);
            }
            rightred_pos[rrdc++] = i;
        }
    }
    rightred_pos = realloc(rightred_pos,rrdc);

    //右向き追記
    int rrdc_add = 0;
    int rredpipes_count_add = DEFAULT_MAXREDIRECTS;
    unsigned int *rightred_pos_add = (unsigned int*)malloc(rredpipes_count_add * sizeof(unsigned int));
    for(int i=0;i<elc;i++){
        if(argv[i] == NULL)continue;
        if(strcmp(argv[i],">>") == 0){
            if(rrdc > rredpipes_count_add){
                rredpipes_count_add += DEFAULT_MAXREDIRECTS;
                rightred_pos_add = realloc(rightred_pos,rredpipes_count_add);
            }
            rightred_pos_add[rrdc_add++] = i;
        }
    }
    rightred_pos_add = realloc(rightred_pos_add,rrdc_add);


    //pidを格納
    int pidc = 0;
    pid_t *child_pids = (pid_t*)malloc((pipes + 1) * sizeof(pid_t));


    if(pipes == 0){
        int rwpipes[3][2];
        if(lrdc > 0){
            int pos = leftred_pos[0];
            free(argv[pos]);
            argv[pos] = NULL;
            pipe(rwpipes[0]);
        }
        if(rrdc > 0){
            int pos = rightred_pos[0];
            free(argv[pos]);
            argv[pos] = NULL;
            pipe(rwpipes[1]);
        }
        if(rrdc_add > 0){
            int pos = rightred_pos_add[0];
            free(argv[pos]);
            argv[pos] = NULL;
            pipe(rwpipes[2]);
        }

        pid_t pid = fork();

        if(pid < 0){
            fprintf(stderr,"%s","redirecting error:fork(2) failed.");
            return -1;
        }else if(pid == 0){
            //子
            if(lrdc > 0){
                dup2(rwpipes[0][0],0);
                close(rwpipes[0][0]);
                close(rwpipes[0][1]);
            }
            if(rrdc > 0){
                dup2(rwpipes[1][1],1);
                close(rwpipes[1][0]);
                close(rwpipes[1][1]);
            }
            if(rrdc_add > 0){
                dup2(rwpipes[2][1],1);
                close(rwpipes[2][0]);
                close(rwpipes[2][1]);
            }

            execvp(argv[0],argv);
            exit(0);
        }else{
            //親
            if(lrdc>0){
                int pos = leftred_pos[0];
                FILE *readfile = fopen(argv[pos + 1],"r");
                
                int cs = 0;
                int readlen = DEFAULT_MAXREADBUF;
                char *readbuf = (char*)malloc(sizeof(char) * readlen);
                while(1){
                    char c = fgetc(readfile);
                    if(c == EOF)break;
                    readbuf[cs++] = c;
                    if(cs > readlen){
                        readlen += DEFAULT_MAXREADBUF;
                        readbuf = realloc(readbuf,readlen);
                    }
                }

                write(rwpipes[0][1],readbuf,cs);
                free(readbuf);
                fclose(readfile);

                close(rwpipes[0][0]);
                close(rwpipes[0][1]);
            }
            if(rrdc > 0){
                int pos = rightred_pos[0];
                FILE *writefile = fopen(argv[pos + 1],"w");

                int writelen = DEFAULT_MAXWRITEBUF;
                char *writebuf = (char*)malloc(sizeof(char) * writelen);
                while(1){
                    int res = read(rwpipes[1][0],writebuf,writelen);
                    if(!(res < writelen)){
                        writelen += DEFAULT_MAXWRITEBUF;
                        writebuf = realloc(writebuf,writelen);
                    }else{
                        break;
                    }
                }
                fputs(writebuf,writefile);
                free(writebuf);
                fclose(writefile);

                close(rwpipes[1][0]);
                close(rwpipes[1][1]);
            }
            if(rrdc_add > 0){
                int pos = rightred_pos_add[0];
                FILE *writefile = fopen(argv[pos + 1],"a");

                int writelen = DEFAULT_MAXWRITEBUF;
                char *writebuf = (char*)malloc(sizeof(char) * writelen);
                while(1){
                    int res = read(rwpipes[2][0],writebuf,writelen);
                    if(!(res < writelen)){
                        writelen += DEFAULT_MAXWRITEBUF;
                        writebuf = realloc(writebuf,writelen);
                    }else{
                        break;
                    }
                }
                fputs(writebuf,writefile);
                free(writebuf);
                fclose(writefile);

                close(rwpipes[2][0]);
                close(rwpipes[2][1]);
            }
        }

        waitchild(pid);

    }else{
        //必要なパイプの数分パイプを作成
        int **pipe_ins = (int**)malloc(pipes * sizeof(int));
        for(int i=0;i<pipes;i++){
            pipe_ins[i] = (int*)malloc(sizeof(int) * 2);
        }

        //リダイレクト用のパイプ
        int **left_redpipe = (int**)malloc(sizeof(int) * lrdc);
        int **right_redpipe = (int**)malloc(sizeof(int) * rrdc);
        int **right_redpipe_add = (int**)malloc(sizeof(int) * rrdc_add);

        for(int i=0;i<lrdc;i++)left_redpipe[i] = (int*)malloc(sizeof(int) * 2);
        for(int i=0;i<rrdc;i++)right_redpipe[i] = (int*)malloc(sizeof(int) * 2);
        for(int i=0;i<rrdc_add;i++)right_redpipe_add[i] = (int*)malloc(sizeof(int) * 2);

        for(int i=0;i<lrdc;i++)pipe(left_redpipe[i]);
        for(int i=0;i<rrdc;i++)pipe(right_redpipe[i]);
        for(int i=0;i<rrdc_add;i++)pipe(right_redpipe_add[i]);

        lrdc = rrdc = rrdc_add = 0;

        //コマンドをforkして実行
        for(int i=0;i<pipes + 1;i++){
            //forkする前に最後の一回以外でパイプを作成
            if(i != pipes){
                pipe(pipe_ins[i]);
            }

            //パイプの個数+1個分(実行する数execのぶん)forkする まだ実行しない
            pid_t pid = fork();
            child_pids[pidc++] = pid;

            if(pid < 0){
                //例外処理
                fprintf(stderr,"fork(2) failed.\n");
                break;//例外が起きたらループを抜ける

            }else if(pid == 0){
                //リダイレクトがあれば親とパイプをつなぐ
                
                if(i == 0){
                    //左
                    for(int j=0;argv[j] != NULL;j++){
                        if(strcmp(argv[j],"<") == 0){
                            free(argv[j]);
                            argv[j] = NULL;
                            dup2(left_redpipe[lrdc][0],0);
                            close(left_redpipe[lrdc][0]);
                            close(left_redpipe[lrdc][1]);
                            lrdc++;
                        }
                    }

                    //右
                    for(int j=0;argv[j] != NULL;j++){
                        if(strcmp(argv[j],">") == 0){
                            free(argv[j]);
                            argv[j] = NULL;
                            dup2(right_redpipe[rrdc][1],1);
                            close(right_redpipe[rrdc][0]);
                            close(right_redpipe[rrdc][1]);
                            rrdc++;
                        }
                    }


                }else{
                    //左

                    for(int j=pipe_strpos[i-1] + 1;j<elc;j++){
                        if(argv[j] == NULL)continue;
                        if(strcmp(argv[j],"<") == 0){
                            free(argv[j]);
                            argv[j] = NULL;
                            dup2(left_redpipe[lrdc][0],0);
                            close(left_redpipe[lrdc][0]);
                            close(left_redpipe[lrdc][1]);
                            lrdc++;
                        }
                    }
                    
                    //右
                    for(int j=pipe_strpos[i-1] + 1;j<elc;j++){
                        if(argv[j] == NULL)continue;
                        if(strcmp(argv[j],">") == 0){
                            free(argv[j]);
                            argv[j] = NULL;
                            dup2(right_redpipe[rrdc][1],1);
                            close(right_redpipe[rrdc][0]);
                            close(right_redpipe[rrdc][1]);
                            rrdc++;
                        }
                    }
                }

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
            }else{
                //親プロセス
                if(i > 0){
                    //子プロセスとのパイプ
                    close(pipe_ins[i-1][0]);
                    close(pipe_ins[i-1][1]);
                }
                
                
                //リダイレクト用のパイプ
                if(i == 0){
                    //左
                    for(int j=0;argv[j] != NULL;j++){
                        if(strcmp(argv[j],"<") == 0){
                            free(argv[j]);
                            argv[j] = NULL;
                            int pos = j;
                            FILE *readfile = fopen(argv[pos + 1],"r");
                            
                            int cs = 0;
                            int readlen = DEFAULT_MAXREADBUF;
                            char *readbuf = (char*)malloc(sizeof(char) * readlen);
                            while(1){
                                char c = fgetc(readfile);
                                if(c == EOF)break;
                                readbuf[cs++] = c;
                                if(cs > readlen){
                                    readlen += DEFAULT_MAXREADBUF;
                                    readbuf = realloc(readbuf,readlen);
                                }
                            }

                            write(left_redpipe[lrdc][1],readbuf,cs);
                            free(readbuf);
                            fclose(readfile);

                            close(left_redpipe[lrdc][0]);
                            close(left_redpipe[lrdc][1]);
                            lrdc++;
                        }
                    }

                    //右
                    for(int j=0;argv[j] != NULL;j++){
                        if(strcmp(argv[j],">") == 0){
                            free(argv[j]);
                            argv[j] = NULL;
                            int pos = j;
                            FILE *writefile = fopen(argv[pos + 1],"w");

                            int writelen = DEFAULT_MAXWRITEBUF;
                            char *writebuf = (char*)malloc(sizeof(char) * writelen);
                            while(1){
                                int res = read(right_redpipe[rrdc][0],writebuf,writelen);
                                if(!(res < writelen)){
                                    writelen += DEFAULT_MAXWRITEBUF;
                                    writebuf = realloc(writebuf,writelen);
                                }else{
                                    break;
                                }
                            }
                            fputs(writebuf,writefile);
                            free(writebuf);
                            fclose(writefile);
                            

                            close(left_redpipe[rrdc][0]);
                            close(left_redpipe[rrdc][1]);
                            rrdc++;
                        }
                    }
                }else{
                    //左
                    for(int j=pipe_strpos[i-1] + 1;j<elc;j++){
                        if(argv[j] == NULL)continue;
                        if(strcmp(argv[j],"<") == 0){
                            free(argv[j]);
                            argv[j] = NULL;
                            int pos = j;
                            FILE *readfile = fopen(argv[pos + 1],"r");
                            
                            int cs = 0;
                            int readlen = DEFAULT_MAXREADBUF;
                            char *readbuf = (char*)malloc(sizeof(char) * readlen);
                            while(1){
                                char c = fgetc(readfile);
                                if(c == EOF)break;
                                readbuf[cs++] = c;
                                if(cs > readlen){
                                    readlen += DEFAULT_MAXREADBUF;
                                    readbuf = realloc(readbuf,readlen);
                                }
                            }

                            write(left_redpipe[lrdc][1],readbuf,cs);
                            free(readbuf);
                            fclose(readfile);

                            close(left_redpipe[lrdc][0]);
                            close(left_redpipe[lrdc][1]);
                            lrdc++;
                        }
                    }

                    //右
                    for(int j=pipe_strpos[i-1] + 1;j<elc;j++){
                        if(argv[j] == NULL)continue;
                        if(strcmp(argv[j],">") == 0){
                            free(argv[j]);
                            argv[j] = NULL;
                            int pos = j;
                            FILE *writefile = fopen(argv[pos + 1],"w");

                            int writelen = DEFAULT_MAXWRITEBUF;
                            char *writebuf = (char*)malloc(sizeof(char) * writelen);
                            while(1){
                                int res = read(right_redpipe[rrdc][0],writebuf,writelen);
                                if(!(res < writelen)){
                                    writelen += DEFAULT_MAXWRITEBUF;
                                    writebuf = realloc(writebuf,writelen);
                                }else{
                                    break;
                                }
                            }

                            fputs(writebuf,writefile);
                            free(writebuf);
                            fclose(writefile);
                            

                            close(left_redpipe[rrdc][0]);
                            close(left_redpipe[rrdc][1]);
                            rrdc++;
                        }
                    }
                }
            }
        }

        //待つ
        for(int i=0;i<pidc;i++){
            waitchild(child_pids[i]);
        }


        free(pipe_strpos);
        for(int i=0;i<pipes;i++)free(pipe_ins[i]);
        free(pipe_ins);
    }

    free(rightred_pos);
    free(leftred_pos);
    free(child_pids);
    return 0;
}

