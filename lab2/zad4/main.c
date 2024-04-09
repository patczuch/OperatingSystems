#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <ftw.h>

#define MAX_PATH_LENGTH 1024

long long total_size = 0;

int for_each_file(const char *filename, const struct stat *file_info, int flag)
{
	if (S_ISDIR(file_info->st_mode))
	{
		//printf("%s: Is a directory\n", filename);
		printf("%lld %s\n", 0LL, filename);
		return 0;	
	}
	printf("%lld %s\n", (long long)file_info->st_size, filename);
	total_size += (long long)file_info->st_size;
	return 0;
}

int main(int argc, char **argv)
{	
	if (argc != 2)
	{
		printf("Wrong number of arguments!\n");
		return 0;
	}
	
	ftw(argv[1], for_each_file, FOPEN_MAX);
	
	printf("%lld total\n", (long long)total_size);
	  
	return 0;
}