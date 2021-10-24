#ifndef _ASM_H
#define _ASM_H

#include <cassert>
#include "onegin.h"
#include "enum.h"
#include <cerrno>
#include "disasem.h"


struct ASM {
	int ip;
	char *data;
};

int compile(const char *namein, const char *nameout);
val_t getValue(char *token, COMMANDS *cmds);
int isReg(char *token);
int write_bin(ASM *code, const char *nameout);
void printLst(int cmd, int pos, int size, val_t val = 0, FILE *file = NULL);
int process_asm(textBuff *btext, ASM *code, FILE *file);
void parse(textBuff *btext);
#endif // _ASM_H
