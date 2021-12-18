#include "cpu.h"

int start_cpu(CPU *cpu)
{
	assert(cpu);

	if (StackCtor(&cpu->stack, 8))
		return ERRNUM;

	return NO_ERR;
}

int end_cpu(CPU *cpu)
{
	assert(cpu);

	free(cpu->code);

	if (StackDtor(&cpu->stack))
		return ERRNUM;
	return NO_ERR;
}


int run_cpu(const char *namein)
{            
	CPU cpu = {};	

	cpu.csize = read_bin(namein, &cpu.code);

	if (cpu.csize < 0)
		return ERRNUM;

	start_cpu(&cpu);

	COMMANDS cmds = *(COMMANDS*)(cpu.code + cpu.ip);
	val_t arg_val = 0;

	while(cpu.ip < cpu.csize) {
		//cpu_dump(&cpu, stdout);
		//reg_dump(&cpu, stdout);
		//ram_dump(&cpu, stdout);

		cmds = *(COMMANDS*)(cpu.code + cpu.ip++);
		//printf("[%u]\t[%u]\t[%u]\t[%d]\n", cmds.ram, cmds.reg, cmds.imm, cmds.cmd);

		switch(cmds.cmd) {
#define DEF_CMD(num, name, arg, code)						\
	case CMD_##name:							\
		printf(#name"\n");						\
		code;								\
		break;

#define DEF_JMP_CMD(num, name, code)						\
	case CMD_##name:                                                        \
        	printf(#name"\n");						\
		code;	                                                        \
		break;

#define DEF_COND_JMP_CMD(num, name, cond)					\
	case CMD_##name: {							\
		printf(#name"\n");						\
		arg_val = _POP;							\
		val_t b = _POP;							\
		if (b cond arg_val)						\
			IP = (int)CODE(IP);					\
		else 								\
			IP += (int)sizeof(val_t);				\
 		}								 \
		break;

#include "commands.h"
#undef DEF_CMD
#undef DEF_JMP_CMD
#undef DEF_COND_JMP_CMD
		default:
			return ERRNUM = UNKNOWN_CMD_ERR;
			break;
		}
	}

label_exit:
	end_cpu(&cpu);
		
	return ERRNUM;
	
}


void cpu_dump(CPU *cpu, FILE *file)
{
	assert(file);

	_StackDump(&cpu->stack, __func__, __FILE__, __LINE__);
	
	assert(cpu);
	assert(cpu->code);
	
	int size = cpu->csize;
	if (cpu->csize > 255)
		size = 255;

	for (int i = 0; i!= size; i++) {
		fprintf(file, "%02x ", (unsigned)i);
	}

	printf("\n");

	for (int i = 0; i != size; i++) {
		fprintf(file, "%02x ", (unsigned char)cpu->code[i]);
	}

	fprintf(file, "\n");
	fprintf(file, "%*s\n", cpu->ip * 3 + 1, "^");

#if CPU_SLEEP == 1
	getchar();
#endif
}

void ram_dump(CPU *cpu, FILE *file)
{
	assert(file);

	for (int i = 0; i != MAX_RAM_SIZE; i++) {
		if (i % 10 == 0) 
			printf("\n[%d] : ", i);

		fprintf(file, "%d ", (int)cpu->RAM[i]);
	}
}

void reg_dump(CPU *cpu, FILE *file)
{
	assert(file);

	for(int i = 0; i != REGS_CNT; i++) {
		printf("[%cx] : %d\n", ('a' + i), (int)cpu->regs[i]);
	}
}
val_t _get_ram(CPU *cpu, int num)
{
	assert(cpu);
	
	if (num >= MAX_RAM_SIZE + MAX_VRAM_SIZE - 1)
		assert(!"SEGMENTATION_FAULT");

	sleep(0);
		
	return cpu->RAM[num];
}

val_t *_ARG(CPU *cpu, val_t *arg_val, COMMANDS cmds)
{
	if (cmds.ram == 1) {
		int a = (int)*(val_t*)(cpu->code + cpu->ip);
		if (cmds.reg == 1) {
			int reg_data = (int)cpu->regs[a];
			if (cmds.imm == 1) {
		   		int b = (int)*(val_t *)(cpu->code + cpu->ip + SIZEOF);
				cpu->ip += 2 * SIZEOF;
				return (cpu->RAM + b + reg_data);
			} else {
				cpu->ip += SIZEOF;
				return (cpu->RAM + reg_data);
			}                                
		} else if (cmds.imm == 1) {
			cpu->ip += SIZEOF;
			return (cpu->RAM + a);
		}
	} else if (cmds.reg == 1){
		int a = (int)*(val_t*)(cpu->code + cpu->ip);
		cpu->ip += SIZEOF;
		return (cpu->regs + a);
	} else {
		cpu->ip += (int)sizeof(val_t);
		return (val_t*)(cpu->code + cpu->ip - SIZEOF);
	}

	cpu->ip += (int)sizeof(val_t);

	return (arg_val);
}
