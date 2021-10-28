CC=g++
CFLAGS=-I -Wall 
DEPS = asm.h  config.h  includes/error.h  includes/file.h  includes/input.h  includes/onegin.h  includes/output.h  includes/printval.h  includes/stack.h enum.h disasem.h cpu.h commands.h
OBJ = asm.o  includes/error.o  includes/file.o  includes/input.o includes/onegin.o  includes/output.o  includes/printval.o  includes/stack.o disasem.o cpu.o
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
	xxd -g 1 -c 1 -b asm.bin 
