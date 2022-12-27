#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


int find_next_char_occurence(char* line, char x, int start, int len){
    int i;
    for (i=start; i<len; i++){
        if (line[i] == x){
            return i;
        }
    }
    return -1;
}



