#ifndef _ASM_H
#define _ASM_H

#include <cassert>
#include "onegin.h"
#include "stack.h"
#include "enum.h"
#include <cerrno>
#include "disasem.h"

int compile(const char *namein, const char *nameout);
val_t getValue(char *token);
int write_bin(val_t *code, int codesize, const char *nameout);
void printLst(int cmd, int pos, int size, val_t val = 0, FILE *file = NULL);
int process_asm(textBuff *btext, Stack *stack, FILE *file);
void parse(textBuff *btext);
#endif // _ASM_H
