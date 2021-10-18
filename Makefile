CC=g++
CFLAGS=-I -Wall
DEPS = asm.h  config.h  error.h  file.h  input.h  onegin.h  output.h  printval.h  stack.h enum.h disasem.h cpu.h commands.h
OBJ = main.o asm.o  error.o  file.o  input.o onegin.o  output.o  printval.o  stack.o disasem.o cpu.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

stack: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean :
	rm *.o
asm:
	xxd -g 1 -c 4 asm.bin
 
