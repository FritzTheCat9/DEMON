main: main.o file.o help.o demon.o log.o directory.o
	gcc main.c file.c help.c demon.c log.c directory.c -o main