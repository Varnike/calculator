#ifndef _ASM_H
#define _ASM_H

#include <cassert>
#include "onegin.h"
#include "stack.h"
#include "enum.h"
#include <cerrno>
#include "disasem.h"

int identcmd(strsize *str);
int writecmd(int cmd, strsize *str, Stack *stack, FILE *lstfile = NULL);
int compile(const char *namein, const char *nameout);
val_t getValue(strsize *str, int valpos);
int write_bin(val_t *code, int codesize, const char *nameout);
int setCodeArray(Stack *stack, val_t *arr);
void printLst(int cmd, int pos, int size, val_t val = 0, FILE *file = NULL);
int parse(textBuff *btext, Stack *stack, FILE *file);
#endif // _ASM_H
