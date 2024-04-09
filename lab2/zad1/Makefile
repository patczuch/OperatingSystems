SHELL=/bin/bash
CFLAGS= -Wall $(O)

lib:
	gcc $(CFLAGS) main_lib.c -o zamien 

sys:
	gcc $(CFLAGS) main_sys.c -o zamien
	
all:
	gcc $(CFLAGS) main_lib.c -o zamien_lib
	gcc $(CFLAGS) main_sys.c -o zamien_sys

test:
	echo "Pomiar czasu przy użyciu funkcji bibliotecznych:" > pomiar_zad_1.txt
	make lib
	./zamien >> pomiar_zad_1.txt a d test.txt test_out_lib.txt ; tr a d < test.txt > tmp ; diff -s tmp test_out_lib.txt
	rm tmp
	echo "Pomiar czasu przy użyciu funkcji systemowych:" >> pomiar_zad_1.txt
	make sys
	./zamien >> pomiar_zad_1.txt a d test.txt test_out_sys.txt ; tr a d < test.txt > tmp ; diff -s tmp test_out_sys.txt
	rm tmp
	make clean
clean:
	rm -f zamien_lib
	rm -f zamien_sys
	rm -f zamien