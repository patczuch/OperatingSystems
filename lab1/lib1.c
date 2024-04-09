#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib1.h"

#define COMMAND_BUFFER_SIZE 256

CharPointerArray* cpa_create(int max_size)
{
	CharPointerArray* cpa = malloc(sizeof(CharPointerArray));
	cpa->array = calloc(max_size, sizeof(char*));
	cpa->max_size = max_size;
	cpa->current_size = 0;
	return cpa;
}

void cpa_load_file_info(CharPointerArray* cpa, char* filepath)
{	
	if(cpa->current_size >= cpa->max_size)
		return;
	
	char command [COMMAND_BUFFER_SIZE];
	char tmp_filepath[] = "/tmp/tmp-XXXXXX";
	mkstemp(tmp_filepath);
	
	snprintf(command,COMMAND_BUFFER_SIZE,"wc %s > %s", filepath, tmp_filepath);
	system(command);
	
	FILE* tmp_fd = fopen(tmp_filepath, "r");
	
	fseek(tmp_fd, 0L, SEEK_END);
	long size = ftell(tmp_fd);
	//printf("%ld", size);
	
	rewind(tmp_fd);
	
	cpa->array[cpa->current_size] = malloc(size);
	int i = fread(cpa->array[cpa->current_size], sizeof(char), size, tmp_fd);
	cpa->array[cpa->current_size][i-1] = '\0';
    fclose(tmp_fd);
	
	//printf("%s", cpa->array[cpa->current_size]);
	
	snprintf(command,COMMAND_BUFFER_SIZE, "rm %s", tmp_filepath);
	system(command);
	
	cpa->current_size++;
}

char* cpa_get_block(CharPointerArray* cpa, int index)
{
	return cpa->array[index];
}

void cpa_delete_block(CharPointerArray* cpa, int index)
{
	free(cpa->array[index]);
	if (index < cpa->current_size - 1)
		memmove(&cpa->array[index],&cpa->array[index+1],sizeof(char*)*(cpa->current_size - index - 1));
	cpa->current_size--;
}

void cpa_free(CharPointerArray* cpa)
{
	for (int i = 0; i < cpa->current_size; i++)
		free(cpa->array[i]);
	free(cpa);
}


