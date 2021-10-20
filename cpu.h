#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "stack.h"
#include <cerrno>
#include "onegin.h"
#include "enum.h"
#include "disasem.h"

#define Push(val)								\
{										\
	if (StackPush(&stack, val))						\
		/*DO SMTH*/;							\
}

#define CHECK_POP								\
{										\
	if (ERRNUM)								\
		/*DO SMTH*/;							\
}

#define PROCESS_CMD(cmd)							\
{										\
	if(cmd && ERRNUM)							\
		return ERRNUM;							\
}

typedef double value_t;

extern Stack stack;

struct CPU {
	val_t *code;
	int ip;
	int csize;
	//TODO Stack stack;
	//For future
	val_t ax;
	val_t bx;
	val_t cx;
	val_t dx;
};
int run_cpu(const char *fname);
int push(value_t);
int sub();
int div();
int add();
int mul();
int start();
int hlt();
val_t out();
void cpu_dump(CPU cpu);

#endif //CALCULATOR_H
