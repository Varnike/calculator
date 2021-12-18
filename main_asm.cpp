#include <stdio.h>
#include "asm.h"

int main()
{
	printf("---compilation---\n");         
	if (compile("_asm_in.txt", "asm.bin")) {
        	printf("%s\n", errmsg(ERRNUM));
		return -1;
	}

	return 0;
}
