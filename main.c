#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//my headers
#include "ui.h"
#include "piping.h"
#include "calling.h"
#include "builtin_commands.h"
#include "login_AA.h"


int main(int argc,int *argv[]){
	//ログインメッセージ
	printf("%s%s\n",loginAA,"Welcome agsh! made by @Agchan_Luice twitter → https://twitter.com/Agchan_Luice\ngithub repository → https://github.com/xLilys/agchan_shell");

	trap_signal(SIGINT,child_killer);

	while(1){
		char *input_str = read_command();
		if(input_str==NULL)continue;
		char **elements = parse_command(input_str);

		int elc = 0;
		while(elements[elc] != NULL)elc++;
		
		//ビルトインコマンドの実行
		int ck = __bcs_func__(elc,elements);

		//更新用
		if(strcmp(elements[0],"update") == 0){
			char *update_command[] = {"make",NULL};
			if(waitchild(call(update_command))){
				fprintf(stderr,"update failed.\n");
			}else{
				for(int i=0;i<elc;i++){
					if(elements[i] != NULL)free(elements[i]);
				}
				free(elements);
				free(input_str);
				fprintf(stderr,"update succeeded.\n");
				execl("./agsh","./agsh",NULL);
			}
		}else{
			if(!ck)piping(elements);
		}


		for(int i=0;i<elc;i++){
			if(elements[i] != NULL)free(elements[i]);
		}
		free(elements);
		free(input_str);
		putchar('\n');
	}

	return 0;
}


