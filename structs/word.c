#include <stdio.h>
#include <assert.h>

typedef struct word
{
    char* str;
    int len;
} word;


word init_word(int len){
    word arr;
    arr.len = len;     
    arr.str = (char*) malloc ((len+1) * sizeof(char));
    assert(arr.str);
    arr.str[len] = '\0';
    return arr;
}

