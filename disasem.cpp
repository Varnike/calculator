#include "disasem.h"

int decompile(const char *namein, const char *nameout)
{
	assert(namein);                         
	assert(nameout);                        	
						
	FILE *file_out = open_file(nameout,"w");
	assert(file_out);                 	

	int *code = NULL;
	int codesize = read_bin(namein, &code);
	
	/*
	for (int i = 0; i != codesize / sizeof(int); i++) {
		printf(">-< %d\n", code[i]);
	}
	*/

	processDecomp(code, codesize/sizeof(int), file_out);
	
	close_file(file_out);
	return 0;
}

int read_bin(const char *namein, int **code)
{
	assert(namein);

	FILE *file_in  = open_file(namein, "rb");

	assert(file_in);

	int codesize = getFileSize(namein); 

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
