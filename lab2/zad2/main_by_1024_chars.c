#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BUFFER_SIZE 1024

void reverse(char * array, int length)
{
	char tmp;
	for(int j = 0;j<length/2; j++)
	{
		tmp = array[j];
		array[j] = array[length-j-1];
		array[length-j-1] = tmp;
    }	
}

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
	char buffer[BUFFER_SIZE];
	
	long size = ftell(fd_in);
	
	for (long i = 0; i < size / BUFFER_SIZE; i++)
	{
		fseek(fd_in, -BUFFER_SIZE, SEEK_CUR);
		
		fread(buffer, BUFFER_SIZE * sizeof(char), 1, fd_in );
		
		reverse(buffer, BUFFER_SIZE);
		
		fwrite(buffer, BUFFER_SIZE * sizeof(char), 1, fd_out );
		
		fseek(fd_in, -BUFFER_SIZE, SEEK_CUR);
	}
	
	long last_block_size = (size - ((size / BUFFER_SIZE) * BUFFER_SIZE));
	
	fseek(fd_in, 0, SEEK_SET);
	
	fread(buffer, last_block_size * sizeof(char), 1, fd_in );
	
	reverse(buffer, last_block_size);
	
	fwrite(buffer, last_block_size * sizeof(char), 1, fd_out );
	
    fclose(fd_in);
    fclose(fd_out);
	
	clock_gettime(CLOCK_REALTIME, &end_time);
	printf("Time taken: %ldns\n", end_time.tv_nsec-start_time.tv_nsec);
	
	return 0;
}