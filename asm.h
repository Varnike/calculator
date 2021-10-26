#ifndef _ASM_H
#define _ASM_H

#include <cassert>
#include "onegin.h"
#include "enum.h"
#include <cerrno>
#include "disasem.h"

#define $ printf("\t\t\t---ON LINE %d IN FUNCTION %s---\n", __LINE__, __func__);
const int MAX_LABEL_LEN = 20;

struct ASM {
	int ip;
	char *data;
};

int compile(const char *namein, const char *nameout);
val_t getValue(char *token, COMMANDS *cmds);
int isReg(char *token);
int isRAM(char *token, COMMANDS *cmds, ASM *code);
int write_bin(ASM *code, const char *nameout);
void printLst(int cmd, int pos, int size, val_t val = 0, FILE *file = NULL);
int process_asm(textBuff *btext, ASM *code, FILE *file);
void parse(textBuff *btext);

//TODO on release: static!
#endif // _ASM_H
