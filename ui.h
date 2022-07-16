#ifndef UI_H
#define UI_H

#define INPUT_BUFSIZE 256
#define PATH_BUFSIZE 256

#define strtest(str) for(int i=0;str[i] != '\0';i++)printf("%c%c",str[i],str[i+1] != '\0' ? ' ':'\n');
	

//入力された文字列を読み取る。メモリが動的確保されるので要free()
char *read_command();
char **parse_command(char *command);


#endif