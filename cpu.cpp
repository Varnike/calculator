#include "cpu.h"

Stack stack = {};

int start()
{
	if (StackCtor(&stack, 8))
		return ERRNUM;
	return NO_ERR;
}

int hlt()
{
	if (StackDtor(&stack))
		return ERRNUM;
	return NO_ERR;
}

int push(val_t val)
{
	printf("push %d\n", val);
	if (StackPush(&stack, val))
		return ERRNUM;
	return NO_ERR;
}

int add()
{
	printf("add\n");
	ERRNUM = 0;
	StackPush(&stack, StackPop(&stack) + StackPop(&stack));
	if (ERRNUM)
		return ERRNUM;
	return NO_ERR;
}

int sub()                                                                                            
{      
	printf("sub\n");	
        ERRNUM = 0;                                                                                  
        StackPush(&stack, StackPop(&stack) - StackPop(&stack));                                      
        if (ERRNUM)
                return ERRNUM;
	return NO_ERR;	
}

int mul()
{
	printf("mul\n");
        ERRNUM = 0;
        StackPush(&stack, StackPop(&stack) * StackPop(&stack));
        if (ERRNUM)
                return ERRNUM;
	return NO_ERR;
}

int div()
{
	printf("div\n");
        ERRNUM = 0;
        StackPush(&stack, StackPop(&stack) / StackPop(&stack));
        if (ERRNUM)
                return ERRNUM;
	return NO_ERR;
}

val_t out()
{
	printf("out : %d\n", StackTop(&stack));
	return StackTop(&stack);
}

int run_cpu(const char *namein)
{            
	int *code = NULL;
	int csize = 0;

	csize = read_bin(namein, &code)/sizeof(int);

	if (csize < 0)
		return ERRNUM;

	int pc = 0;
	
	PROCESS_CMD(start());
	
	while(pc != csize) { 
		cpu_dump(code, csize, pc);		
		switch(code[pc++]) {                           
		case CMD_PUSH:                                 
			PROCESS_CMD(push(code[pc++]));
			break;                                 
		case CMD_ADD:                                  
			PROCESS_CMD(add());                
			break;                                 
		case CMD_SUB:                                  
			PROCESS_CMD(sub());
			break;                                 
		case CMD_MUL:                                  
			PROCESS_CMD(mul());
			break;                                 
		case CMD_DIV:                                  
			PROCESS_CMD(div());
			break;                                 
		case CMD_OUT:                                  
			PROCESS_CMD(out());
			break;                                 
		case CMD_HLT:                                  
			PROCESS_CMD(hlt());
			break;                                 
		default:                                       
			return UNKNOWN_CMD_ERR;
			break;                
		}                                              
	}                                                      
	return 0;
}

void cpu_dump(val_t *code, int size, int pc)
{
	assert(code);
	
	if (size > 255)
		size = 255;

	for (int i = 0; i!= size; i++) {
		printf("%02x ", i);
	}

	printf("\n");

	for (int i = 0; i != size; i++) {
		printf("%02x ", code[i]);
	}

	printf("\n");
	printf("%*s\n", pc * 3 + 1, "^");

#if CPU_SLEEP == 1
	getchar();
#endif
}
