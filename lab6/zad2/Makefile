SHELL=/bin/bash
CFLAGS= -Wall $(O)

all:
	gcc $(CFLAGS) server.c -o server -lc -lpthread
	gcc $(CFLAGS) client.c -o client
	
clean:
	rm -f server
	rm -f client

test_server:
	./server 3277

test_client:
	./client 127.0.0.1 3277
