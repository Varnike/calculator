#include "asm.h"

void printLst(int cmd, int pos, int size, val_t val, FILE *lstfile)
{
#if 0
	if (!lstfile)
		lstfile = NULL;
//(char*)(&num)//TODO
	if (cmd == CMD_PUSH) {
		
		fprintf(lstfile, "%04x\t\t%s %d\t\t%02x%02x %02x%02x\n", (pos - 2) * size, 
				cmdName(cmd), val, cmd & 255, (cmd / 256) & 255, 
				val & 255, (val / 256) & 255);	
	}
	else {
		fprintf(lstfile, "%04x\t\t%s\t\t%02x%02x\n", (pos - 1) *size, cmdName(cmd), 
				cmd & 255, (cmd / 256) & 255);
	}
#endif
}

#define DEF_CMD(num, name, args, ...)							\
	if (strcmp(token, #name) == 0) {						\
		COMMANDS cmds = {};							\
		cmds.cmd = num;								\
		if (args == 0) 								\
			*(COMMANDS *)(code->data + code->ip++) = cmds; 			\
		for (int i = 0; i < args; i++) {                                        \
			token = strtok(nullptr, delim);					\
			if (token == NULL)						\
				return ERRNUM = SYNTAX_ERR;				\
			ERRNUM = 0;							\
			int val = getValue(token, &cmds);				\
			if (ERRNUM)							\
				return ERRNUM;						\
			*(COMMANDS *)(code->data + code->ip++) = cmds;			\
			*(val_t*)(code->data + code->ip) = val;				\
			code->ip += sizeof(val_t);					\
		}                                                                       \
	}										\
	else
	
int process_asm(textBuff *btext, ASM *code, FILE *lst_file)
{
	const char *delim = " \n";

	for (int i = 0; i != btext->linecnt; i++) {
		printf("\t\t\tparsing one str\n");
		char *token = strtok(btext->str[i].strptr, delim);
		while (token) {
			printf("token is : \"%s\"\n", token);
#include "commands.h"
#undef DEF_CMD
			return ERRNUM = SYNTAX_ERR;

			token = strtok(nullptr, delim);
		}
	}
	return 0;
}


int compile(const char *namein, const char *nameout)
{
	assert(namein);
	assert(nameout);

	textBuff btext = {};
	
	FORMAT format = {};
	*(char*)(&format) = 3;
	printf("{%u}\t{%u}\t{%u}\n", format.num, format.reg, format.ram);

        btext.file_in  = open_file(namein, "r");
	FILE *lst_file = open_file("lst.txt", "w");
	//TODO lst
	assert(btext.file_in);
	assert(lst_file);
        
	if (read_from_file(&btext, namein))
                return READ_ERR;

	int cmd = 0;

	ASM code = {};
	code.data = (char *)calloc(sizeof(int), btext.linecnt * 2 * sizeof(val_t));
	assert(code.data);

	parse(&btext);
	int err = process_asm(&btext, &code, lst_file);
	if (err)
		return err;	

	err = write_bin(&code, nameout);

	if(err)
		return err;
	
	close_file(btext.file_in);
	close_file(lst_file);
	
	free(code.data);
	free(btext.buff);
	free(btext.str);

	printf("==compilation complete, lst file <NOT> generated==\n\n\n");
	return 0;
}

int write_bin(ASM *code, const char *nameout)
{
	assert(nameout);

	FILE *file_out = fopen(nameout, "wb");
	assert(file_out);
	
	ERRNUM = 0;
	Hdr header = {NAME, VERSION};

	int wrote = fwrite(&header, sizeof(Hdr), 1, file_out);
	if (wrote != 1)
        	return WRITE_ERR;
	
	wrote = fwrite(code->data, sizeof(char), code->ip, file_out);	
	if (wrote != code->ip)
		return WRITE_ERR;

	close_file(file_out);

	return 0;
}

val_t getValue(char *token, COMMANDS *cmds)
{
	assert(token);
	assert(cmds);

	val_t val = 0; 
	char *ptr = NULL;

	errno = 0;
	val = strtod(token, &ptr);
	if (errno != 0)
		return ERRNUM = READ_ERR;

	if (*ptr != '\0') {
		printf("token is :%s\n", ptr);
		int reg = isReg(ptr);
		if (reg >= 0) {
			cmds->reg = 1;
			return reg;
		}

		return ERRNUM = UNKNOWN_VAL_ERR;
	}
	
	cmds->reg = 1;

	return val;	 
}

void parse(textBuff *btext)
{
	if (btext == NULL || btext->str == NULL)
		return;

	for (int strn = 0; strn != btext->linecnt; strn++) {
		int it = 0;
		for (; it != btext->str[strn].len; it++) {
			if (btext->str[strn].strptr[it] == ';') {
				btext->str[strn].strptr[it] = '\0';
				break;
			}
		}
		btext->str[strn].strptr[it] = '\0';
	}
}

int isReg(char *token)
{
	if (token[1] == 'x' && token[2] == '\0') {
		int regno = token[0] - 'a';
		if (regno <= 4)	//max nomder of regs
			return regno;
		else 
			return -1;
	}
	return -1;
}
