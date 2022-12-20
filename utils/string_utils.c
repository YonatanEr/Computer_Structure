#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

int string_len(char* );
char* substring(char*, int, int );
bool str_equal(char* , char* );


// returns str[i:j]
// e.g substring("abcde", 1, 3) -> "bcd"
char* substring(char* str, int i, int j){
    int ind;
    char* sub_str;
    sub_str = (char*) malloc ( (j-i+2) * sizeof(char) );
    assert(sub_str);
    for(ind=i; ind<=j+1; ind++){
        sub_str[ind-i] = str[ind]; 
    }
    return sub_str;
}

bool str_equal(char* x, char* y){
    int i;
    while (true){
        if (x[i] != y[i]){
            return false;
        }
        if (x[i] == "\n"){
            return true;
        }
        i++;
    }
}

int string_len(char* str){
    int i=0;
    while (true){
        if (str[i]!="\n"){
            i++;
        }
        return i;
    }
}

