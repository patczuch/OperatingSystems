#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void handler_siginfo(int sig, siginfo_t *siginfo, void *ucontext)
{
   	switch (sig)
   	{
		case SIGUSR1:
		   printf("Received SIGUSR1 signal, ");
		   break;
		case SIGUSR2:
		   printf("Received SIGUSR2 signal, ");
		   break;
		case SIGSEGV:
		   printf("Received SIGSEGV signal, ");
		   break;
	    default:
		   break;
		   
   	}
   	printf("signal ID: %d, sender PID: %d\n", sig, siginfo->si_pid);
}

void handler_nodefer(int sig, siginfo_t *siginfo, void *ucontext)
{
	sigset_t* old_set = malloc(sizeof(sigset_t));
	sigprocmask(SIG_BLOCK, NULL, old_set);
	
	if(sigismember(old_set, SIGUSR1))
		printf("Signal in signal mask\n");
	else
		printf("Signal not in signal mask\n");	
}

void handler_resethand(int sig, siginfo_t *siginfo, void *ucontext)
{
	printf("Signal handled\n");	
}

int main(int argc, char **argv)
{	
    struct sigaction* act = malloc(sizeof(struct sigaction));
	sigemptyset(&act->sa_mask);
	act->sa_sigaction = handler_siginfo;
	act->sa_flags = SA_SIGINFO;
	
    sigaction(SIGUSR1, act, NULL);
	sigaction(SIGUSR2, act, NULL);
	sigaction(SIGSEGV, act, NULL);
	
	printf("SA_SIGINFO test\n");
	
	raise(SIGUSR1);
	raise(SIGUSR2);
	raise(SIGSEGV);
	
	printf("\n");
	printf("SA_NODEFER test\n"); //deletes signal from the signal mask inside the handler
	
	struct sigaction* act2 = malloc(sizeof(struct sigaction));
	sigemptyset(&act2->sa_mask);
	act2->sa_sigaction = handler_nodefer;
    sigaction(SIGUSR1, act2, NULL);
	
	printf("Without SA_NODEFER:\n");
	raise(SIGUSR1);
	
	act2->sa_flags = SA_NODEFER;
    sigaction(SIGUSR1, act2, NULL);
	
	printf("With SA_NODEFER:\n");
	raise(SIGUSR1);
	
	printf("\n");
	
	printf("SA_RESETHAND test\n"); //deletes handle from signal after one use
	
	struct sigaction* act3 = malloc(sizeof(struct sigaction));
	sigemptyset(&act3->sa_mask);
	act3->sa_sigaction = handler_resethand;
	act3->sa_flags = SA_RESETHAND;
    sigaction(SIGUSR1, act3, NULL);
	
	raise(SIGUSR1);
	raise(SIGUSR1);
	
	printf("\n");
	
	return 0;
}