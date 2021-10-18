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
	printf("hlt\n");
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
	return StackPop(&stack);
}

int run_cpu(const char *namein)
{            
	CPU cpu = {};	

	int size = read_bin(namein, &cpu.code);

	if (size < 0)
		return ERRNUM;
	
	cpu.csize = (size - sizeof(Hdr))/sizeof(val_t);

	if (cpu.csize < 0)
		return ERRNUM = INVALID_BIN_DATA;

	int is_unvalid = checkVersion((Hdr*)cpu.code);
	
	if (is_unvalid)
		return ERRNUM;

	setCodePtr(&cpu.code);
	
	PROCESS_CMD(start());
	
	while(cpu.pc != cpu.csize) { 
		cpu_dump(cpu);
		switch(cpu.code[cpu.pc++]) {                           
		case CMD_PUSH:                                 
			PROCESS_CMD(push(cpu.code[cpu.pc++]));
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

	setRealPtr(&cpu.code);

	free(cpu.code);	
	return 0;
}

void cpu_dump(CPU cpu)
{
	assert(cpu.code);
	
	if (cpu.csize > 255)
		cpu.csize = 255;

	for (int i = 0; i!= cpu.csize; i++) {
		printf("%02x ", i);
	}

	printf("\n");

	for (int i = 0; i != cpu.csize; i++) {
		printf("%02x ", cpu.code[i]);
	}

	printf("\n");
	printf("%*s\n", cpu.pc * 3 + 1, "^");

#if CPU_SLEEP == 1
	getchar();
#endif
}
