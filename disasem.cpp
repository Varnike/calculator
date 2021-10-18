#include "disasem.h"

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
	fread(*code, sizeof(int), codesize / sizeof(int), file_in);
	
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

	int pp =  0;
	int val = 0;
	while(pp != len) {
		switch(code[pp++]) {
		case CMD_PUSH:
			fprintf(file, "push %d\n", code[pp++]);
			break;               
		case CMD_ADD:                
			fprintf(file, "add\n");
			break;               
		case CMD_SUB:                
			fprintf(file, "sub\n");
			break;               
		case CMD_MUL:                
			fprintf(file, "mul\n");
			break;               
		case CMD_DIV:                
			fprintf(file, "div\n");
			break;               
		case CMD_OUT:                
			fprintf(file, "out\n");
			break;               
		case CMD_HLT:                
			fprintf(file, "hlt\n");
			break;               
		default:                     
			printf("UNKNOWN CMD!!!");
			return;
		}
	}
}
//TODO DEFINES
//TODO array
char* cmdName(int cmd)
{	
	char *name = NULL;
	switch (cmd) {
	case CMD_PUSH:
		name = (char *)"push";
		break;
	case CMD_ADD:
		name = (char *)"add";
		break;
	case CMD_SUB:
		name = (char *)"sub";
		break;
	case CMD_MUL:
		name = (char *)"mul";
		break;
	case CMD_DIV:
		name = (char *)"div";
		break;
	case CMD_OUT:
		name = (char *)"out";
		break;
	case CMD_HLT:
		name = (char *)"hlt";
		break;
	default:
		return NULL; 
	}
	return name;
}

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
