	#include <stdio.h>
	#include <stdbool.h>
	#include <stdlib.h>
	#include <string.h>
	#include <assert.h>

typedef struct label_element {
	// a linked list which will serve us to store labels 
	
	struct label_element* next;
	char* label;
	int pc_label;
} label_element;


label_element* new_label(char*, int);
void append_to_label_list(label_element*, label_element*);
void print_label_element(label_element*);
void print_label_list(label_element* );
bool str_equal(char*, char*);
void free_label_list(label_element*);


label_element* new_label(char* line, int pc_label) {
    // given a string, integer
    // returns a label_element pointer with the attributes set

    int i, len = strlen(line);

    label_element* elem = (label_element*) malloc (sizeof(label_element));
    assert(elem);

    elem->label = (char*) malloc (len);
    assert(elem->label);

    elem->pc_label = pc_label;

    elem->next = NULL;

    for (i = 0; i < len; i++) {
        elem->label[i] = line[i];
        if (line[i] == ':'){
            elem->label[i] = '\0';
            return elem;
        }
    }
    return elem;
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


void append_to_label_list(label_element* head_of_label_list, label_element* new_label) {
	// given head_of_label_list, new_label, append the new_label to the end of the linked list

	while (head_of_label_list->next != NULL)
		head_of_label_list = head_of_label_list->next;
	head_of_label_list->next = new_label;
}


void print_label_element(label_element* elem) {
	// given a label_element pointer prints it

	if (elem == NULL){
		printf("\tLABEL = NULL\n");
	}
	else{
		printf("\nLABEL:\n");
		printf("\t %s", elem->label);
		printf("\t %d \n", elem->pc_label);
	}
}


void print_label_list(label_element* label_list) {
	// given a label_element pointer
	// prints the element and the elements which follows it 
	
	while (label_list != NULL) {
		print_label_element(label_list);
		label_list = label_list->next;
	}
}


bool str_equal(char* x, char* y) {
	// returns true iff x==y
	
	int i = 0;
	while (true) {
		if (x[i] != y[i]) {
			return false;
		}
		if (x[i] == '\0' || y[i] == '\0') {
			return true;
		}
		i++;
	}
}


void free_label_list(label_element* elem){
	// frees the memory allocated in the label_element

	if (elem != NULL){
		free_label_list(elem->next);
		free(elem->label);
		elem->label = NULL;
		free(elem);
		elem = NULL;
	}
}

