# CPU
---
Simple cpu emulator with assembler and custom list commands, which can be changed.  [`Onegin`](https://github.com/Varnike/onegin.git) and ['stack'](https://github.com/Varnike/stack.git) libraries are used.
## Compiling 
To compile assembler run:
````sh
make asm
````
To compile cpu run:
````sh
make cpu
````
## Usage
Two programes are presented: assembler and processor emulator. To obtain a binary file for the processor, you must write a program in the assembler language and run:
````sh
./asm
````
You can then run the program by running cpu:
````sh
./cpu
````
## CPU commands
* 'push reg/imm/mem' - push reg/imm/mem to stack
* 'pop reg/imm/mem' - pop value from stack to reg/mem
* 'j? label' - jump to lablel if condition is satisfied
* 'jmp label' - jump to label
* 'call func' - push return address to stack and jump to func label
* 'ret' - jump to address from stack
* 'hlt' - stop cpu
* 'label:' - label
* 'in' - read number from console
* 'out' - print number to console

## Programm example
This is an example of program which calculates the factorial of the entered number:
````sh
in
pop ax
push 1
pop bx
push 1
pop cx

call factorial
push cx
out
hlt

factorial:
        push ax
        push bx
        jal end_function

        push cx
        push bx
        push 1
        add
        mul
        pop cx
        push bx
        push 1
        add
        pop bx
        call factorial
end_function:

````
