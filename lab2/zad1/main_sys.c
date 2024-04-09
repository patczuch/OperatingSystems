#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char **argv)
{	
	struct timespec start_time, end_time;
	clock_gettime(CLOCK_REALTIME, &start_time);
	
	if (argc != 5)
	{
		printf("Insufficient arguments!\n");
		return 0;
	}
	
	if (strlen(argv[1]) != 1)
	{
		printf("First argument should be one character long!\n");
		return 0;
	}	
	
	if (strlen(argv[2]) != 1)
	{
		printf("Second argument should be one character long!\n");
		return 0;
	}	
	
	int fd_in = open(argv[3], O_RDONLY);
	if (fd_in == -1)
    {
        printf("Error opening input file!\n");
        return 0;
    }

	int fd_out = open(argv[4], O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
	if (fd_out == -1)
    {
        printf("Error opening output file!\n");
        return 0;
    }
	
	char c;
  	while(read(fd_in,&c,1)==1)
		if (c == *argv[1])
			write(fd_out,&argv[2][0],1);
		else
        	write(fd_out,&c,1);
	
	close(fd_in);
	close(fd_out);
	
	clock_gettime(CLOCK_REALTIME, &end_time);
	printf("Time taken: %ldns\n", end_time.tv_nsec-start_time.tv_nsec);
		   
	return 0;
}