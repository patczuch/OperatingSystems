SHELL=/bin/bash
CFLAGS= -Wall $(O)
all:
	gcc $(CFLAGS) main.c -o main
	
clean:
	rm -f main

test:
	make test2 -i

test2:
	make all
	./main
	make clean