//TODO reverce ???
DEF_CMD(1, PUSH, 1, 
		{
			printf("num = %d\n", *(int *)(cpu.code + cpu.ip));
			val_t val = *(val_t *)(cpu.code + cpu.ip);

			if (cmds.ram == 1) {
				if (val == 0) {
					perror("Segmentation fault\n");
					break;
				}
				Push(cpu.RAM[val]);
			}

			if (cmds.reg == 0) {
                                Push(val);
                        }
                        else {
                                Push(val);//TODO ??? if double ???
                        }
		})
DEF_CMD(2,  ADD, 0, 
		{
			val_t a = _POP;
			val_t b = _POP;
			CHECK_POP;
			Push(a + b);	
		})
DEF_CMD(3,  SUB, 0, 
		{
			val_t a = _POP;
			val_t b = _POP;
			CHECK_POP;
			Push(a - b);
		})
DEF_CMD(4,  MUL, 0, 
		{
			val_t a = _POP;
			val_t b = _POP;
			CHECK_POP;
			Push(a * b);
		})
DEF_CMD(5,  DIV, 0, 
		{
			val_t a = _POP;
			val_t b = _POP;
			CHECK_POP;
			Push(a / b);

		})
DEF_CMD(6,   IN, 0, 
		{
			val_t a = 0;
			scanf("%d", &a);//TODO change if data type changes
			Push(a);
		})

DEF_CMD(10, OUT, 0, 
		{
			val_t a = _POP;
			CHECK_POP;
			printf("out: %d\n", a);
		})

DEF_CMD(11, HLT, 0, 
		{
			break;		
		})
DEF_CMD(12, POP, 1,
		{
			if (cmds.reg == 1)
				cpu.regs[*(val_t *)(cpu.code + cpu.ip)] = _POP;
			if (cmds.ram == 1)
				cpu.RAM[*(int *)(cpu.code + cpu.ip)] = _POP;
		})
