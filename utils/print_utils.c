#include <stdio.h>

void print_int_array(int* arr, int len){
    int i;
    for (i=0; i<len; i++){
        printf("%d, ", arr[i]);
    }
}


