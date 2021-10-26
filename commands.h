DEF_CMD(1, PUSH, 2, 
		{	
			if (cmds.ram == 1) {
				int a = *(val_t*)(cpu.code + cpu.ip);                 
				if (cmds.reg == 1) {
					int reg_data = GET_REG(a);
					if (cmds.imm == 1) {
						int b = *(val_t*)(cpu.code + cpu.ip + sizeof(val_t));
						Push(GET_RAM(b + reg_data));
						cpu.ip += sizeof(val_t);
					} else {
						Push(GET_RAM(reg_data));
					}
				} else if (cmds.imm == 1) {
					Push(GET_RAM(a));
				}
			} else if (cmds.reg == 0) {
                                Push(*(val_t *)(cpu.code + cpu.ip));
                        } else {
				int a = *(val_t*)(cpu.code + cpu.ip);
                                Push(GET_REG(a));
				}
			cpu.ip += sizeof(val_t);
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
			scanf("%d", &a);
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
			goto label_exit;		
		})
DEF_CMD(12, POP, 2,
		{
			int a = *(val_t*)(cpu.code + cpu.ip);
			if (cmds.ram == 1) {
				if (cmds.reg == 1) {
					int b = *(val_t*)(cpu.code + cpu.ip + sizeof(val_t));
					int reg_data = GET_REG(a);
					if (cmds.imm == 1) {
						SET_RAM(b + reg_data);
						cpu.ip += 2 * sizeof(val_t);
					} else {
						SET_RAM(reg_data);
						cpu.ip += sizeof(val_t);	
					}
				} else if (cmds.imm == 1) {
					SET_RAM(a);
					cpu.ip += sizeof(val_t);
				}
			} else if (cmds.reg == 1) {
				SET_REG(a);
				cpu.ip += sizeof(val_t);
			} else  {
				_POP;
				cpu.ip += sizeof(val_t);
			}
		})
