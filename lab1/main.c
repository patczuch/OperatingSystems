#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/times.h>
#include <inttypes.h>
#include <time.h>

#include "lib1.h"

#ifdef DYNAMIC
#include <dlfcn.h>
#endif

#define LINE_BUFFER_SIZE 256

intmax_t time_diff(clock_t start, clock_t end){
    return ((intmax_t)(end - start));
}

int main()
{
	#ifdef DYNAMIC
	void *lib_handle = dlopen("./liblib1.so", RTLD_LAZY);
    if(!lib_handle)
	{
		printf("Error opening library!\n");
    	exit(0);
	}
	
    struct CharPointerArray* (*cpa_create)() = (struct CharPointerArray* (*)())dlsym(lib_handle,"cpa_create");
	void (*cpa_load_file_info)() = (void (*)())dlsym(lib_handle, "cpa_load_file_info");
	char* (*cpa_get_block)() = (char* (*)())dlsym(lib_handle, "cpa_get_block");
	void (*cpa_delete_block)() = (void (*)())dlsym(lib_handle, "cpa_delete_block");
	void (*cpa_free)() = (void (*)())dlsym(lib_handle, "cpa_free");
	
    if(dlerror() != NULL)
	{
		printf("%s\n", dlerror());
    	exit(0);
	}

	#endif
	
	printf("Available commands - init <size>, count <file>, show <index>, delete <index>, destroy, exit\n");	
	
	CharPointerArray* cpa = NULL;
	char* line = malloc(sizeof(char)*LINE_BUFFER_SIZE);
	//size_t len = LINE_BUFFER_SIZE;
	char* word1, * word2;
	struct timespec start_time, end_time;
	struct tms start_cpu, end_cpu;
	
	while (1)
	{
		//printf("> ");
    	fgets( line, LINE_BUFFER_SIZE, stdin );
		//printf(line);
		//if (getline(&line, &len, stdin) == -1) 
		//	break;
		
		times(&start_cpu);
        clock_gettime(CLOCK_REALTIME, &start_time);
		
		word1 = strtok(line," ");
		word2 = strtok(NULL," ");
		
		if (word1 != NULL && word2 == NULL)
			word1[strlen(word1)-1] = '\0';
		else if (word2 != NULL)
			word2[strlen(word2)-1] = '\0';
		
    	if (strcmp(word1,"init") == 0)
		{
			if (cpa != NULL)
				cpa_free(cpa);
			cpa = cpa_create(atoi(word2));
		}
		else if (strcmp(word1,"count") == 0)
			if (cpa == NULL)
				printf("CPA not initialized!\n");
			else if (access(word2, F_OK) != 0)
				printf("File doesn't exist!\n");
			else
				cpa_load_file_info(cpa, word2);
		else if (strcmp(word1,"show") == 0)
			if (cpa == NULL)
				printf("CPA not initialized!\n");
			else if (atoi(word2) < 0 || cpa->current_size <= atoi(word2))
				printf("Index doesn't exist!\n");
			else
				printf("%s\n",cpa_get_block(cpa,atoi(word2)));
		else if (strcmp(word1,"delete") == 0)
			if (cpa == NULL)
				printf("CPA not initialized!\n");
			else if (atoi(word2) < 0 || cpa->current_size <= atoi(word2))
				printf("Index doesn't exist!\n");
			else
				cpa_delete_block(cpa,atoi(word2));
		else if (strcmp(word1,"destroy") == 0)
			if (cpa == NULL)
				printf("CPA not initialized!\n");
			else
			{
				cpa_free(cpa);
				cpa = NULL;
			}
		else if (strcmp(word1,"exit") == 0)
			break;
		else
			printf("%s: Command not found\n", line);	
		
		times(&end_cpu);
        clock_gettime(CLOCK_REALTIME, &end_time);
		
		printf("Real time: %ldns, User time: %jd ticks, System time: %jd ticks\n", end_time.tv_nsec-start_time.tv_nsec, time_diff(start_cpu.tms_utime, end_cpu.tms_utime), time_diff(start_cpu.tms_stime, end_cpu.tms_stime));
		//printf("%jd %jd %jd %jd\n", (intmax_t)start_cpu.tms_utime, (intmax_t)end_cpu.tms_utime, (intmax_t)start_cpu.tms_stime, (intmax_t)end_cpu.tms_stime);
	}
	
	#ifdef DYNAMIC
	dlclose(lib_handle);
	#endif
	
	return 0;
}