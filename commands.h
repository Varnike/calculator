DEF_CMD(1, PUSH, 1, 
		{
			printf("num = %d\n", cpu.code[cpu.ip]);
			Push(cpu.code[cpu.ip]);
		})
DEF_CMD(2,  ADD, 0, 
		{
			val_t a = StackPop(&stack);
			val_t b = StackPop(&stack);
			CHECK_POP;
			Push(a + b);	
		})
DEF_CMD(3,  SUB, 0, 
		{
			val_t a = StackPop(&stack);
			val_t b = StackPop(&stack);
			CHECK_POP;
			Push(a-b);
		})
DEF_CMD(4,  MUL, 0, 
		{
			val_t a = StackPop(&stack);
			val_t b = StackPop(&stack);
			CHECK_POP;
			Push(a * b);
		})
DEF_CMD(5,  DIV, 0, 
		{
			val_t a = StackPop(&stack);
			val_t b = StackPop(&stack);
			CHECK_POP;
			Push(a / b);

		})
DEF_CMD(6,   IN, 0, 
		{
			val_t a = 0;
			scanf("%d", &a);
			Push(a);
		})

DEF_CMD(10, OUT, 0, 
		{
			val_t a = StackPop(&stack);
			CHECK_POP;
			printf("out: %d\n", a);
		})

DEF_CMD(11, HLT, 0, 
		{
			int err = StackDtor(&stack);
			if (err)
				assert("DIDNT DIE\n");
		})
