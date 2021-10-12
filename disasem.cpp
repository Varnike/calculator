#include "disasem.h"

int decompile(const char *namein, const char *nameout)
{
	assert(namein);                         
	assert(nameout);                        	
						
	FILE *file_in  = open_file(namein, "r");
	FILE *file_out = open_file(nameout,"w");
						
	assert(file_in);                  
	assert(file_out);                 
					
	int cmd   = 0;
	val_t val = 0;
	while (fscanf(file_in, "%d", &cmd) > 0) {
		const char *commandn = cmdName(cmd);

		if (commandn == NULL)
			return UNKNOWN_CMD_ERR;

		if (cmd == CMD_PUSH) {
			fscanf(file_in, "%d", &val); //TODO
			fprintf(file_out, "%s %d\n", commandn, val);
		}
		else 	
			fprintf(file_out, "%s\n", commandn);
	}
	return 0;
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
