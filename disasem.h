#ifndef DISASEM_H
#define DISASEM_H

#include <cassert>   
#include "onegin.h"  
#include "stack.h"   
#include "commands.h"
#include <cerrno>    


int decompile(const char *namein, const char *nameout);
int identcmd(strsize str);
char *cmdName(int cmd);

#endif
