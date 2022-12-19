#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

typedef struct int_array
{
    int* num;
    int len;
} int_array;


int_array init_int_array(int len){
    int_array arr;
    arr.len = len;     
    arr.num = (int*) malloc ((len) * sizeof(int));
    assert(arr.num);
    return arr;
}

void print_int_array(int_array arr){
    int i;
    for (i=0; i<arr.len; i++){
        printf("%d ", arr.num);
    }
}

