all:		main.o utility.o
		gcc -o prog.x main.o utility.o
main.o:		main.c
		gcc -c main.c
utility.o:	utility.c utility.h
		gcc -c utility.c
clean:
		rm *.o prog.x
