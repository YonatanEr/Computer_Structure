#include <stdio.h>
#include <assert.h>
#include "char_array.c"

struct char_matrix
{
    struct char_array* val;
    int len;  
};


struct char_matrix init_char_matrix(char* val, int len){
    struct char_matrix mat;
    mat.len = len;     
    mat.val = (struct char_array*) malloc (len * sizeof(struct char_array));
    assert(mat.val);
    return mat;
}


void print_char_matrix(struct char_matrix mat){
    int i;
    for (i=0; i<mat.len; i++){
        printf("%s\n", mat.val[i].val);
    }
}


void free_char_matrix(struct char_matrix mat){
    int i;
    for (i=0; i<mat.len; i++){
        free(mat.val[i]);
    }
}
