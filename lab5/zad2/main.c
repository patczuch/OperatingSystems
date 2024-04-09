#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

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
	
	//double square_width = 0.000000001;
	double a = 0;
	double b = 1;
	//int n = 1000;
	//double square_width = 0.0000001;
	double process_width = (b-a)/n;
	
	
	int** fd = (int**)malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++)
		fd[i] = (int*)malloc(2 * sizeof(int));
	
	double x = a;
	pid_t pid;
	for (int i = 0; i<n; i++)
	{
		pipe(fd[i]);
		pid = fork();
		if (pid == 0) 
		{
			close(fd[i][0]);
			
			double c = 0;
			double x1 = x;
			while (x1 < x + process_width - square_width)
			{
				c += f(x1) * square_width;
				x1 += square_width;	
			}
			c+= f(x1) * (x + process_width - x1);
			
			write(fd[i][1], &c, sizeof(double));
			return 0;
		} 
		close(fd[i][1]);
		x += process_width;	
	}
	
	while (wait(NULL) > 0);
	
	double res = 0;
	double c;
	
	for (int i = 0; i<n; i++)
	{
		read(fd[i][0], &c, sizeof(double));
		res+=c;
		free(fd[i]);
	}
	
	free(fd);
	
	gettimeofday(&time_end, NULL);

	printf("Number of processes: %d, Square width: %.15lf\n", n, square_width);
	printf("Result: %.15lf\n",res);
    double elapsedTime = (time_end.tv_sec - time_start.tv_sec) * 1000.0 + (time_end.tv_usec - time_start.tv_usec) / 1000.0;
    printf("Time taken: %.2fms\n", elapsedTime);
	
	
	return 0;
}