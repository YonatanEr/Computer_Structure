#include <stdio.h>
#include <assert.h>

struct int_array
{
    int* val;
    int len;
};


struct int_array init_int_array(int len){
    struct int_array arr;
    arr.len = len;     
    arr.val = (int*) malloc ((len) * sizeof(int));
    assert(arr.val);
    return arr;
}

void print_int_array(struct int_array arr){
    int i;
    for (i=0; i<arr.len; i++){
        printf("%d ", arr.val);
    }
}


void free_int_array(struct int_array arr){
    free(arr.val);
}