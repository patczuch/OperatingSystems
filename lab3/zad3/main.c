#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>
#include<sys/wait.h>

#define MAX_SEARCH_LENGTH 255

void search_dir(char start_path[PATH_MAX], char search_phrase[MAX_SEARCH_LENGTH]);

int main(int argc, char **argv)
{	
	if (argc != 3)
	{
		printf("Wrong number of arguments!\n");
		return 0;
	}
	
	if (strlen(argv[1]) > PATH_MAX)
	{
		printf("Path too long!\n");
		return 0;
	}
		
	if (strlen(argv[2]) > MAX_SEARCH_LENGTH)
	{
		printf("Search phrase too long!\n");
		return 0;
	}
	
	setbuf(stdout, NULL);
	
	search_dir(argv[1],argv[2]);
	
	while (wait(NULL) > 0);
	
	return 0;
}
		
void search_dir(char start_path[PATH_MAX], char search_phrase[MAX_SEARCH_LENGTH])
{
	DIR* dir;
	char path[PATH_MAX];
	struct dirent *file;
	struct stat file_info;
	
	if ((dir = opendir(start_path)) == NULL)
	{
    	perror("Error opening directory!\n");
		return;
	}
  	else
    	while ((file = readdir(dir)) != NULL)
		{
			strcpy(path, start_path);
			char * slash = "/";
			strcat(path, slash);
        	strcat(path, file->d_name);
				
			if (stat(path, &file_info) != 0)
			{	
				perror("Error reading path!\n");
				return;	
			}
			
			if (!S_ISDIR(file_info.st_mode) || file->d_name[0] != '.')
			{
				if (S_ISDIR(file_info.st_mode))
				{
					pid_t id = fork();
					if (id == 0)
					{
						if (strlen(path) > PATH_MAX)
						{
							perror("Path too long!\n");
							return;
						}
						//printf("new process: %s\n", path);
						search_dir(path,search_phrase);
						return;
					}
					else if (id == -1)
					{
						perror("Couldn't create process!\n");
						return;
					}
				}
				else
				{
					FILE *f = fopen(path, "r");
					if (f == NULL)
					{
						perror("Error opening file!\n");
						return;
					}
					char c;
					int i = 0;
				    int flag = 0;
					while ((c = fgetc(f)) != EOF)
					{
						//printf("%c %c\n",search_phrase[i],c);
						if (c != search_phrase[i])
							break;
						i++;
						//printf("%d\n",strlen(search_phrase));
						if (i >= strlen(search_phrase))
						{
							flag = 1;
							break;
						}
					}
					if (flag) 
						printf("%s %d\n",path,(int)getpid());			
					
					fclose(f);
				}
			}
		}
    closedir(dir);			
}