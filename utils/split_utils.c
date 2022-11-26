#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "slice_utils.c"


int find_next_char_occurence(char*, char, int, int);


char* coma_split(char* line, int len){
    int word_ind, i, j;
    char** words;
    words = (char**) malloc (4 * sizeof(char*));
    assert(words);
    i=0;
    for (word_ind=0; word_ind<=4; word_ind++){
        j = find_next_char_occurence(line, ',', i, len);
        words[word_ind] = substring(line, i, j-1);                      // We don't want to copy ','
        i = j+1;
    }
    return words;
}

int find_next_char_occurence(char* line, char x, int start, int len){
    int i;
    for (i=start; i<len; i++){
        if (line[i] == x){
            return i;
        }
    }
    return -1;
}

