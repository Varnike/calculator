#include "disasem.h"


int decompile(const char *namein, const char *nameout)
{
	assert(namein);                         
	assert(nameout);                        	
	
	DISASM dasm = {};

	dasm.file_out = open_file(nameout,"w");
	assert(dasm.file_out);                 	

	dasm.codesize = read_bin(namein, &dasm.data);
	
	if (dasm.codesize < 0) {
		fclose(dasm.file_out);
		return ERRNUM;
	}

	processDecomp(&dasm);
	
	close_file(dasm.file_out);
	free(dasm.data);

	return 0;
}

int read_bin(const char *namein, char **code)
{
	assert(namein);
	
	FILE *file_in  = open_file(namein, "rb");
	assert(file_in);

	int hdr_bad = checkHdr(file_in);	
	if (hdr_bad) {
		fclose(file_in);
		return -1;
	}
	
	int codesize = getFileSize(namein) - sizeof(Hdr); 
	assert(codesize >= 0);
	*code = (char *)calloc(codesize, 1);
	assert(code);

	fread(*code, sizeof(char), codesize, file_in);
	
	if (*code == NULL) {
		ERRNUM = READ_ERR;
		return -1;
	}
	
	fclose(file_in);

	return codesize;
}

#define DEF_CMD(num, name, args, ...)								\
	case CMD_##name:									\
		fprintf(dasm->file_out, #name);							\
		for (int i = 0; i < args; i++) {						\
			if (cmds.ram == 1) {							\
				if (cmds.reg == 1) {						\
					if (cmds.imm == 1) {					\
						fprintf(dasm->file_out, " [%d+%cx]", 		\
						   (int)*(val_t *)(dasm->data + dasm->ip), 'a' +\
						   (int)*(val_t *)(dasm->data + dasm->ip));	\
							dasm->ip += sizeof(val_t);		\
					} else {						\
						fprintf(dasm->file_out, " [%cx]", 		\
						   'a'+ (int)*(val_t *)(dasm->data + dasm->ip));\
					}							\
				} else {							\
					fprintf(dasm->file_out, " [%d]",			\
						(int)*(val_t *)(dasm->data + dasm->ip));	\
				}								\
				break;								\
			} else if (cmds.reg == 0) {						\
				fprintf(dasm->file_out," %lg",*(val_t *)(dasm->data+dasm->ip));\
			} else	{								\
				fprintf(dasm->file_out, " %cx",(int)dasm->data[dasm->ip] + 'a');\
			}									\
			dasm->ip += sizeof(val_t);						\
			break;									\
		}										\
		fprintf(dasm->file_out, "\n");							\
		break;

#define DEF_JMP_CMD(num, name, ...)								\
	case CMD_##name:									\
		fprintf(dasm->file_out, #name);							\
		if (num == CMD_ret) {								\
			fprintf(dasm->file_out, "\n");						\
			break;									\
		}										\
		fprintf(dasm->file_out," %02x %02x\n",(int)*(val_t *)(dasm->data+dasm->ip) & 255,\
				((int)*(val_t *)(dasm->data+dasm->ip) / 256) & 255);		\
		dasm->ip += sizeof(val_t);							\
		break;

#define DEF_COND_JMP_CMD(num, name, ...) DEF_JMP_CMD(num, name)
void processDecomp(DISASM *dasm)
{
	assert(dasm->data);
	assert(dasm->file_out);

	int val = 0;
	COMMANDS cmds = {};

	while(dasm->ip < dasm->codesize) {
		cmds = *(COMMANDS*)(dasm->data + dasm->ip++);

		switch(cmds.cmd) {
#include "commands.h"
#undef DEF_CMD
#undef DEF_COND_JMP_CMD
#undef DEF_JMP_CMD
		default:                     
			fprintf(dasm->file_out, "UNKNOWN CMD!!!\n");
			return;
		}
	}
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

