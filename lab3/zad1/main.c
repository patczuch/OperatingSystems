#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{	
	if (argc != 2)
	{
		printf("Wrong number of arguments!\n");
		return 0;
	}
	
	int n = atoi(argv[1]);
	
	if (n < 1)
	{
		printf("First argument invalid!\n");
		return 0;
	}
	
	pid_t * child_pids = malloc(sizeof(pid_t) * n);
	
	for (int i = 0; i<n; i++)
		if ((child_pids[i] = fork()) == 0)
		{
			printf("id procesu macierzystego: %d id tego procesu: %d\n", (int)getppid(), (int)getpid());
			return 0;
		}
	
	for (int i = 0; i<n; i++)
		waitpid(child_pids[i], NULL, 0);
	
	free(child_pids);
	
	printf("argv[1]: %s\n", argv[1]);
	
	
	return 0;
}