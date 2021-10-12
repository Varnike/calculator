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

int writecmd(int cmd, FILE *file, strsize str)
{
	assert(file);
	
	if (cmd != CMD_PUSH)
		fprintf(file, "%d\n", cmd);
	else {
		ERRNUM = 0;
		val_t val = getValue(&str, 4);
		if (ERRNUM) {
			printf("syntax err: %d\n", ERRNUM);
			return ERRNUM;
		}

	       	fprintf(file, "%d ", CMD_PUSH);
		printStackVal(val, file);
	}
	return 0;
}

int compile(const char *namein, const char *nameout)
{
	assert(namein);
	assert(nameout);

	textBuff btext = {};

        btext.file_in  = open_file(namein, "r");
	btext.file_out = open_file(nameout,"w");
	
	assert(btext.file_in);
	assert(btext.file_out);
        
	if (read_from_file(&btext, namein))
                return READ_ERR;

        printf ("OK)\n");

	int cmd = 0;

        for (int i = 0; i != btext.linecnt; i++) {
		cmd = identcmd(&btext.str[i]);
		if (cmd == 0)
			continue;

		if (cmd < 0)
			return -1;

		if (writecmd(cmd, btext.file_out, btext.str[i]))
			return ERRNUM;
        }

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
	printf("%c\n", str->strptr[i]);
	val = strtod(str->realptr + i, &ptr);
	printf("VALUE is %d\n", val);

	if (errno != 0 || *ptr != '\0')         
		ERRNUM = UNKNOWN_VAL_ERR;                                  
	
	return val;	 
}
