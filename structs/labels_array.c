#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "utils/string_utils.c"

labels_array extend_labels_array(labels_array);
labels_array init_labels_array(max_len);


typedef struct labels_array
{
    char* labels_names; 
    int* labels_line;
    int len;
} labels_array;


labels_array init_labels_array(int max_len){
    word* names = (word*) malloc ((max_len) * sizeof(word));
    assert(names);
    int_array lines = init_int_array(max_len);
}


int find_label_index(labels_array arr, char* label){
    int i;
    for (i=0; i<arr.labels_names->len; i++){
        if (str_equal(arr.labels_names->str, label)){
            return i;
        }
    }
    return -1;
}


int add_label(labels_array arr, char* label, int line){
    if (arr.len == arr.max_len){
        extend_labels_array(arr);
    }

}


labels_array extend_labels_array(labels_array arr){
    int i;
    labels_array brr = init_labels_array(2*arr.max_len);
    for (i=0; i<arr.len; i++){
        brr.labels_line.nums[i] = arr.labels_line.nums[i];
        brr.labels_names[i].len = arr.labels_names[i].len;
        
    }
}

