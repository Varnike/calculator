#include <stdio.h>
#include "asm.h"

int main()
{
        printf("---decompilation---\n");         
        if (decompile("asm.bin", "output.txt")) {
                printf("%s\n", errmsg(ERRNUM));
                return -1;
        }
	system("cat output.txt");

	printf("---decompiled---\n");


        return 0;
}
