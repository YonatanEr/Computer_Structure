#include <stdio.h>
#include <assert.h>
#include "utils/file_utils.c"

enum opcode{add, sub, mul, and, or, xor, sll, sra, srl, beq, bne, blt, bgt, ble, bge, jal, lw, sw, reti, in, out, halt};

 
int main()
{
    read_file("file.txt");
    enum opcode op;
    return 0;    
}