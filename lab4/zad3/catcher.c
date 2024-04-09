#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define SIGNAL SIGUSR1

int stop = 1;
int received_signals = 0;
int _print_time = 0;

void print_time()
{
	time_t mytime = time(NULL);
	char * time_str = ctime(&mytime);
	time_str[strlen(time_str)-1] = '\0';
	printf("%s\n", time_str);
}

void handler(int sig, siginfo_t *siginfo, void *ucontext)
{
	_print_time = 0;
	int mode = siginfo->si_value.sival_int;
	printf("Sender PID: %d, mode: %d\n", siginfo->si_pid, mode);
	printf("Sending confirmation\n");
	kill(siginfo->si_pid, SIGNAL);
	received_signals++;
	switch(mode)
	{
		case 1:
			printf("Printing numbers 1-100\n");
			for (int i = 1; i <= 100; i++)
				printf("%d ",i);
			printf("\n");
			break;
		case 2:
			printf("Printing current time\n");
			print_time();
			break;
		case 3:
			printf("Printing current time each second\n");
			print_time();
			_print_time = 1;
			return;
		case 4:
			printf("Printing number of received signals\n");
			printf("%d\n",received_signals);
			break;
		case 5:
			printf("Stopping\n");
			stop = 0;
			return;
		default:
			printf("Unknown mode!\n");
			break;
	}
	printf("Waiting for a signal...\n");	
}

int main(int argc, char **argv)
{	
	printf("PID: %d\n", (int) getpid());
	printf("Waiting for a signal...\n");	
	
	struct sigaction* act = malloc(sizeof(struct sigaction));
	sigemptyset(&act->sa_mask);
	act->sa_sigaction = handler;
	act->sa_flags = SA_SIGINFO;
	
	sigaction(SIGNAL, act, NULL);
	
	while (stop)
		if (_print_time)
		{
			sleep(1);
			if (_print_time)
				print_time();
		}
	
	return 0;
}