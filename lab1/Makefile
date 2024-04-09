CFLAGS= -Wall $(O)

lib_static:
	gcc $(CFLAGS) -c lib1.c
	ar rc liblib1.a lib1.o
	
lib_shared:
	gcc $(CFLAGS) -fPIC -c lib1.c
	gcc $(CFLAGS) -shared lib1.o -o liblib1.so

static:
	make lib_static
	gcc $(CFLAGS) -c main.c
	gcc $(CFLAGS) main.o liblib1.a -o main_static

shared:
	make lib_shared
	gcc $(CFLAGS) -c main.c
	gcc $(CFLAGS) main.o -L. -llib1 -Wl,-rpath=. -o main_shared
	
dynamic:
	gcc $(CFLAGS) -fPIC -c lib1.c
	gcc $(CFLAGS) -shared lib1.o -o liblib1.so
	gcc $(CFLAGS) main.c -o main_dynamic -ldl -DDYNAMIC

all:
	make static
	make shared
	make dynamic

clean:
	rm -f *.o
	rm -f *.so
	rm -f *.a
	
test2:
	make all
	file_count=`ls -p | grep -v / | wc -l`; \
	command="init 4\n"; \
	command+="count main.c\ncount lib1.c\ncount lib1.h\ncount Makefile\n"; \
	for (( i=0; i<4; i++ )); do command+="show "; command+=$$i; command+="\n"; done; \
	for (( i=0; i<4; i++ )); do command+="delete 0\n"; done; \
	#command+=$$file_count; \
	#command+="\n"; \
	#for f in ./*; do if [ -f $$f ]; then command+="count "; command+=$$f; command+="\n"; fi; done; \
	#for (( i=0; i<$$file_count; i++ )); do command+="show "; command+=$$i; command+="\n"; done; \
	#for (( i=0; i<$$file_count; i++ )); do command+="delete 0\n"; done; \
	command+="destroy\nexit"; \
	./main_static < <(echo -e $$command) > ./results_static$(O).txt; \
	./main_shared < <(echo -e $$command) > ./results_shared$(O).txt; \
	./main_dynamic < <(echo -e $$command) > ./results_dynamic$(O).txt
	
test:
	make test2 O=-O0
	make test2 O=-O2
	make test2 O=-Os