#include "asm.h"

int identcmd(strsize *str)
{
	assert(str->realptr);

	for (int i = 0; i != str->len; i++, str->realptr++) { 
        	if (*str->realptr == ';')
		        return 0;        
		if (!isspace(*str->realptr))
	                break;
	}

	for (int j = 0; j!= str->len; j++)
        	if (str->realptr[j] == ';')
        	        str->realptr[j] = '\0';

	//TODO in future detect syntax errors better
	if (strncmp(str->realptr, "push", 4) == 0)
		return CMD_PUSH;

	if (strncmp(str->realptr, "add", 3) == 0)
        	return CMD_ADD;

	if (strncmp(str->realptr, "sub", 3) == 0)
        	return CMD_SUB;

	if (strncmp(str->realptr, "mul", 3) == 0)
        	return CMD_MUL;

	if (strncmp(str->realptr, "div", 3) == 0)
	        return CMD_DIV;

	if (strncmp(str->realptr, "out", 3) == 0)
        	return CMD_OUT;
	
	if (strncmp(str->realptr, "hlt", 3) == 0)
		return CMD_HLT;

	if (strncmp(str->realptr, ";", 1) == 0)
		return 0;
	
	ERRNUM = UNKNOWN_ERR;
	printf("ERROR\n");
	return -1;
}

int writecmd(int cmd, strsize *str, Stack *stack, FILE *lstfile)
{
	assert(str);
	
	StackPush(stack, cmd);
	
	val_t val = 0;

	if (cmd == CMD_PUSH) {
		ERRNUM = 0;
		val = getValue(str, 4);
	
		if (ERRNUM) {
			printf("syntax err: %d\n", ERRNUM);
			return ERRNUM;
		}
		
		StackPush(stack, val);
	}

	printLst(cmd, getStackSize(stack), sizeof(val_t), val, lstfile);

	return 0;
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

int parse(textBuff *btext, Stack *stack, FILE *lst_file)
{
	int cmd = 0;
	for (int i = 0; i != btext->linecnt; i++) {                  
		cmd = identcmd(&btext->str[i]);                      
		if (cmd == 0)                                       
			continue;                                   
								    
		if (cmd < 0)                                        
			return -1;                                  
								    
		if (writecmd(cmd, &btext->str[i], stack, lst_file)) 
			return ERRNUM;                              
	}

	return 0;
}

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
	
	int err = parse(&btext, &stack, lst_file);
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

	printf("==compilation complete, lst file generated==\n");
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

val_t getValue(strsize *str, int valpos)
{
	assert(str);
	assert(str->realptr);

	int i = valpos;

	for (int j = 0; j!= str->len; j++) {
		if (str->realptr[j] == ';')    
		        str->realptr[j] = '\0';
	}

	val_t val = 0; 
						  
	str->realptr[str->len] = '\0';              
		  
	char *ptr = nullptr;

	errno = 0;

	//printf("%c\n", str->strptr[i]);
	
	val = strtod(str->realptr + i, &ptr);

	if (errno != 0 || *ptr != '\0')         
		ERRNUM = UNKNOWN_VAL_ERR;                                  
	
	return val;	 
}

#if 0
void setHdr(val_t **code)
{
	assert(code);
	assert(*code);
	Hdr header = {};

	//*(Hdr*)(*code) = header;
	*code = (val_t*)((char*)(*code) + sizeof(Hdr));
}

void setCodePtr(val_t **code)
{
	assert(code);
	assert(*code);

	*code = (val_t*)((char*)(*code) - sizeof(Hdr));
}
#endif
