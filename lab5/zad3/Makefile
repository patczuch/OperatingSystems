SHELL=/bin/bash
CFLAGS= -Wall $(O)

all:
	gcc $(CFLAGS) main.c -o main
	gcc $(CFLAGS) child.c -o child
	
clean:
	rm -f main
	rm -f child

test:
	make all
	./main 0.000000001 10
	make clean