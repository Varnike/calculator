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

#define DEF_CMD(num, name, arg, code)						\
	case CMD_##name:							\
		printf(#name"\n");						\
		code;								\
		break;

#define DEF_JMP_CMD(num, name, code)						\
	case CMD_##name:                                                        \
        	printf(#name"\n");                                              \
		code;	                                                        \
		break;


int run_cpu(const char *namein)
{            
	CPU cpu = {};	

	cpu.csize = read_bin(namein, &cpu.code);

	if (cpu.csize < 0)
		return ERRNUM;

	start_cpu(&cpu);

	COMMANDS cmds = *(COMMANDS*)(cpu.code + cpu.ip);

	while(cpu.ip < cpu.csize) {
		//cpu_dump(&cpu, stdout);
		//reg_dump(&cpu, stdout);
		//ram_dump(&cpu, stdout);

		cmds = *(COMMANDS*)(cpu.code + cpu.ip++);
		printf("[%u]\t[%u]\t[%u]\t[%d]\n", cmds.ram, cmds.reg, cmds.imm, cmds.cmd);

		switch(cmds.cmd) {
#include "commands.h"
#undef DEF_CMD
#undef DEF_JMP_CMD
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
	
	//fprintf(file, "\n\n\n\n**********CPU CODE DUMP**********\n\n");
	int size = cpu->csize;
	if (cpu->csize > 255)
		size = 255;

	for (int i = 0; i!= size; i++) {
		fprintf(file, "%02x ", i);
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

	if (num <= 0 || num >= MAX_RAM_SIZE)
		assert(!"SEGMENTATION_FAULT");
	sleep(0.5);
	printf("\tGET RAM[%d] = %lg\n", num, cpu->RAM[num]);
	return cpu->RAM[num];
}	
