all: myshell


myshell: Job.o LineParser.o JobControl.o
	cc -o myshell Job.o LineParser.o JobControl.o

Job.o: Job.c 
	gcc -c -o Job.o Job.c

LineParser.o: LineParser.c 
	gcc -c -o LineParser.o LineParser.c

JobControl.o: JobControl.c 
	gcc -c -o JobControl.o JobControl.c
.PHONY:clean
clean: 
	rm -f *.o myshell
