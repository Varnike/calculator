#ifndef ENUM_H
#define ENUM_H

#include <cstdint>

#define DEF_CMD(num, name, ...)									\
	CMD_##name=(num),

#define DEF_JMP_CMD(num, name, ...)								\
	CMD_##name=(num),

#define DEF_COND_JMP_CMD(num, name, ...)							\
	CMD_##name=(num),
enum commands {
#include "commands.h"
};
#undef DEF_CMD
#undef DEF_COND_JMP_CMD
#undef DEF_JMP_CMD

const uint32_t NAME    = 0x4E4545;
const uint32_t VERSION = 0x01020012;
const int REGS_CNT = 5;
const int MAX_RAM_SIZE  = 100;
/*
 * for screen 20x20
 */
const int MAX_VRAM_SIZE = 400;
const int SCREEN_LENGHT = 20;
const int SCREEN_HEIGHT = 20;
const int MAX_LABELS_CNT = 20;

struct COMMANDS{
	unsigned ram : 1;
	unsigned reg : 1;
	unsigned imm : 1;
	unsigned cmd : 5;
};

struct Hdr {
	uint32_t name;
	uint32_t version;
};
#endif // ENUM_H
