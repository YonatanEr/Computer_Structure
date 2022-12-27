#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct label_element {
	struct label_element* next;
	char* label;
	int pc_label;
} label_element;

void print_label_element(label_element*);

label_element* new_label(char* label, int pc_label) {
	label_element* header = (label_element*)malloc(sizeof(label_element));
	if (header != NULL) {
		header->label = (char*) malloc (strlen(label) + 1); 
		printf("\nCLEAN ME: new_label\n\n");
		strcpy(header->label, label);
		header->label[strlen(label)] = '\0';
		header->pc_label = pc_label;
		header->next = NULL;
	}
	else {
		printf("ERROR, could not allocate memory to new_label_list.");
		exit(0);
	}
	return header;
}

void append_to_label_list(label_element* label_list, label_element* new_label) {
	while (label_list->next != NULL)
		label_list = label_list->next;
	label_list->next = new_label;
}


int get_pc_label(label_element* label_list, char* label){
	// given label, function will return it's pc_label
	// if label is not in the label_list, returns -1  
	while (label_list->next != NULL) {
		if (strcmp(label_list->label, label)){
			return label_list->pc_label;
		}
		label_list = label_list->next;
	}
	return -1;
}


void print_label_element(label_element* label){
	printf("label = %s", label->label);
	printf("pc_label = %d\n", label->pc_label);
}