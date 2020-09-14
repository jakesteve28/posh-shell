# this is a comment
CC=gcc

all: posh

posh:	cscd340Lab7.c ./hashmap/hashmap.o ./tokenize/makeArgs.o ./process/process.o ./pipes/pipes.o
	gcc -g cscd340Lab7.c ./tokenize/makeArgs.o ./process/process.o ./pipes/pipes.o -o posh

makeArgs.o:	./tokenize/makeArgs.c ./tokenize/makeArgs.h
	gcc -g -c ./tokenize/makeArgs.c

process.o:	./process/process.c ./process/process.h
	gcc -g -c ./process/process.c	

pipes.o:	./pipes/pipes.c ./pipes/pipes.h
	gcc -g -c ./pipes/pipes.c
	
listUtils.o:	./linkedlist/listUtils.c listUtils.h
	gcc  -g -c ./linkedlist/listUtils.c

linkedList.o:	./linkedlist/linkedList.h ./linkedlist/linkedList.c ./linkedlist/requiredIncludes.h
	gcc -g -c ./linkedlist/linkedList.c

hashmap.o:	./hashmap/hashmap.h ./hashmap/hashmap.c ./hashmap/hashmap.h
	gcc -g -c ./hashmap/hashmap.c

clean:
	rm ./pipes/pipes.o	
	rm ./hashmap/hashmap.c
	rm ./process/process.o
	rm ./tokenize/makeArgs.o
	rm posh

