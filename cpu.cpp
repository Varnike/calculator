#include "cpu.h"
#define DEF_CMD(num, name, arg, code)						\
	case CMD_##name:							\
		printf(#name"\n");						\
		code;								\
		cpu.ip += (arg * sizeof(val_t));				\
		break;


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

	while(cpu.ip < cpu.csize) {
		cpu_dump(cpu);

		cmds = *(COMMANDS*)(cpu.code + cpu.ip++);
		printf("[%u]\t[%u]\t[%u]\t[%d]\n", cmds.ram, cmds.reg, cmds.imm, cmds.cmd);

		switch(cmds.cmd) {
#include "commands.h"
		default:
			return ERRNUM = UNKNOWN_CMD_ERR;
			break;
		}
	}

	end_cpu(&cpu);
		
	return ERRNUM;
	
}
#undef DEF_CMD
void cpu_dump(CPU cpu)
{
	assert(cpu.code);
	
	if (cpu.csize > 255)
		cpu.csize = 255;

	for (int i = 0; i!= cpu.csize; i++) {
		printf("%02x ", i);
	}

	printf("\n");

	for (int i = 0; i != cpu.csize; i++) {
		printf("%02x ", (unsigned char)cpu.code[i]);
	}

	printf("\n");
	printf("%*s\n", cpu.ip * 3 + 1, "^");

#if CPU_SLEEP == 1
	getchar();
#endif
}
