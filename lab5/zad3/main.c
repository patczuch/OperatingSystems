#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_ARG_LENGTH 64

//ta calka wynosi dokladnie pi = 3.14159265359

double f(double x)
{
	return 4 / (x*x+1);	
}

int main(int argc, char **argv)
{	
	if (argc != 3)
	{
		printf("Wrong number of arguments!\n");
		return 0;
	}
	
	double square_width = strtod(argv[1], NULL);
	if (square_width <= 0)
	{
		printf("Square width must be a positive number!\n");
		return 0;
	}
	
	int n = atoi(argv[2]);
	if (n <= 0)
	{
		printf("Number of processes must be a positive number!\n");
		return 0;
	}
	
	struct timeval time_start, time_end;
    gettimeofday(&time_start, NULL);
	
	double a = 0;
	double b = 1;
	double process_width = (b-a)/n;
	
	mkfifo("pipe", 0700);
	FILE * fd = fopen("pipe", "ab+");
	
	setbuf(stdout, NULL);
	
	double x = a;
	for (int i = 0; i < n; i++)
	{
		char arg1[MAX_ARG_LENGTH];
		sprintf(arg1, "%.15lf", x);
		char arg2[MAX_ARG_LENGTH];
		sprintf(arg2, "%.15lf", x+process_width);
		if (fork() == 0)
		{
			execl("./child","./child",argv[1],arg1,arg2,NULL);
			return 0;
		}
		x += process_width;
	}
	
	while (wait(NULL) > 0);
	
	//int fd = open(fifo, O_RDONLY);
	double c;
	double res = 0;
	for (int i = 0; i < n; i++)
	{
		fread(&c, sizeof(double), 1, fd);
		//printf("parent: %lf\n",c);
		res += c;
	}
	
	fclose(fd);
	
	system("rm pipe");
	
	gettimeofday(&time_end, NULL);

	printf("Number of processes: %d, Square width: %.15lf\n", n, square_width);
	printf("Result: %.15lf\n",res);
    double elapsedTime = (time_end.tv_sec - time_start.tv_sec) * 1000.0 + (time_end.tv_usec - time_start.tv_usec) / 1000.0;
    printf("Time taken: %.2fms\n", elapsedTime);
	
	
	return 0;
}