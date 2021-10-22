#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "asm.h"
#include "disasem.h"
#include "cpu.h"

const char *BIN_NAME = "asm.bin";
const char *ASM_NAME = "_asm_in.txt";


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
			if (compile("_asm_in.txt", "asm.bin"))
				printf("%s\n", errmsg(ERRNUM));
			break;
		case 'd':
			printf("---decompilation---\n");
			if(decompile("asm.bin", "input.txt"))
				printf("%s\n", errmsg(ERRNUM));
			break;
		default:
			break;
		}
		return 0;
	} else {
		struct stat bin_stat = {};
		struct stat asm_stat = {};

		stat(BIN_NAME, &bin_stat);
		stat(ASM_NAME, &asm_stat);
		
		if (difftime(bin_stat.st_mtim.tv_sec, asm_stat.st_mtim.tv_sec) < 0)
			system("./qasm");

		system("./qcpu");
	}

	if (ERRNUM)
		printf("%s\n", errmsg(ERRNUM));
	
	return 0;
}
