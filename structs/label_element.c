#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct label_element {
	struct label_element* next;
	char* label;
	int pc_label;
} label_element;

label_element* new_label(char* label, int pc_label) {
	label_element* header = (label_element*)malloc(sizeof(label_element));
	if (header != NULL) {
		strcpy(header->label, label);
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
	label_element* current_label = label_list;
	while (current_label->next != NULL)
		current_label = current_label->next;
	current_label->next = new_label;
}

