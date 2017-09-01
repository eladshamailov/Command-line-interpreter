all: myShell

myShell: myShell.o LineParser.o
	gcc -m32 -g -Wall -o myShell myShell.o LineParser.o
        
myShell.o: myShell.c 
	gcc -g -Wall -m32 -ansi -c -o myShell.o myShell.c 

LineParser.o :LineParser.h LineParser.c
	gcc -g -Wall -m32 -ansi -c -o LineParser.o LineParser.c


PHONY: clean

clean: 
	rm -f *.o myShell
