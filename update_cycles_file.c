#include "../header.h"

void update_cycles_file(int cycles, FILE* fptr) {
	fptr = fopen("cycles.txt", "w");

	if (fptr == NULL) {
		printf("Error, couldn't open memin.txt\n");
		exit(0);
	}
	fprintf(fptr, "%d", cycles);
}