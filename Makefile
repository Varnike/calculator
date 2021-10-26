CC=g++
CFLAGS=-I -Wall
DEPS = asm.h  config.h  error.h  file.h  input.h  onegin.h  output.h  printval.h  stack.h enum.h disasem.h cpu.h commands.h
OBJ = asm.o  error.o  file.o  input.o onegin.o  output.o  printval.o  stack.o disasem.o cpu.o
MAIN = main.o
CPU  = main_cpu.o
ASM  = main_asm.o
DASM = main_disasm.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ) $(MAIN)
	$(CC) -o $@ $^ $(CFLAGS)

cpu : $(OBJ) $(CPU)
	$(CC) -o $@ $^ $(CFLAGS)

asm : $(OBJ) $(ASM)
	$(CC) -o $@ $^ $(CFLAGS)
dasm: $(OBJ) $(DASM)
	$(CC) -o $@ $^ $(CFLAGS)

clean :
	rm *.o
bin:
	xxd -g 1 -c 4 asm.bin
	xxd -g 1 -c 1 asm.bin 
