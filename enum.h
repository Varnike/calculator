#ifndef ENUM_H
#define ENUM_H

#define DEF_CMD(num, name, ...)										\
	CMD_##name=(num),

enum commands {/*
	CMD_PUSH =  1,
	CMD_SUB  = 3,
	CMD_DIV  = 5,
	CMD_ADD  = 2,
	CMD_MUL  = 4,
	CMD_OUT  = 10,
	CMD_HLT  = 11,
	CMD_CRACK= 12
	*/
#include "commands.h"
};
#undef DEF_CMD

const uint32_t NAME    = 0x4E4545;
const uint32_t VERSION = 0x01010001;

struct Hdr {
	uint32_t name;
	uint32_t version;
};
#endif // ENUM_H
