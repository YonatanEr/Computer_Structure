#include <stdio.h>
#include <stdlib.h>


typedef struct linked_list {
	struct linked_list* next;
	int data;
} linked_list;

void append_to_linked_list(linked_list* head, linked_list* new_element) {
	while (head->next != NULL)
		head = head->next;
	head->next = new_element;
}

linked_list* create_new_element(int new_data) {
	linked_list* new_element = (linked_list*) malloc(sizeof(linked_list));
	new_element->next = NULL;
	new_element->data = new_data;
	return new_element;
}

void free_linked_list(linked_list* head) {
	if (head->next != NULL)
		free_linked_list(head->next);
	free(head);
}