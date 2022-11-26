#include <stdio.h>
#include <assert.h>
#include "utils/file_utils.c"

enum opcode{add, sub, mul, and, or, xor, sll, sra, srl, beq, bne, blt, bgt, ble, bge, jal, lw, sw, reti, in, out, halt};

int main()
{
    FILE *f;
    char* line;
    char* path = "file.txt";
    f = fopen(path, "r");
    while (1){
        line = read_next_line(f);

        if (line == NULL){
            break;    
        }

        printf("%s", line);
    }
    fclose(f);
    return 0;    
}
