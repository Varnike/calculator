#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "stack.h"
#include "input.h"
#include <cerrno>
#include "onegin.h"
#include "commands.h"
#include "disasem.h"

#define PROCESS_CMD(cmd)							\
{										\
	if(cmd && ERRNUM)							\
		return ERRNUM;							\
}

typedef double value_t;

extern Stack stack;

int run_cpu(const char *fname);
int push(value_t);
int sub();
int div();
int add();
int mul();
int start();
int hlt();
val_t out();
void cpu_dump(val_t *code, int size, int pp);

#endif //CALCULATOR_H
