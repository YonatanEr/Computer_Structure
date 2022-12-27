#include <stdio.h>

char* read_next_line(FILE* f){
    char line[MAX_LINE_SIZE+1];
    return fgets(line, MAX_LINE_SIZE+1, (FILE*)f);
}

/*

void read_line_number(char* filename, int line_number, char* result) { //returns a string from a specific line number in a requested file.
    FILE* fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("Error, Failed opening %s", filename);
        exit(0);
    }
    int i = 0;
    do { //while i != line_number i will keep loading new lines to &result in order to increase fptr to the next line.
        fscanf(fptr, "%s", result);
        i++;
    } while (i != line_number);
    fclose(fptr);
}

*/


/*

void overwrite_line_number(char* filename, int line_number, char* data) {
    int n_confirm = rename(filename, "delete_me.txt"); //returns 0 if successful
    if (n_confirm) {
        printf("Error, Failed renaming %s to delete_me.txt", filename);
        exit(0);
    }

    FILE* fptr_old = fopen("delete_me.txt", "r");
    FILE* fptr_new = fopen(filename, "w");
    if (fptr_old == NULL) {
        printf("Error, Failed opening delete_me.txt");
        exit(0);
    }
    if (fptr_new == NULL) {
        printf("Error, Failed opening %s", filename);
        exit(0);
    }

<<<<<<< Updated upstream
    char* temp_data;
    for (int i = 0; ; i++) {
=======
    char temp_data[6]; //THIS MAKES THE FUNCTION ONLY VALID FOR FILES WITH LINES OF SIZE 5 CHARS ONLY, DONT WANT TO USE MALLOC.
    for (int i = 0; feof(fptr_old); i++) { //feof returns 0 only if reached EOF
>>>>>>> Stashed changes
        if (i != line_number) { //reloads the data lines from delete_me to the new file.
            fscanf(fptr_old,"%s", &temp_data);
            fprintf(fptr_new, "%s\n", temp_data);
        }
        else {
            fscanf(fptr_old,"%s", &temp_data); //fscanf automaticlly increases line count by 1 so need to add it here.
            fprintf(fptr_new, "%s\n", data);
        }
    }

    fclose(fptr_old);
    fclose(fptr_new);

    n_confirm = remove("delete_me.txt"); //returns 0 if successful
    if (n_confirm) {
        printf("Error, Failed deleting delete_me.txt");
        exit(0);
    }
}

*/
