FLAGS = -Wall -std=c99

all:		exe

debug: 		FLAGS += -g
debug:		exe

exe:		utility.o filetable.o directoryparse.o ls.o parseinput.o size.o program.o read.o cd.o openclose.o write.o
		gcc $(FLAGS) -o prog.x utility.o filetable.o directoryparse.o ls.o parseinput.o size.o program.o read.o cd.o openclose.o write.o
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
program.o:	program.c program.h
		gcc $(FLAGS) -c program.c
read.o:		read.c read.h
		gcc $(FLAGS) -c read.c
cd.o:		cd.c cd.h
		gcc $(FLAGS) -c cd.c
openclose.o:	openclose.c openclose.h
		gcc $(FLAGS) -c openclose.c
rootparse.o:    rootparse.c rootparse.h
		gcc $(FLAGS) -c rootparse.c
write.o:	write.c write.h
		gcc $(FLAGS) -c write.c
clean:
		rm *.o prog.x
