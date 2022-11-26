#include <stdio.h>

char* read_next_line(FILE* f){
    char line[255];
    return fgets(line, 255, (FILE*)f);
}