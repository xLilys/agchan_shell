#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "builtin_commands.h"

//関係ないやつ-------------------------------
int __bcs_func__(int argc,char **argv);
//------------------------------------------


//登録した関数の数-------
#define BUILTIN_FUNCS 2
//----------------------

//ここに関数ポインタを追加
int (*bcs[BUILTIN_FUNCS])(int argc,char **argv) = {cd,exit_shell};


/*実装のルール
型:int func(int,char**)
返り値:実行(成功の可否に関わらず):1
      実行しない:0
*/

//ビルトインコマンドの実装################################################################################################################################################################################
int cd(int argc,char **argv){
    if(argc > 0){
        if(strcmp(argv[0],"cd") == 0){
            if(argc > 1){
                if(!!chdir(argv[1])){
                fprintf(stderr,"failed to switch directory.\n");
                }
            }else{
                fprintf(stderr,"too few arguments.");
            }
            return 1;
        }
    }
    return 0;
}

int exit_shell(int argc,char **argv){
    if(argc > 0){
        if(strcmp(argv[0],"exit") == 0){
            if(argc > 1){
                int exit_status = (int)atol(argv[1]);
                if(exit_status == 0L){
                    fprintf(stderr,"invalid exit status=%s\n",argv[1]);
                }else{
                    exit(exit_status);
                }
            }else if(argc > 2){
                fprintf(stderr,"too many arguments.\n");
            }else{
                exit(0);
            }
            return 1;
        }
    }
    return 0;
}
//######################################################################################################################################################################################################


int __bcs_func__(int argc,char **argv){
    int ck = 0;
    for(int i=0;i<BUILTIN_FUNCS;i++){
        ck |= bcs[i](argc,argv);
    }
    return ck;
}