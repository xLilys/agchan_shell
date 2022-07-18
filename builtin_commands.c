#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "builtin_commands.h"

//プロトタイプ宣言---------------
int cd(int argc,char **argv);
//------------------------------

//関係ない関数
void bcs_init();

//ここにコマンドを追加
int (*bcs[])(int argc,char **argv) = {cd};



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
            
        }
    }
}
//######################################################################################################################################################################################################
