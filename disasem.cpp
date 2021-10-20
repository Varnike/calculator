#include "disasem.h"

#define DEF_CMD(num, name, args, ...)						\
	case CMD_##name:							\
		fprintf(file, #name);						\
		for (int i = 0; i < args; i++)					\
			fprintf(file, " %d", code[ip++]);			\
		fprintf(file, "\n");						\
		break;


int decompile(const char *namein, const char *nameout)
{
	assert(namein);                         
	assert(nameout);                        	
					
	FILE *file_out = open_file(nameout,"w");
	assert(file_out);                 	

	int *code = NULL;
	int codesize = read_bin(namein, &code);
	
	if (codesize < 0) {
		fclose(file_out);
		return ERRNUM;
	}
	
	int is_valid = checkVersion((Hdr *)(code));
	
	if (is_valid) {
		free(code);
		fclose(file_out);
		return ERRNUM;
	}
	
	setCodePtr(&code);
	
	int cmd_cnt = (codesize - sizeof(Hdr))/sizeof(int);
	processDecomp(code, cmd_cnt, file_out);
	
	setRealPtr(&code);

	close_file(file_out);
	free(code);

	return 0;
}

int read_bin(const char *namein, int **code)
{
	assert(namein);

	FILE *file_in  = open_file(namein, "rb");

	assert(file_in);

	int codesize = getFileSize(namein); 
	//TODO check
	
	*code = (int *)calloc(codesize, 1);

	assert(code);

	fread(*code, sizeof(int), (codesize - sizeof(Hdr))/ sizeof(int), file_in);
	
	if (*code == NULL) {
		ERRNUM = READ_ERR;
		return -1;
	}
	
	close_file(file_in);

	return codesize;
}

void processDecomp(int *code, int len, FILE *file)
{
	assert(code);
	assert(file);

	int ip =  0;
	int val = 0;
	while(ip != len) {
		switch(code[ip++]) {
#include "commands.h"	
		default:                     
			printf("UNKNOWN CMD!!!\n");
			return;
		}
	}
}
//TODO where?

#undef DEF_CMD

//TODO DEFINES
//TODO array

int checkVersion(const Hdr *header)
{
	if (header->name != NAME)
		return ERRNUM = INVALID_SIGN_ERR;
	if (header->version != VERSION)
		return ERRNUM = WRONG_VERSION_ERR;
	else 
		return 0;
}

void setCodePtr(val_t **code)
{
	assert(code);
	assert(*code);

	*code = (val_t*)((char*)(*code) + sizeof(Hdr));
}

void setRealPtr(val_t **code)
{
	assert(code);
	assert(*code);

	*code = (val_t*)((char*)(*code) - sizeof(Hdr));
}
