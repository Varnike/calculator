#include <stdio.h>
#include "asm.h"
#include "disasem.h"
#include "onegin.h"
#include "cpu.h"
int main(int argc, char **argv)
{
	if (argc == 2) {
		switch(*(argv[1])) {
		case 'r':
			printf("---run cpu---\n");
			if (run_cpu("asm.bin"));
				printf("%s\n", errmsg(ERRNUM));
			break;
		case 'c':
			printf("---compilation---\n");
			compile("_asm_in.txt", "asm.bin");
			break;
		case 'd':
			printf("---decompilation---\n");
			decompile("asm.bin", "input.txt");
			break;
		default:
			printf("bad parameter!\n");
			return -1;
			break;
		}
		return 0;
	}
	if (ERRNUM)
		printf("%s\n", errmsg(ERRNUM));
	printf("no parameters!\n");
	return 0;
}
