DEF_CMD(1, push, 2, 
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
DEF_CMD(2,  add, 0, 
		{
			val_t a = _POP;
			val_t b = _POP;
			CHECK_POP;
			Push(a + b);	
		})
DEF_CMD(3,  sub, 0, 
		{
			val_t a = _POP;
			val_t b = _POP;
			CHECK_POP;
			Push(a - b);
		})
DEF_CMD(4,  mul, 0, 
		{
			val_t a = _POP;
			val_t b = _POP;
			CHECK_POP;
			Push(a * b);
		})
DEF_CMD(5,  div, 0, 
		{
			val_t a = _POP;
			val_t b = _POP;
			CHECK_POP;
			Push(a / b);

		})
DEF_CMD(6,   in, 0, 
		{
			val_t a = 0;
			scanf("%d", &a);
			Push(a);
		})

DEF_CMD(10, out, 0, 
		{
			val_t a = _POP;
			CHECK_POP;
			printf("out: %d\n", a);
		})

DEF_CMD(11, hlt, 0, 
		{
			goto label_exit;		
		})
DEF_CMD(12, pop, 2,
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
DEF_JMP_CMD(21, jmp,
		{
			cpu.ip = (int)*(val_t*)(cpu.code + cpu.ip);
		})
DEF_JMP_CMD(22, ja,
		{
			val_t a = _POP;
			val_t b = _POP;
			if (a > b)
				cpu.ip = (int)*(val_t*)(cpu.code + cpu.ip);
			else 
				cpu.ip += sizeof(val_t);
		})
DEF_JMP_CMD(23, jal,
                {
                        val_t a = _POP;
                        val_t b = _POP;
                        if (a >= b)
                                cpu.ip = (int)*(val_t*)(cpu.code + cpu.ip);
			else
			        cpu.ip += sizeof(val_t);

                })
DEF_JMP_CMD(24, jb,
                {
                        val_t a = _POP;
                        val_t b = _POP;
                        if (a < b)
                                cpu.ip = (int)*(val_t*)(cpu.code + cpu.ip);
			else
			        cpu.ip += sizeof(val_t);
		})
DEF_JMP_CMD(25, jbl,
                {
                        val_t a = _POP;
                        val_t b = _POP;
                        if (a <= b)
                                cpu.ip = (int)*(val_t*)(cpu.code + cpu.ip);
			else
			        cpu.ip += sizeof(val_t);
                })
DEF_JMP_CMD(26, je,
                {
                        val_t a = _POP;
                        val_t b = _POP;
                        if (a == b)
                                cpu.ip = (int)*(val_t*)(cpu.code + cpu.ip);
			else
			        cpu.ip += sizeof(val_t);
                })
DEF_JMP_CMD(27, jne,
                {
                        val_t a = _POP;
                        val_t b = _POP;
                        if (a != b)
                                cpu.ip = (int)*(val_t*)(cpu.code + cpu.ip);
			else
			        cpu.ip += sizeof(val_t);

                })
DEF_JMP_CMD(28, call,
		{
			Push(cpu.ip + sizeof(val_t));
			cpu.ip = (int)*(val_t*)(cpu.code + cpu.ip);
		})
DEF_JMP_CMD(29, ret,
		{
			int a = _POP;
			cpu.ip = a;
		})

