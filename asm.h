#ifndef _ASM_H
#define _ASM_H

#include <cassert>
#include "includes/onegin.h"
#include "enum.h"
#include <cerrno>
#include "disasem.h"

#define $ printf("\t\t\t---ON LINE %d IN FUNCTION %s---\n", __LINE__, __func__);

#define CHECK_ERR(err_label) {if (ERRNUM) goto err_label;}

const int MAX_LABEL_LEN = 50;

struct _labels {
        char *name;
        int ip;
        int len;
};

struct ASM {
	int ip;
	char *data;
	_labels *label;
	int labelcnt;
};

int compile(const char *namein, const char *nameout);
val_t getValue(char *token, COMMANDS *cmds);
int setReg(char *token);
int setRAM(char *token, COMMANDS *cmds, ASM *code);
int write_bin(ASM *code, const char *nameout);
int process_asm(textBuff *btext, ASM *code, FILE *file);
val_t getLabelIP(const char *name, ASM *label);
int setLabel(char *name, const int len, const int ip, ASM *label);

#endif // _ASM_H
