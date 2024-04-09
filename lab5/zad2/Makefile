SHELL=/bin/bash
CFLAGS= -Wall $(O)

all:
	gcc $(CFLAGS) main.c -o main
	
clean:
	rm -f main

test:
	make all
	./main 0.0000000001 100
	make clean