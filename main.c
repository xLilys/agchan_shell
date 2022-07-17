#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//my headers
#include "ui.h"
#include "piping.h"

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
			int el = 0;
			for(int i=0;elements[i + 1] != NULL;i++)el++;
			if(strcmp(elements[el],">") == 0){
				if(elements[el+1] == NULL){
					fprintf(stderr,"expected a string, but found end of the statement.\n%s\n",input_str);
					int i=0;
					while(input_str[i++] != '>')fprintf(stderr,"%c",' ');
					fprintf(stderr,"%c",'^');
				}
				//指示されたファイルにリダイレクト
				//FILE writefile = fopen("")
			}

			//char **output;
			//piping(elements,output,0);
		}


		int q=0;
		while(1){
			if(elements[q] != NULL)free(elements[q]);
			else break;
			q++;
		}
		free(elements);
		free(input_str);
		fprintf(stderr,"%c",'\n');
	}

	return 0;
}


