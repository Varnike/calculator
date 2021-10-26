#ifndef DISASEM_H
#define DISASEM_H

#include <cassert>   
#include "onegin.h"  
#include "stack.h"   
#include "enum.h"
#include <cerrno>    

struct DISASM {
	int ip;    
	char *data;
	FILE *file_in;
	FILE *file_out;
	int codesize;
};

int read_bin(const char *namein, char **code);
int decompile(const char *namein, const char *nameout);
char *cmdName(int cmd);
void processDecomp(DISASM *dasm);

int checkHdr(FILE *file);
#endif
