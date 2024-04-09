#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>

//ta calka wynosi dokladnie pi = 3.14159265359

double f(double x)
{
	return 4 / (x*x+1);	
}

int main(int argc, char **argv)
{	
	if (argc != 4)
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
	//printf("%s %s %s %s\n",argv[0],argv[1],argv[2],argv[3]);
	
	double a = strtod(argv[2], NULL);
	double b = strtod(argv[3], NULL);
	
	double x = a;
	double res = 0;
	
	
	while (x < b - square_width)
	{
		res += f(x) * square_width;
		x += square_width;
		//printf("%lf, %lf\n",x,b-square_width);
	}
	
	
	res += f(x) * (b - x);
	
	//int fd = open("pipe",O_WRONLY);
	FILE * fd = fopen("pipe", "ab+");
    fwrite(&res, sizeof(double),1,fd);
    fclose(fd);
	
	//printf("child: %lf\n",res);
	
	return 0;
}