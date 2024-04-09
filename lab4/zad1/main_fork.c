#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define SIGNAL SIGUSR1

void handler(int signo)
{
	printf("Received the signal!\n");	
}

void pending()
{
	sigset_t* pendingSet = malloc(sizeof(sigset_t));
	sigpending(pendingSet);
	if(sigismember(pendingSet, SIGUSR1))
		printf("Signal pending\n");
	else
		printf("Signal not pending\n");	
}

int main(int argc, char **argv)
{	
	if (argc != 2){
		printf("Wrong number of arguments!\n");
		return 0;
	}
	
	char * command = argv[1];
	
	int mode = -1;
	if (!strcmp(command,"ignore"))
	{
		signal(SIGNAL, SIG_IGN);
		mode = 0;	
	}
	else if (!strcmp(command,"handler"))
	{
		signal(SIGNAL, handler);
		mode = 1;	
	}
	else if (!strcmp(command,"mask"))
	{
		sigset_t* new_set = malloc(sizeof(sigset_t));
		sigemptyset(new_set);
        sigaddset(new_set, SIGNAL);
		sigprocmask(SIG_SETMASK, new_set, NULL);
		mode = 2;
	}
	else if (!strcmp(command,"pending"))
	{
		sigset_t* new_set = malloc(sizeof(sigset_t));
		sigemptyset(new_set);
        sigaddset(new_set, SIGNAL);
		sigprocmask(SIG_SETMASK, new_set, NULL);
		mode = 3;
	}
	else
	{
		printf("Option not found!\n");
		return 0;
	}
	
	printf("Parent:\n");
	raise(SIGNAL);
	
	if (mode == 2 || mode == 3)
		pending();
	
	printf("Child:\n");
	
	setbuf(stdout, NULL);
	if (fork() == 0)
	{
		if (mode != 3)
			raise(SIGNAL);	
		if (mode == 2 || mode == 3)
			pending();	
		return 0;
	}

	return 0;
}