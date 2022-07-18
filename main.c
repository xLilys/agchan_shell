#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//my headers
#include "ui.h"
#include "piping.h"
#include "calling.h"
#include "bic.h"

int main(int argc,int *argv[]){

	while(1){
		char *input_str = read_command();
		if(input_str==NULL)continue;
		char **elements = parse_command(input_str);

		int elc = 0;
		while(elements[elc] != NULL)elc++;
		
		//様々場合分けして実行
		//cdコマンド

		char tmp[] = "cd";
		if(strcmp(elements[0],tmp) == 0){
			if(!!chdir(elements[1])){
				fprintf(stderr,"No such directory.\n");
			}
		}else if(strcmp(elements[0],"update") == 0){
			char *update_command[] = {"make",NULL};
			if(waitchild(call(update_command))){
				fprintf(stderr,"update failed.\n");
			}else{
				fprintf(stderr,"update succeeded.\n");
				for(int i=0;i<elc;i++){
					if(elements[i] != NULL)free(elements[i]);
				}
				free(elements);
				free(input_str);
				execl("./run","./run",NULL);
			}
		}else{
			piping(elements);
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


