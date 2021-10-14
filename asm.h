#ifndef _ASM_H
#define _ASM_H

#include <cassert>
#include "onegin.h"
#include "stack.h"
#include "commands.h"
#include <cerrno>

int identcmd(strsize *str);
int writecmd(int cmd, strsize *str, Stack *stack);
int compile(const char *namein, const char *nameout);
val_t getValue(strsize *str, int valpos);
int write_bin(val_t *code, int codesize, const char *nameout);
int setCodeArray(Stack *stack, val_t *arr);
#endif // _ASM_H
