#ifndef _ASM_H
#define _ASM_H

#include <cassert>
#include "onegin.h"
#include "stack.h"
#include "commands.h"
#include <cerrno>

int identcmd(strsize *str);
int writecmd(int cmd, FILE *file, strsize str);
int compile(const char *namein, const char *nameout);
val_t getValue(strsize *str, int valpos);
#endif // _ASM_H
