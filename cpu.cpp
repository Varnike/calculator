#include "cpu.h"
#define DEF_CMD(num, name, arg, code)						\
	case CMD_##name:							\
		printf(#name"\n");						\
		code;								\
		cpu.ip += (arg);						\
		break;

Stack stack = {};

int start_cpu()
{
	if (StackCtor(&stack, 8))
		return ERRNUM;
	return NO_ERR;
}

int run_cpu(const char *namein)
{            
	CPU cpu = {};	

	int size = read_bin(namein, &cpu.code);

	if (size < 0)
		return ERRNUM;
	
	cpu.csize = (size - sizeof(Hdr))/sizeof(val_t);
	
	start_cpu();
	
	while(cpu.ip != cpu.csize) { 
		cpu_dump(cpu);
		switch(cpu.code[cpu.ip++]) {
#include "commands.h"
		default:
			return ERRNUM = UNKNOWN_CMD_ERR;
			break;
		}
	}
	
	free(cpu.code);	
	return 0;
}

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
		printf("%02x ", cpu.code[i]);
	}

	printf("\n");
	printf("%*s\n", cpu.ip * 3 + 1, "^");

#if CPU_SLEEP == 1
	getchar();
#endif
}
