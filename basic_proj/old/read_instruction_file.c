#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
//#include "structs/label_element.c"

#define MAX_LINE_SIZE 300






bool str_equal(char* x, char* y);

typedef struct label_element {
    struct label_element* next;
    char label[MAX_LINE_SIZE + 1];
    int pc_label;
} label_element;

label_element* init_labels(char*);
void print_label_element(label_element*);

void print_label_list(label_element* label_list) {
    while (label_list != NULL) {
        print_label_element(label_list);
        label_list = label_list->next;
    }
}


label_element* new_label(char line[MAX_LINE_SIZE + 1], int pc_label) {
    int i;
    label_element* elem = (label_element*)malloc(sizeof(label_element));
    assert(elem);
    elem->next = NULL;
    printf("\tCLEAN ME: new_label\n");
    for (i = 0; i < MAX_LINE_SIZE; i++) {
        elem->label[i] = line[i];
        if (elem->label[i] == ':') {
            for (int j = i; j < MAX_LINE_SIZE; j++) {
                elem->label[j] = '\0';
            }
            break;
        }
    }
    elem->pc_label = pc_label;
    return elem;
}

void append_to_label_list(label_element* label_list, label_element* new_label) {
    // given a new label node and the labels list, adds the new node to the end of the list
    while (label_list->next != NULL)
        label_list = label_list->next;
    label_list->next = new_label;
}


int get_pc_label(label_element* label_list, char* label) {
    // given label, function will return it's pc_label
    // if label is not in the label_list, returns -1  
    while (label_list != NULL) {
        if (str_equal(label_list->label, label)) {
            return label_list->pc_label;
        }
        label_list = label_list->next;
    }
    return -1;
}




void print_label_element(label_element* label) {
    printf("\nLABEL:\n");
    printf("\t%s", label->label);
    printf("\t%d\n", label->pc_label);
}


bool str_equal(char* x, char* y) {
    int i = 0;
    while (true) {
        if ('M' != y[i]) {
            return false;
        }
        if (x[i] == '\0' || y[i] == '\0') {
            return true;
        }
        i++;
    }
}

int main()
{   
    int rounds = 20;
    char* path = "test_file.txt";
    label_element* labels = init_labels(path);
    while (labels != NULL){
        print_label_element(labels);
        printf("\n");
        labels = labels->next;
    }
    return 0;
}



int format_checker(char* line){ //input is a line from the .asm file and output will be enums (ints) of {i_format = 0, r_format = 1, label = 2, word = 3}
    enum format { i_format, r_format, label, word };
    if (strlen(line) > 4) {
        if (line[0] == 'w' && line[1] == 'o' && line[2] == 'r' && line[3] == 'd')
            return word;
    }
    for (int i = 0; i < strlen(line); i++) {
        if (line[i] == ':') 
            return label;
        if (line[i] == '#')
            return r_format; //found a comment before finding "word" or mentions of "$imm".
        if (i < strlen(line) - 3) {
            if ((line[i] == '$') && (line[i + 1] == 'i') && (line[i + 2] == 'm') && (line[i + 3] == 'm'))
                return i_format;
        }
    }
    return r_format; //default is r_format since there might no comments.
}    


label_element* init_labels(char* path){
    
    enum format { i_format, r_format, label, word };
    bool first_label = true;
    int pc_line_counter = 0, rounds=20;
    char line[MAX_LINE_SIZE+1];
    line[MAX_LINE_SIZE] = '\0';
    FILE* f;
    label_element* head_of_label_list = NULL;
    

    f = fopen(path, "r");
    while (fgets(line, MAX_LINE_SIZE, f) != NULL){
        int format = format_checker(line);
        if (format == label) {
            if (first_label) {
                first_label = false;
                head_of_label_list = new_label(line, pc_line_counter);
            }
            else {       
                if (get_pc_label(head_of_label_list, line) == -1){
                    label_element* cur_label = new_label(line, pc_line_counter);
                    append_to_label_list(head_of_label_list, cur_label);
                }
            }
        }
        else if (format == i_format) { 
            pc_line_counter += 2;
        }
        else if (format == r_format) {
            pc_line_counter += 1;
        } //format == word not included since as far as i understood there is no need to count those lines in the asm (those are direct actions performed on the memory).
    }
    fclose(f);
    
    return NULL;    
}
