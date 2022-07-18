main:main.o ui.o calling.o piping.o builtin_commands.o signal_handle.o
	gcc -Wall -O2 -o agsh main.o ui.o calling.o piping.o builtin_commands.o signal_handle.o
	rm *.o
main.o:main.c
	gcc -c main.c
ui.o:ui.c
	gcc -c ui.c
calling.o:calling.c
	gcc -c calling.c
piping.o:piping.c
	gcc -c piping.c
builtin_commands.o:builtin_commands.c
	gcc -c builtin_commands.c
signal_handle.o:signal_handle.c
	gcc -c signal_handle.c
clean:
	rm *.o