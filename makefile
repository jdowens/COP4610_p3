FLAGS = -Wall

all:		exe

debug: 		FLAGS += -g
debug:		exe

exe:		main.o utility.o filetable.o
		gcc $(FLAGS) -o prog.x main.o utility.o filetable.o
main.o:		main.c
		gcc $(FLAGS) -c main.c
utility.o:	utility.c utility.h
		gcc $(FLAGS) -c utility.c
filetable.o:	filetable.c filetable.h
		gcc $(FLAGS) -c filetable.c
clean:
		rm *.o prog.x
