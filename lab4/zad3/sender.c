#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define SIGNAL SIGUSR1

int stop;

void handler(int sig)
{
	printf("Confirmation received\n");
	stop = 0;
}

int main(int argc, char **argv)
{	
	if (argc < 3){
		printf("Wrong number of arguments!\n");
		return 0;
	}
	
	union sigval to_send;
	int pid = atoi(argv[1]);
	
	signal(SIGNAL,handler);
	
	for (int i = 2; i < argc; i++)
	{
		stop = 1;
		to_send.sival_int = atoi(argv[i]);
		printf("Sending mode %d\n",to_send.sival_int);
		sigqueue(pid, SIGNAL, to_send);
		printf("Waiting for confirmation...\n");
		while (stop);
	}
	
	return 0;
}