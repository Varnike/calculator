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

#define SET_REG(num) cpu.regs[num] = _POP

#define GET_REG(num) cpu.regs[num]

#define IP cpu.ip

#define CODE(num) *(val_t*)(cpu.code + num)

#define CHECK_POP								\
{										\
	if (ERRNUM)								\
		break;								\
}

#define GET_RAM(num) _get_ram(&cpu, num)

#define SET_RAM(num)								\
{										\
	cpu.RAM[num] = _POP;							\
	sleep(0.5);								\
	printf("\tSET RAM[%d] = %lg\n", num, cpu.RAM[num]);			\
}

#define GET_VRAM(num) _get_ram(&cpu, num + MAX_RAM_SIZE)

#define SET_VRAM(num)								\
{                                                                               \
        cpu.RAM[num + MAX_RAM_SIZE] = _POP;					\
        sleep(0.2);                                                             \
        printf("\tSET VRAM[%d] = %lg\n", num, cpu.RAM[num]);			\
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
	val_t RAM[MAX_RAM_SIZE + MAX_VRAM_SIZE];
	/**
	 * Names of registers are strictly defined: 
	 * ax = regs[0] 
	 * bx = regs[1]
	 * cx = regs[2]
	 * dx = regs[3]
	 * fx = regs[4]
	 */
	val_t regs[REGS_CNT];
};

int run_cpu(const char *fname);

int start_cpu(CPU *cpu);
int end_cpu(CPU *cpu);

void ram_dump(CPU *cpu, FILE *file);
void cpu_dump(CPU *cpu, FILE *file);
void reg_dump(CPU *cpu, FILE *file);

val_t _get_ram(CPU *cpu, int num);

#endif //CALCULATOR_H
