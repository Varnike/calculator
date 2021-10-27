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
$			if (num == CMD_pop || num == CMD_push) {			\
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
			printf("VALUE === %d, val = %d\n", *(val_t*)(code->data + code->ip), val);\
			code->ip += sizeof(val_t);					\
		}                                                                       \
		code->data[command_ip] = *(char *)&cmds;				\
	}										\
	else

#define DEF_JMP_CMD(num, name, ...)							\
	if (strcmp(token, #name) == 0) {						\
		COMMANDS cmds = {};							\
		cmds.cmd = num;								\
		code->data[code->ip++] = *(char*)&cmds;					\
		if (num != CMD_ret) {							\
			char *label_name = NULL;					\
			token = strtok(nullptr, "\n");					\
			printf("TOKEN IS %s\n",token);					\
			*(val_t*)(code->data + code->ip) =  getLabelIP(token, code);	\
			code->ip += sizeof(val_t);					\
		}									\
	}										\
	else

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
#include "commands.h"

#undef DEF_CMD	//TODO
#undef DEF_JMP_CMD
			
			/* else */{
				int len = strlen(token);
				if (token[len-1] == ':') {
					ERRNUM = 0;
					setLabel(token, len, code->ip, code);
					if (ERRNUM)
						goto err_clear_buff;
				} else {
					ERRNUM = SYNTAX_ERR;
					goto err_clear_buff;
				}
			}

			token = strtok(nullptr, delim);
		}
	}
err_clear_buff:

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

	code.label = (_labels *)calloc(sizeof(_labels), MAX_LABELS_CNT);
	assert(code.label);

	parse(&btext);
	
	process_asm(&btext, &code, lst_file);
	if (ERRNUM)
		goto out_free_buffer;
	
	printf("\n\nSECOND PASS\n\n\n");

	restore_data(&btext);
	code.ip = 0;

	process_asm(&btext, &code, lst_file);
	if (ERRNUM)
	        goto out_free_buffer;


	write_bin(&code, nameout);

	if(ERRNUM)
		goto out_free_buffer;

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
				btext->str[strn].len = it;
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

	char check_end = 0;
	char reg_name  = 0;
	int imm        = 0;

	if (sscanf(token, "[%d+%cx%c", &imm, &reg_name, &check_end) == 3 && check_end == ']') {
		if (reg_name - 'a' >= 0 && reg_name - 'a' <= REGS_CNT) {
			cmds->reg = 1;
			cmds->imm = 1;
			cmds->ram = 1;

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

		cmds->ram = 1;
		cmds->imm = 1;
		printf("<<\t%d\t>>\n", code->ip);
		*(val_t*)(code->data + code->ip) = (val_t)imm;
		code->ip += sizeof(val_t);
		
		return 0;
	} else if (sscanf(token, "[%cx%c",  &reg_name, &check_end) && check_end == ']') {
		if (reg_name - 'a' >= 0 && reg_name - 'a' <= REGS_CNT) {
			cmds->ram = 1;
			cmds->reg = 1;

			*(val_t*)(code->data + code->ip + sizeof(val_t)) = (reg_name - 'a');
			code->ip += sizeof(val_t);

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
		if (strncmp(lname, labelstr->label[i].name, labelstr->label[i].len - 1) == 0) {
			printf("LABEL no [%d] FOUND: name : %s, len : %d, ip : %d\n", 
					i,
					labelstr->label[i].name, 
					labelstr->label[i].len, 
					labelstr->label[i].ip);
			return labelstr->label[i].ip;
		}

	return -1;
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

int restore_data(textBuff *btext)
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
