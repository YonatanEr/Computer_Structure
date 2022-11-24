#include <stdio.h>
#include <assert.h>

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
