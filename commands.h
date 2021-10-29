DEF_CMD(1, push, 2, 
		{
			Push(ARG);
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
			scanf("%lg", &a);
			Push(a);
		})

DEF_CMD(10, out, 0, 
		{
			val_t a = _POP;
			CHECK_POP;
			printf("\n\n\nout: %lg\n\n\n", a);//
		})

DEF_CMD(11, hlt, 0, 
		{
			goto label_exit;		
		})
DEF_CMD(12, pop, 2,
		{
			ARG = _POP;	
		})
DEF_CMD(13, display, 0,
		{
			for (int h = 0; h != SCREEN_HEIGHT; h++) {
				for (int l = 0; l != SCREEN_LENGHT; l++) {
					printf("%c", ((int)GET_VRAM(l + h * SCREEN_LENGHT) == 0) ? ' ' : '#');
				}
				printf("\n");
			}
		})
DEF_CMD(14, sin, 0,
		{
			Push(sin(_POP));
		})

DEF_CMD(15, cos, 0,
                {
                        Push(cos(_POP));
                })

DEF_CMD(16, sqrt, 0,
		{
			Push(sqrt(_POP));
		})

DEF_JMP_CMD(21, jmp,
		{
			IP = CODE(IP);
		})

DEF_COND_JMP_CMD(22, ja, > )

DEF_COND_JMP_CMD(23, jal, >=)

DEF_COND_JMP_CMD(24, jb,  < )

DEF_COND_JMP_CMD(25, jbl, <=)

DEF_COND_JMP_CMD(26, je,  ==)

DEF_COND_JMP_CMD(27, jne, !=)
	
DEF_JMP_CMD(28, call,
		{
			Push(IP + sizeof(val_t));
			IP = CODE(IP);
		})
DEF_JMP_CMD(29, ret,
		{
			int a = _POP;
			IP = a;
		})
 
