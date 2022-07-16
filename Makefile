main:main.o ui.o calling.o pipe.o
	gcc -Wall -O2 -o run main.o ui.o calling.o pipe.o
main.o:main.c
	gcc -c main.c
ui.o:ui.c
	gcc -c ui.c
calling.o:calling.c
	gcc -c calling.c
pipe.o:pipe.c
	gcc -c pipe.c
clean:
	rm *.o