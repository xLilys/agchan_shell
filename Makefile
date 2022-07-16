main:main.o ui.o calling.o piping.o
	gcc -Wall -O2 -o run main.o ui.o calling.o piping.o
	rm *.o
main.o:main.c
	gcc -c main.c
ui.o:ui.c
	gcc -c ui.c
calling.o:calling.c
	gcc -c calling.c
piping.o:piping.c
	gcc -c piping.c
clean:
	rm *.o