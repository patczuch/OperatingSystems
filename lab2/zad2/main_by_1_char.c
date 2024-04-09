#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{	
	struct timespec start_time, end_time;
	clock_gettime(CLOCK_REALTIME, &start_time);
	
	if (argc != 3)
	{
		printf("Insufficient arguments!\n");
		return 0;
	}
	
	FILE* fd_in = fopen(argv[1], "rb+");
	if (fd_in == NULL)
    {
        printf("Error opening input file!\n");
        return 0;
    }

	FILE *fd_out = fopen( argv[2] , "w" );
	if (fd_out == NULL)
    {
        printf("Error opening output file!\n");
        return 0;
    }
	
	fseek(fd_in, 0L, SEEK_END);
	char c;
	while (ftell(fd_in) > 0)
	{
		fseek(fd_in, -1L, SEEK_CUR);
		c = fgetc(fd_in);
		fwrite(&c, sizeof(char) , 1, fd_out );
		fseek(fd_in, -1L, SEEK_CUR);
	}
	
	
    fclose(fd_in);
    fclose(fd_out);
	
	clock_gettime(CLOCK_REALTIME, &end_time);
	printf("Time taken: %ldns\n", end_time.tv_nsec-start_time.tv_nsec);
	
	return 0;
}