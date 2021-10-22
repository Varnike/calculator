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

	int cmd_cnt = (codesize - sizeof(Hdr))/sizeof(int);
	processDecomp(code, cmd_cnt, file_out);
	
	close_file(file_out);
	free(code);

	return 0;
}

int read_bin(const char *namein, int **code)
{
	assert(namein);
	
	FILE *file_in  = open_file(namein, "rb");
	assert(file_in);

	int hdr_bad = checkHdr(file_in);	
	if (hdr_bad) {
		fclose(file_in);
		return -1;
	}
	
	int codesize = getFileSize(namein); 
	assert(codesize >= 0);
	*code = (int *)calloc(codesize, 1);
	assert(code);

	fread(*code, sizeof(int), (codesize - sizeof(Hdr))/ sizeof(int), file_in);
	
	if (*code == NULL) {
		ERRNUM = READ_ERR;
		return -1;
	}
	
	fclose(file_in);

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
			fprintf(file, "UNKNOWN CMD!!!\n");
			return;
		}
	}
}

#undef DEF_CMD

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

int checkHdr(FILE *filein)
{
	Hdr header = {};
	int read = fread(&header, sizeof(Hdr), 1, filein);
	
	if (read != 1)
		return ERRNUM = HDR_READ_ERR;

	if (header.name != NAME)
		return ERRNUM = INVALID_SIGN_ERR;

	if (header.version != VERSION)
		return ERRNUM = WRONG_VERSION_ERR;

	return 0;
}

