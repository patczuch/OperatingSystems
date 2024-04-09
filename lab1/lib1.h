typedef struct CharPointerArray
{
	int max_size;
	int current_size;
	char** array;
} CharPointerArray;
 
CharPointerArray* cpa_create(int max_size);
void cpa_load_file_info(CharPointerArray* structure, char* filepath);
char* cpa_get_block(CharPointerArray* cpa, int index);
void cpa_delete_block(CharPointerArray* cpa, int index);
void cpa_free(CharPointerArray* cpa);