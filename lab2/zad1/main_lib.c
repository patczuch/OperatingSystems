#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
	
	FILE* fd_in = fopen(argv[3], "r");
	if (fd_in == NULL)
    {
        printf("Error opening input file!\n");
        return 0;
    }

	FILE *fd_out = fopen( argv[4] , "w" );
	if (fd_out == NULL)
    {
        printf("Error opening output file!\n");
        return 0;
    }
	
	char c;
	while ((c = fgetc(fd_in)) != EOF)
        if (c == argv[1][0])
			fwrite(&argv[2][0], sizeof(char) , 1, fd_out );
		else
			fwrite(&c, sizeof(char) , 1, fd_out );
	
    fclose(fd_in);
    fclose(fd_out);
	
	clock_gettime(CLOCK_REALTIME, &end_time);
	printf("Time taken: %ldns\n", end_time.tv_nsec-start_time.tv_nsec);
	
	return 0;
}