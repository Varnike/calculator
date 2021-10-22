#ifndef DISASEM_H
#define DISASEM_H

#include <cassert>   
#include "onegin.h"  
#include "stack.h"   
#include "enum.h"
#include <cerrno>    


int read_bin(const char *namein, int **code);
int decompile(const char *namein, const char *nameout);
char *cmdName(int cmd);
void processDecomp(int *code, int len, FILE *file);

int checkHdr(FILE *file);
#endif
