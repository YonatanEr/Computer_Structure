#include <stdio.h>
#include <assert.h>

struct char_array
{
    char* val;
    int len;
};


struct char_array init_char_array(int len){
    struct char_array arr;
    arr.len = len;     
    arr.val = (char*) malloc ((len+1) * sizeof(char));
    assert(arr.val);
    arr.val[len] = '\0';
    return arr;
}


void print_char_array(struct char_array arr){
    printf("%s", arr.val);
}


void free_char_array(struct char_array arr){
    free(arr.val);
}