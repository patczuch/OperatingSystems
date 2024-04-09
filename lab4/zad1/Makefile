SHELL=/bin/bash
CFLAGS= -Wall $(O)
all:
	gcc $(CFLAGS) main_fork.c -o main_fork
	gcc $(CFLAGS) main_exec.c -o main_exec

fork:
	gcc $(CFLAGS) main_fork.c -o main
	
exec:
	gcc $(CFLAGS) main_exec.c -o main
	
clean:
	rm -f main
	rm -f main_fork
	rm -f main_exec

test:
	make test2 -i
	
test2:
	@make fork
	@echo FORK
	@echo
	@echo IGNORE
	@./main ignore
	@echo
	@echo HANDLER
	@./main handler
	@echo
	@echo MASK
	@./main mask
	@echo
	@echo PENDING
	@./main pending
	@echo
	@make exec
	@echo EXEC
	@echo
	@echo IGNORE
	@./main ignore
	@echo
	@echo HANDLER
	@./main handler
	@echo
	@echo MASK
	@./main mask
	@echo
	@echo PENDING
	@./main pending
	@echo
	@make clean
