#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 256
#define MAX_LINES 64

int strcmp2(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

int main(int argc, char **argv)
{	
    if (argc != 2 && argc != 4)
	{
		printf("Wrong number of arguments!\n");
		return 0;
	}

    if (argc == 2)
    {
        if (strcmp(argv[1],"nadawca") != 0 && strcmp(argv[1],"data") != 0)
        {
            printf("Wrong argument!\n");
            return 0;
        }
    	FILE * fd = popen("exec bash -c 'mail -f <<< \"echo q\"'","r");
        char buffer [MAX_LINES][BUFFER_SIZE];
        int i = 0;
        while (fgets( buffer[i], BUFFER_SIZE, fd))
        {
            buffer[i][0] = ' ';
            buffer[i][4] = ' ';
            i++;
            if (i >= MAX_LINES)
                break;
        }
        pclose(fd);

        if (strcmp(argv[1],"nadawca") == 0)
        {
            qsort(buffer, i, BUFFER_SIZE, strcmp2);
            for(int j = 1; j < i-2; j++)
                printf("%s", buffer[j]);
        }
        else
            for(int j = 2; j < i-1; j++)
                printf("%s", buffer[j]);
    }

    else if (argc == 4)
    {
        char buffer [BUFFER_SIZE];
        sprintf(buffer, "mail -s %s %s",argv[2],argv[1]);
        FILE * fd = popen(buffer,"w");
        fwrite(argv[3],strlen(argv[3]),1,fd);
        pclose(fd);
    }
	return 0;
}