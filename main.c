#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

//my headers
#include "ui.h"
#include "calling.h"

int main(int argc,int *argv[]){

	while(1){
		char *input_str = read_command();
		if(input_str==NULL)continue;
		char **elements = parse_command(input_str);
		
		//様々場合分けして実行
		//cdコマンド

		char tmp[] = "cd";
		if(strcmp(elements[0],tmp) == 0){
			if(!!chdir(elements[1])){
				fprintf(stderr,"No such directory.\n");
			}
		}else{
			call(elements);
		}


		int q=0;
		while(1){
			if(elements[q] != NULL)free(elements[q]);
			else break;
			q++;
		}
		free(elements);
		free(input_str);
		putchar('\n');
	}

	return 0;
}


