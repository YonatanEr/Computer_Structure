#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define MAX_LINE_SIZE 300


bool str_equal(char* x, char* y);

typedef struct label_element {
	struct label_element* next;
	char label[MAX_LINE_SIZE+1];
	int pc_label;
} label_element;

void print_label_element(label_element* );

void print_label_list(label_element* label_list){
	while (label_list != NULL){
		print_label_element(label_list);
		label_list = label_list->next;
	}
}


label_element* new_label(char line[MAX_LINE_SIZE+1], int pc_label) {
	int i;
	label_element* elem = (label_element*) malloc (sizeof(label_element));
	assert(elem);
	elem->next = NULL;
	printf("\tCLEAN ME: new_label\n");
	for (i=0; i<MAX_LINE_SIZE; i++){
		elem->label[i] = line[i];
		if (elem->label[i] == ':'){
			for (int j=i; j<MAX_LINE_SIZE; j++){
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


int get_pc_label(label_element* label_list, char* label){
	// given label, function will return it's pc_label
	// if label is not in the label_list, returns -1  
	while (label_list != NULL) {
		if (str_equal(label_list->label, label)){
			return label_list->pc_label;
		}
		label_list = label_list->next;
	}
	return -1;
}




void print_label_element(label_element* label){
	printf("\nLABEL:\n");
	printf("\t%s", label->label);
	printf("\t%d\n", label->pc_label);
}


bool str_equal(char* x, char* y){
    int i=0;
    while (true){
        if (x[i] != y[i]) {
            return false;
        }
        if (x[i] == '\0' || y[i] == '\0'){
            return true;
        }
        i++;
    }
}