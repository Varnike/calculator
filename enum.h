#ifndef ENUM_H
#define ENUM_H

#define DEF_CMD(num, name, ...)										\
	CMD_##name=(num),

enum commands {
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
