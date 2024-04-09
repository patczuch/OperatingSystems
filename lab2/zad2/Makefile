SHELL=/bin/bash
CFLAGS= -Wall $(O)

by_1_char:
	gcc $(CFLAGS) main_by_1_char.c -o reverse 

by_1024_chars:
	gcc $(CFLAGS) main_by_1024_chars.c -o reverse
	
all:
	gcc $(CFLAGS) main_by_1_char.c -o reverse_by_1_char
	gcc $(CFLAGS) main_by_1024_chars.c -o reverse_by_1024_chars
	
test_by_1_char:
	make by_1_char
	./reverse test.txt tmp1 
	./reverse tmp1 tmp2
	diff -s tmp2 test.txt
	rm tmp1
	rm tmp2
	
test_by_1024_chars:
	make by_1024_chars
	./reverse test.txt tmp1 
	./reverse tmp1 tmp2
	diff -s tmp2 test.txt
	rm tmp1
	rm tmp2

test:
	echo "Pomiar czasu przy kopiowaniu po 1 znaku:" > pomiar_zad_2.txt
	make by_1_char
	./reverse test.txt tmp1 >> pomiar_zad_2.txt
	./reverse tmp1 tmp2
	diff -s tmp2 test.txt
	rm tmp1
	rm tmp2
	
	echo "Pomiar czasu przy kopiowaniu po 1024 znakach:" >> pomiar_zad_2.txt
	make by_1024_chars
	./reverse test.txt tmp1 >> pomiar_zad_2.txt
	./reverse tmp1 tmp2
	diff -s tmp2 test.txt
	rm tmp1
	rm tmp2
	make clean
clean:
	rm -f reverse_by_1_char
	rm -f reverse_by_1024_chars
	rm -f reverse