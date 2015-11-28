FLAGS = -Wall -std=c99

all:		exe

debug: 		FLAGS += -g
debug:		exe

exe:		main.o utility.o filetable.o directoryparse.o ls.o parseinput.o size.o
		gcc $(FLAGS) -o prog.x main.o utility.o filetable.o directoryparse.o ls.o parseinput.o size.o
main.o:		main.c
		gcc $(FLAGS) -c main.c
utility.o:	utility.c utility.h
		gcc $(FLAGS) -c utility.c
filetable.o:	filetable.c filetable.h
		gcc $(FLAGS) -c filetable.c
directoryparse.o:	directoryparse.c directoryparse.h
		gcc $(FLAGS) -c directoryparse.c
ls.o:		ls.c ls.h
		gcc $(FLAGS) -c ls.c
parseinput.o:	parseinput.c parseinput.h
		gcc $(FLAGS) -c parseinput.c
size.o:		size.c size.h
		gcc $(FLAGS) -c size.c
clean:
		rm *.o prog.x
