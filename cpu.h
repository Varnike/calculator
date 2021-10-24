#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <unistd.h>
#include "stack.h"
#include <cerrno>
#include "onegin.h"
#include "enum.h"
#include "disasem.h"

#define Push(val)								\
{										\
	if (StackPush(&cpu.stack, val))						\
		break;								\
}

#define _POP StackPop(&cpu.stack)

#define CHECK_POP								\
{										\
	if (ERRNUM)								\
		break;								\
}

#define SET_RAM(num)								\
{										\
	cpu.RAM[num] = _POP;							\
	sleep(0.5)								\
}
#define PROCESS_CMD(cmd)							\
{										\
	if(cmd && ERRNUM)							\
		return ERRNUM;							\
}

typedef double value_t;

struct CPU {
	char *code;
	int ip;
	int csize;
	Stack stack;
	val_t RAM[1000];
	/**
	 * Names of registers are strictly defined: 
	 * ax = regs[0] 
	 * bx = regs[1]
	 * cx = regs[2]
	 * dx = regs[3]
	 * fx = regs[4]
	 */
	val_t regs[5];
};
int run_cpu(const char *fname);

int start_cpu(CPU *cpu);
int end_cpu(CPU *cpu);

void cpu_dump(CPU cpu);

#endif //CALCULATOR_H
