#include "cpu.h"
#include <stdio.h> 

int main()
{
	printf("---run cpu---\n");             
	
	run_cpu("asm.bin");
	printf("%s\n", errmsg(ERRNUM));

	if (ERRNUM)
		return -1;

	return 0;

}
