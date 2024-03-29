#include "asm.h"

static int restore_data(textBuff *btext);
static void parse(textBuff *btext);
static struct _labels make_label(char *name, int ip, int len);
static int process_args(const int args, COMMANDS *cmds, ASM *code, char *token);


int process_asm(textBuff *btext, ASM *code, FILE *lst_file)
{	
	assert(btext);
	assert(btext->str);
	assert(code);
	assert(lst_file);

	ERRNUM = 0;

	const char *delim = " \n";

	for (int i = 0; i != btext->linecnt; i++) {
		printf("parsing one str\n");
		char *token = strtok(btext->str[i].strptr, delim);
		while (token) {
			printf("command name : \"%s\"\n", token);
#define DEF_CMD(num, name, args, ...)							\
	if (strcmp(token, #name) == 0) {						\
		COMMANDS cmds = {};							\
		cmds.cmd = num;								\
		int command_ip = code->ip++;						\
		if(process_args(args, &cmds, code, token))				\
			return ERRNUM;							\
		code->data[command_ip] = *(char *)&cmds;				\
	}										\
	else

#define DEF_JMP_CMD(num, name, ...)							\
	if (strcmp(token, #name) == 0) {						\
		COMMANDS cmds = {};							\
		cmds.cmd = num;								\
		code->data[code->ip++] = *(char*)&cmds;					\
		if (num != CMD_ret) {							\
			token = strtok(nullptr, "\n");					\
			printf("TOKEN IS %s\n",token);					\
			*(val_t*)(code->data + code->ip) =  getLabelIP(token, code);	\
			code->ip += (int)sizeof(val_t);					\
		}									\
	}										\
	else


#define DEF_COND_JMP_CMD(num, name, ...) DEF_JMP_CMD(num, name)

#include "commands.h"

#undef DEF_CMD
#undef DEF_COND_JMP_CMD
#undef DEF_JMP_CMD
			
			/* else */{
				int len = (int)strlen(token);
				if (len >= 1 && token[len-1] == ':') {
					ERRNUM = 0;
					setLabel(token, len, code->ip, code);
					if (ERRNUM)
						return ERRNUM;
				} else {
					return ERRNUM = SYNTAX_ERR;
				}
			}

			token = strtok(nullptr, delim);
		}
	}
	
	return ERRNUM;
}


int compile(const char *namein, const char *nameout)
{
	assert(namein);
	assert(nameout);

	textBuff btext = {};

	ERRNUM = 0;

        btext.file_in  = open_file(namein, "r");
	FILE *lst_file = open_file("lst.txt", "w");
	
	assert(btext.file_in);
	assert(lst_file);
       
	ASM code = {}; 

	read_from_file(&btext, namein);

	CHECK_ERR(out_free_buffer);

	code.data = (char *)calloc(sizeof(int), (unsigned)btext.linecnt * 2 * sizeof(val_t));
	assert(code.data);

	code.label = (_labels *)calloc(sizeof(_labels), MAX_LABELS_CNT);
	assert(code.label);

	parse(&btext);
	
	process_asm(&btext, &code, lst_file);
	CHECK_ERR(out_free_buffer);
	
	printf("\n\nSECOND PASS\n\n\n");

	restore_data(&btext);
	code.ip = 0;

	process_asm(&btext, &code, lst_file);
	CHECK_ERR(out_free_buffer);

	write_bin(&code, nameout);
	CHECK_ERR(out_free_buffer);
	

out_free_buffer:	
	close_file(btext.file_in);
	close_file(lst_file);

	free(code.label);	
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

	size_t wrote = fwrite(&header, sizeof(Hdr), 1, file_out);
	if (wrote != 1) {
        	ERRNUM = WRITE_ERR;
		goto out_close_file;
	}
	
	wrote = fwrite(code->data, sizeof(char), (size_t)code->ip, file_out);	
	if (wrote != (size_t)code->ip) {
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
	
	val_t val = 0; 
	char *ptr = NULL;

	errno = 0;
	val = strtod(token, &ptr);
	
	if (errno != 0)
		return ERRNUM = READ_ERR;

	if (*ptr != '\0') {
		printf("value token is :%s\n", ptr);
		int reg = setReg(ptr);
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

static void parse(textBuff *btext)
{
	assert(btext);
	assert(btext->str);

	for (int strn = 0; strn != btext->linecnt; strn++) {
		int it = 0;
		for (; it != btext->str[strn].len; it++) {
			if (btext->str[strn].strptr[it] == ';') {
				btext->str[strn].strptr[it] = '\0';
				btext->str[strn].len = it;
				break;
			}
		}
		btext->str[strn].strptr[it] = '\0';
	}
}

int setReg(char *token)
{
	if (token[1] == 'x' && token[2] == '\0') {
		int regno = token[0] - 'a';
		if (regno <= REGS_CNT)//max nomder of regs
			return regno;
		else 
			return -1;
	}
	return -1;
}

int setRAM(char *token, COMMANDS *cmds, ASM *code)
{
	if (token == NULL) 
		return -1;

	assert(cmds);
	assert(code);

	char check_end = 0;
	char reg_name  = 0;
	int imm        = 0;
	
	printf("\n!!! %s \n\n", token);
	if (sscanf(token, "[%cx+%d%c", &reg_name, &imm, &check_end) == 3 && check_end == ']') {
		if (reg_name - 'a' >= 0 && reg_name - 'a' <= REGS_CNT) {
			cmds->reg = 1;
			cmds->imm = 1;
			cmds->ram = 1;

			*(val_t*)(code->data + code->ip) = (reg_name - 'a');
		        *(val_t*)(code->data + code->ip + (int)sizeof(val_t)) = imm;
			code->ip += 2 * (int)sizeof(val_t);
			
			return 0;
		} else {
			return ERRNUM = INVALID_INPUT_VAL;
		}
	} else if (sscanf(token, "[%d%c", &imm, &check_end) == 2 && check_end == ']') {
		if (imm < 0)
			return ERRNUM = INVALID_INPUT_VAL;

		cmds->ram = 1;
		cmds->imm = 1;
		printf("<<\t%d\t>>\n", code->ip);
		*(val_t*)(code->data + code->ip) = (val_t)imm;
		code->ip += (int)sizeof(val_t);
		
		return 0;
	} else if (sscanf(token, "[%cx%c",  &reg_name, &check_end) && check_end == ']') {
		if (reg_name - 'a' >= 0 && reg_name - 'a' <= REGS_CNT) {
			cmds->ram = 1;
			cmds->reg = 1;

			*(val_t*)(code->data + code->ip) = (reg_name - 'a');
			
			code->ip += (int)sizeof(val_t);

			return 0;
		} else {
			return ERRNUM = INVALID_INPUT_VAL;
		}
	}

	return -1; // THIS IS NOT RAM
}

val_t getLabelIP(const char *lname, ASM *labelstr)
{
	for (int i = 0; i != labelstr->labelcnt; i++)
		if (strncmp(lname, labelstr->label[i].name, (unsigned)labelstr->label[i].len - 1) == 0) {
			printf("LABEL no [%d] FOUND: name : %s, len : %d, ip : %d\n", 
					i,
					labelstr->label[i].name, 
					labelstr->label[i].len, 
					labelstr->label[i].ip);
			return (val_t)labelstr->label[i].ip;
		}

	return -1.0;
}

static struct _labels make_label(char *name, int ip, int len)
{
	_labels nlabel = {};
                    
	nlabel.name = name; 
	nlabel.ip   = ip;   
	nlabel.len  = len;  

	return nlabel;
}
int setLabel(char *name, const int len, const int ip, ASM *labelstr)
{
	assert(name);
	assert(labelstr);

	if (len >= MAX_LABELS_CNT)
		return ERRNUM = LABEL_CNT_ERR;
	
	if (getLabelIP(name, labelstr) != -1) {
		return 0;
	}
	
	*(labelstr->label + labelstr->labelcnt++) = make_label(name, ip, len);	
	
	printf("LABEL no [%d] ADDED: name : %s, len : %d, ip : %d\n", labelstr->labelcnt - 1, name, len, ip);

	return 0;
}

static int restore_data(textBuff *btext)
{
	assert(btext);
	assert(btext->str);

	for (int linec = 0; linec != btext->linecnt; linec++) {
		for (int it = 0; it != btext->str[linec].len; it++) {
			if (btext->str[linec].strptr[it] == '\0') {
				btext->str[linec].strptr[it] = ' ';
			}
		}
	}

	return 0;
}

int process_args(const int args, COMMANDS *cmds, ASM *code, char *token)
{
	for (int i = 0; i < args; i++) {
		token = strtok(nullptr, " \n");
		
		if (token == NULL) {
			if (i == 0)
				return ERRNUM = SYNTAX_ERR;
			else
				break;
		}

		if (cmds->cmd == CMD_pop || cmds->cmd == CMD_push) {
			int is_ram = setRAM(token, cmds, code);
			
			if (is_ram == 0)
				break;
			
			if (is_ram > 0)
				return ERRNUM;
		}

		ERRNUM = 0;
		val_t val = getValue(token, cmds);
		
		if (ERRNUM)
			return ERRNUM;

		*(val_t*)(code->data + code->ip) = val;
		code->ip += (int)sizeof(val_t);
	}

	return 0;	
}
