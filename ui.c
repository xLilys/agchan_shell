#include "ui.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_command(){
	//get current directory
	int pbsize = PATH_BUFSIZE * sizeof(char) + 1;
	char *pathname = (char*)malloc(pbsize);
	while(getcwd(pathname,pbsize) == NULL){
		pbsize += PATH_BUFSIZE;
		pathname = realloc(pathname,pbsize);
	}

	strcat(pathname,">");
	fprintf(stderr,pathname);
	
	free(pathname);


	//buffer alloc
	char *inputbuff = (char*)malloc(INPUT_BUFSIZE * sizeof(char));

	int pos = 0;

	//NULL check
	if(!inputbuff){
		fprintf(stderr,"ash: input buffer allocation error\n");
		exit(EXIT_FAILURE);
	}

	//input
	int bufflen = INPUT_BUFSIZE;
	for(int i=0;i<INPUT_BUFSIZE * sizeof(char);i++){
		inputbuff[i] = '\0';
	}
	
	unsigned char nsp = 0;
	while(1){
		char c = fgetc(stdin);
		if(c != ' ')nsp = 1;
		if(!nsp)continue;
		//fprintf(stderr,"%d ",c);
		if(c == EOF || c == '\n' || c == 10){
			//1文字もない時はNULLを返す
			if(pos==0){
				free(inputbuff);
				return NULL;
			}
			//打ち終えたらNULL文字を付加してreturn
			inputbuff[pos++] = '\0';
			return inputbuff;
		}else{
			inputbuff[pos++] = c;
		}

		//文字数超過時はバッファを追加
		if(pos >= bufflen){
			bufflen += INPUT_BUFSIZE;
			inputbuff = realloc(inputbuff,bufflen);
			//NULL check
			if(!inputbuff){
				fprintf(stderr,"ash: input buffer reallocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}

char **parse_command(char *command){
	int pos = 0;
	//コマンドの区切りごとの文字列のポインタを格納する配列
	char **elements = (char**)malloc(INPUT_BUFSIZE * sizeof(char*));
	char *element = (char*)malloc(INPUT_BUFSIZE * sizeof(char));

	if(!elements){
		fprintf(stderr,"ash: parsing memory allocation error\n");
		exit(EXIT_FAILURE);
	}

	int index = 0;
	int ip = 0;
	int ep = 0;
	for(int i=0;i<INPUT_BUFSIZE * sizeof(char);i++)element[i] = 0;
	while(1){
		char c = command[index++];
		if(c == '\0' || c == EOF || c == 10 || c == '\n'){
			element[ip++] = '\0';
			elements[ep++] = element;
			elements[ep] = NULL;
			ip = 0;
			break;
		}else if(c == ' '){
			ip = 0;
			elements[ep++] = element;
			element = (char*)malloc(INPUT_BUFSIZE);
			for(int i=0;i<INPUT_BUFSIZE;i++){
				element[i] = '\0';
			}
		}else{
			element[ip++] = c;
		}
	}
	
	return elements;
}

