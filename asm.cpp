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
		int command_ip = code->ip++;						\
		for (int i = 0; i < args; i++) {                                        \
			token = strtok(nullptr, delim);					\
$			if (token == NULL) {						\
			       	if (i == 0)						\
					return ERRNUM = SYNTAX_ERR;			\
				else 							\
					break;						\
			}								\
$			if (num == CMD_POP || num == CMD_PUSH) {			\
				int is_ram = isRAM(token, &cmds, code);			\
				if (is_ram == 0)					\
					break;						\
				if (is_ram > 0)						\
					return ERRNUM;					\
			}								\
$			ERRNUM = 0;							\
			val_t val = getValue(token, &cmds);				\
			if (ERRNUM)							\
				return ERRNUM;						\
			*(val_t*)(code->data + code->ip) = val;				\
			printf("VALUE === %d, val = %d\n", *(val_t*)(code->data + code->ip), val);	\
			code->ip += sizeof(val_t);					\
		}                                                                       \
		code->data[command_ip] = *(char *)&cmds;				\
	}										\
	else

#define DEF_JMP_CMD(num, name, ...)							\
	if (strcmp(token, #name) == 0) {						\
		COMMANDS cmds = {};							\
		cmds.num = num;								\
		char label_name[MAX_LABEL_LEN] = {};					\
		token = strtok(nullptr, " ");						\
		if (sscanf(token, "%s", label_name))					\
			return ERRNUM = SYNTAX_ERR;					\
		val_t label_p = getLabelPtr(label_name, labels);			\
		*(COMMANDS *)(code->data + data->ip++) = cmds;				\
		*(val_t*)(code->data + code->ip) = label_p;				\
		code->ip += sizeof(val_t);						\
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
#undef DEF_CMD	//TODO
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
#if 0
	FORMAT format = {};
	*(char*)(&format) = 3;
	printf("{%u}\t{%u}\t{%u}\t{%u}\n", format.imm, format.reg, format.ram, format.unused);
#endif
	ERRNUM = 0;

        btext.file_in  = open_file(namein, "r");
	FILE *lst_file = open_file("lst.txt", "w");
	
	//TODO lst
	assert(btext.file_in);
	assert(lst_file);
       
	int cmd = 0;
	ASM code = {}; 

	read_from_file(&btext, namein);	
	if (ERRNUM)
                goto out_free_buffer;

	code.data = (char *)calloc(sizeof(int), btext.linecnt * 2 * sizeof(val_t));
	assert(code.data);

	parse(&btext);

	process_asm(&btext, &code, lst_file);
	if (ERRNUM)
		goto out_free_buffer;

	write_bin(&code, nameout);

	if(ERRNUM)
		goto out_free_buffer;

out_free_buffer:	
	close_file(btext.file_in);
	close_file(lst_file);
	
	free(code.data);
	free(btext.buff);
	free(btext.str);
	if (!ERRNUM)
		printf("==compilation complete, lst file <NOT> generated==\n\n\n");
	else 
		perror("An error ocured while compilation\n");
	return ERRNUM;
}

int write_bin(ASM *code, const char *nameout)
{
	assert(nameout);

	FILE *file_out = fopen(nameout, "wb");
	assert(file_out);

	ERRNUM = 0;
	Hdr header = {NAME, VERSION};

	int wrote = fwrite(&header, sizeof(Hdr), 1, file_out);
	if (wrote != 1) {
        	ERRNUM = WRITE_ERR;
		goto out_close_file;
	}
	
	wrote = fwrite(code->data, sizeof(char), code->ip, file_out);	
	if (wrote != code->ip) {
		ERRNUM = WRITE_ERR;
		goto out_close_file;
	}

out_close_file:
	close_file(file_out);
	return ERRNUM;
}

val_t getValue(char *token, COMMANDS *cmds) 
{
	if (token == NULL)
		return 0;

	assert(cmds);
	
	if ((cmds->cmd == CMD_PUSH || cmds->cmd == CMD_POP) &&
			(cmds->imm || cmds->reg)) { // was already used(3am)//TODO ??
$		return 0;
	}

	val_t val = 0; 
	char *ptr = NULL;

	errno = 0;
	val = strtod(token, &ptr);
	
	if (errno != 0)
		return ERRNUM = READ_ERR;

	if (*ptr != '\0') {
		printf("value token is :%s\n", ptr);
		int reg = isReg(ptr);
		if (reg >= 0) {
			cmds->reg = 1;
			return reg;
		}

		return ERRNUM = UNKNOWN_VAL_ERR;
	}
	
	cmds->reg = 0;
	cmds->imm = 1;

	return val;	 
}

void parse(textBuff *btext)
{
	assert(btext);
	assert(btext->str);

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
{//TODO ???
	if (token[1] == 'x' && token[2] == '\0') {
		int regno = token[0] - 'a';
		if (regno <= REGS_CNT)//max nomder of regs
			return regno;
		else 
			return -1;
	}
	return -1;
}

int isRAM(char *token, COMMANDS *cmds, ASM *code)
{
	if (token == NULL) 
		return -1;

	assert(cmds);
	assert(code);

	//printf("RAM is token : %s\n", token);
	
	char check_end = 0;
	char reg_name  = 0;
	int imm        = 0;
$
	if (sscanf(token, "[%d+%cx%c", &imm, &reg_name, &check_end) == 3 && check_end == ']') {
		if (reg_name - 'a' >= 0 && reg_name - 'a' <= REGS_CNT) {
			cmds->reg = 1;
			cmds->imm = 1;
			cmds->ram = 1;
$
			*(val_t*)(code->data + code->ip) = (reg_name - 'a');
		        *(val_t*)(code->data + code->ip + sizeof(val_t)) = imm;
			code->ip += 2 * sizeof(val_t);
			
			return 0;
		} else {
			return ERRNUM = INVALID_INPUT_VAL;
		}
	} else if (sscanf(token, "[%d%c", &imm, &check_end) == 2 && check_end == ']') {
		if (imm < 0)
			return ERRNUM = INVALID_INPUT_VAL;
$
		cmds->ram = 1;
		cmds->imm = 1;
		printf("<<\t%d\t>>\n", code->ip);
		*(val_t*)(code->data + code->ip) = (val_t)imm;
		//printf("{{\t%d\t}}\n", *(val_t*)(code->data + code->ip));
		code->ip += sizeof(val_t);
		//printf("<<\t%d\t>>\n", code->ip);

		return 0;
	} else if (sscanf(token, "[%cx%c",  &reg_name, &check_end) && check_end == ']') {
		if (reg_name - 'a' >= 0 && reg_name - 'a' <= REGS_CNT) {
			cmds->ram = 1;
			cmds->reg = 1;
$
			*(val_t*)(code->data + code->ip + sizeof(val_t)) = (reg_name - 'a');
			code->ip += sizeof(val_t);

			return 0;
		} else {
			return ERRNUM = INVALID_INPUT_VAL;
		}
	}
$
	return -1; // THIS IS NOT RAM
}
