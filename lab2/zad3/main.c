#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#define MAX_PATH_LENGTH 1024

int main(int argc, char **argv)
{	
	DIR* dir;
	char path[MAX_PATH_LENGTH];
	char* current_path = "./";
	
	struct dirent *file;
	struct stat file_info;
	
	long long total_size = 0;
	
	if ((dir = opendir(current_path)) == NULL)
	{
    	printf("Error opening directory!\n");
		return 0;
	}
  	else
    	while ((file = readdir(dir)) != NULL)
			if (file->d_name[0] != '.')
			{
				strcpy(path, current_path);
        		strcat(path, file->d_name);
				
				if (stat(path, &file_info) != 0)
				{	
					printf("Error reading path: %s\n",path);
					return 0;	
				}
				
				if (!S_ISDIR(file_info.st_mode))
				{
					total_size += file_info.st_size;

					printf("%lld %s\n", (long long)file_info.st_size, file->d_name);
				}
			}
    closedir(dir);
	
	printf("%lld total\n", (long long)total_size);
	  
	return 0;
}