CC=g++ -std=c++14
CFLAGS=-I -Werror -fmax-errors=1 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++0x-compat -Wc++11-compat -Wc++14-compat -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlarger-than=8192 -Wlogical-op -Wmissing-declarations -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstack-usage=8192 -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -fcheck-new -fsized-deallocation -fstack-check -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -fPIE -fsanitize=address -fsanitize=alignment -fsanitize=bool -fsanitize=bounds -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=integer-divide-by-zero -fsanitize=leak -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=object-size -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=shift -fsanitize=signed-integer-overflow -fsanitize=undefined -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -lm -pie
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
