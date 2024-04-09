SHELL=/bin/bash
CFLAGS= -Wall $(O)

all:
	gcc $(CFLAGS) sender.c -o sender
	gcc $(CFLAGS) catcher.c -o catcher
	
clean:
	rm -f sender
	rm -f catcher
