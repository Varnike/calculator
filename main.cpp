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
			run_cpu("_asm_out.txt");
			break;
		case 'c':
			printf("---compilation---\n");
			compile("_asm_in.txt", "_asm_out.txt");
			break;
		case 'd':
			printf("---decompilation---\n");
			decompile("_asm_out.txt", "input.txt");
			break;
		default:
			printf("bad parameter!\n");
			return -1;
			break;
		}
		return 0;
	}
	printf("no parameters!\n");
	return 0;
}
