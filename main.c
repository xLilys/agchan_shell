#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//my headers
#include "ui.h"
#include "piping.h"

#define READ_BUFSIZE 100

int main(int argc,int *argv[]){

	while(1){
		char *input_str = read_command();
		if(input_str==NULL)continue;
		char **elements = parse_command(input_str);


		char ttmp[] = "ash";
		if(strcmp(elements[0],ttmp) == 0){
			free(input_str);
			int q=0;
			while(1){
				if(elements[q] != NULL)free(elements[q]);
				else break;
				q++;
			}
			free(elements);
			execl("./run","/run",NULL);
		}

		int parsed_args = 0;
		while(elements[parsed_args] != NULL)parsed_args++;

		//様々場合分けして実行
		//cdコマンド
		char tmp[] = "cd";
		if(strcmp(elements[0],tmp) == 0){
			if(!!chdir(elements[1])){
				fprintf(stderr,"No such directory.\n");
			}

		}else{
			int el = 0;
			for(int i=0;elements[i+1] != NULL;i++)el = i;

			//fprintf(stderr,"%d\n",parsed_args);

			//入力のリダイレクト
			//ファイルを読み込んで埋め込みでいいのかな？
			for(int i=0;i<parsed_args;i++){
				if((i != parsed_args - 1) && (strcmp(elements[i],"<") == 0)){
					//見つけたらファイルが存在するか確認
					if(elements[i] == NULL){
						//存在していなければエラー
						fprintf(stderr,"expected a string, but found end of the statement.\n%s\n",input_str);
						int i=0;
						while(input_str[i++] != '<')fprintf(stderr,"%c",' ');
						fprintf(stderr,"%c",'^');
					}else{
						//存在していたらファイルを開く
						FILE *readfile = fopen(elements[i + 1],"r");
						if(readfile == NULL){
							//失敗
							fprintf(stderr,"file cannot open. %s\n",elements[i + 1]);
						}else{
							int buflen = READ_BUFSIZE;
							char *readbuf = (char*)malloc(sizeof(char) * buflen);
						}
					}
				}
			}
			
			
			
			if(strcmp(elements[el],">") == 0){
				//出力の終端リダイレクト
				if(elements[el+1] == NULL){
					//リダイレクト先ファイルがない時のエラー出力
					fprintf(stderr,"expected a string, but found end of the statement.\n%s\n",input_str);
					int i=0;
					while(input_str[i++] != '>')fprintf(stderr,"%c",' ');
					fprintf(stderr,"%c",'^');

				}else{
					//書き込みモードでファイルを開く
					FILE *writefile = fopen(elements[el + 1],"w");

					if(writefile == NULL){
						//fopen失敗
						fprintf(stderr,"file cannnot open. %s",elements[el + 1]);
					}else{
						//成功
						//出力にパイプさせてコマンド実行
						free(elements[el]);//書き換える前にfree
						elements[el] = NULL;//いらん引数が渡らないように>をNULLに書き換え

						char *output;
						//fprintf(stderr,"%x\n",&output);
						if(!piping(elements,&output,1)){
							//実行成功
							fputs(output,writefile);
						}else{
							//実行失敗
							//エラー処理はもうしてあるから特になんもせん
						}
						fclose(writefile);
					}
				}

			}else if(strcmp(elements[el],">>") == 0){
				//追記モード
				if(elements[el+1] == NULL){
					//リダイレクト先ファイルがない時のエラー出力
					fprintf(stderr,"expected a string, but found end of the statement.\n%s\n",input_str);
					int i=0;
					while(input_str[i++] != '>')fprintf(stderr,"%c",' ');
					fprintf(stderr,"%c",'^');

				}else{
					//書き込みモードでファイルを開く
					FILE *writefile = fopen(elements[el + 1],"a");

					if(writefile == NULL){
						//fopen失敗
						fprintf(stderr,"file cannnot open. %s",elements[el + 1]);
					}else{
						//成功
						//出力にパイプさせてコマンド実行
						free(elements[el]);//書き換える前にfree
						elements[el] = NULL;//いらん引数が渡らないように>をNULLに書き換え

						char *output;
						//fprintf(stderr,"%x\n",&output);
						if(!piping(elements,&output,1)){
							//実行成功
							fputs(output,writefile);
						}else{
							//実行失敗
							//エラー処理はもうしてあるから特になんもせん
						}
						fclose(writefile);
					}
				}
			}else{
				//リダイレクトいらん
				char **dummy;
				piping(elements,dummy,0);
			}
		}


		//elementsのメモリを解法
		for(int i=0;i<parsed_args;i++){
			if(elements[i] == NULL)continue;
			free(elements[i]);
		}
		free(elements);
		free(input_str);
		fprintf(stderr,"%c",'\n');
	}


	return 0;
}


