#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_LINE_SIZE 301


bool str_equal(char* x, char* y);

typedef struct label_element {
	struct label_element* next;
	char* label;
	int pc_label;
} label_element;

void print_label_element(label_element*);


void print_label_list(label_element* label_list) {
	while (label_list != NULL) {
		print_label_element(label_list);
		label_list = label_list->next;
	}
}


label_element* new_label(char* line, int pc_label) {

	label_element* elem = (label_element*)malloc(sizeof(label_element));
	assert(elem);
	int i, len = strlen(line) + 1;

	elem->label = (char*)malloc(len);
	assert(elem->label);

	elem->next = NULL;

	printf("\tCLEAN ME: new_label\n");

	elem->pc_label = pc_label;
	for (i = 0; i <= len; i++) {
		if (line[i] == ':') {
			elem->label[i] = '\0';
			return elem;
		}
		elem->label[i] = line[i];
	}

	return elem;
}

void append_to_label_list(label_element* head_of_label_list, label_element* new_label) {
	// given a new label node and the labels list, adds the new node to the end of the list
	while (head_of_label_list->next != NULL)
		head_of_label_list = head_of_label_list->next;
	head_of_label_list->next = new_label;
}
void print_label_element(label_element* label) {
	printf("\nLABEL:\n");
	printf("\t%s", label->label);
	printf("\t%d\n", label->pc_label);
}

int get_pc_label(label_element* label_list, char* label) {
	// given label, function will return it's pc_label
	// if label is not in the label_list, returns -1  
	//print_label_element(label_list);
	//printf("%s\n",label);
	while (label_list != NULL) {
		if (str_equal(label_list->label, label)) {
			return label_list->pc_label;
		}
		label_list = label_list->next;
	}
	return -1;
}





bool str_equal(char* x, char* y) {
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