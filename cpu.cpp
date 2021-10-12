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
	return StackTop(&stack);
}

int run_cpu(const char *namein)
{            
        FILE *file_in  = open_file(namein, "r");

        assert(file_in); 
                       
        int cmd   = 0;
        val_t val = 0;
	
	start();

        while (fscanf(file_in, "%d", &cmd) > 0) { // STOPPED HERE
		switch (cmd) {
		case CMD_PUSH:
			fscanf(file_in, "%d", &val);
			PROCESS_CMD(push(val));
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
			ERRNUM = 0;
			val = out();

			if (ERRNUM)
				return ERRNUM;
			
			printf("out : %d\n", val);

			break;
		case CMD_HLT:
			PROCESS_CMD(hlt());
			printf("hlt\n");
			return 0;
			break;
		default:
			return UNKNOWN_CMD_ERR;
		}
	}

	hlt();
        return 0;	
}
