#include "asm.h"

#define DEF_CMD(num, name, args, ...)							\
	if (strcmp(token, #name) == 0) {						\
		StackPush(stack, num);							\
		for (int i = 0; i < args; i++) {                                        \
			token = strtok(nullptr, delim);					\
			if (token == NULL)						\
				return ERRNUM = SYNTAX_ERR;				\
			ERRNUM = 0;							\
			int val = getValue(token);					\
			if (ERRNUM)							\
				return ERRNUM;						\
        		StackPush(stack, val);						\
		}                                                                       \
		goto next_label;							\
	}

void printLst(int cmd, int pos, int size, val_t val, FILE *lstfile)
{
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
}

int process_asm(textBuff *btext, Stack *stack, FILE *lst_file)
{
	const char *delim = " \n";

	for (int i = 0; i != btext->linecnt; i++) {
		printf("\t\t\tparsing one str\n");
		char *token = strtok(btext->str[i].strptr, delim);
		while (token) {
			printf("token is : \"%s\"\n", token);
#include "commands.h"

			return ERRNUM = SYNTAX_ERR;
next_label:
			token = strtok(nullptr, delim);
		}
	}
	return 0;
}

#undef DEF_CMD

int compile(const char *namein, const char *nameout)
{
	assert(namein);
	assert(nameout);

	textBuff btext = {};

        btext.file_in  = open_file(namein, "r");
	FILE *lst_file = open_file("lst.txt", "w");
	
	assert(btext.file_in);
	assert(lst_file);
        
	if (read_from_file(&btext, namein))
                return READ_ERR;

	int cmd = 0;

	Stack stack = {};
	StackCtor(&stack, 8);

	parse(&btext);
	int err = process_asm(&btext, &stack, lst_file);
	if (err)
		return err;	

	err = write_bin(getStackData(&stack), getStackSize(&stack), nameout);

	if(err)
		return err;

	StackDtor(&stack);

	close_file(btext.file_in);
	close_file(lst_file);
	
	free(btext.buff);
	free(btext.str);

	printf("==compilation complete, lst file <NOT> generated==\n\n\n");
	return 0;
}

int write_bin(val_t *code, int codesize, const char *nameout)
{
	assert(nameout);

	FILE *file_out = fopen(nameout, "wb");
	assert(file_out);
	
	ERRNUM = 0;
	Hdr header = {NAME, VERSION};

	int wrote = fwrite(&header, sizeof(Hdr), 1, file_out);
	if (wrote != 1)
        	return WRITE_ERR;
	
	wrote = fwrite(code, sizeof(val_t), codesize, file_out);	
	if (wrote != codesize)
		return WRITE_ERR;

	close_file(file_out);

	return 0;
}

val_t getValue(char *token)
{
	assert(token);

	val_t val = 0; 
	char *ptr = NULL;

	errno = 0;

	val = strtod(token, &ptr);

	if (errno != 0 || *ptr != '\0')         
		ERRNUM = UNKNOWN_VAL_ERR;                                  
	
	return val;	 
}

void parse(textBuff *btext)
{
	if (btext == NULL || btext->str == NULL)
		return;

	for (int strn = 0; strn != btext->linecnt; strn++) {
		int it = 0;
		for (it = 0; it != btext->str[strn].len; it++) {
			if (btext->str[strn].strptr[it] == ';') {
				btext->str[strn].strptr[it] = '\0';
				break;
			}
		}
		btext->str[strn].strptr[it] = '\0';
	}
}
