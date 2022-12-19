#include "header.h"

int format_checker(char*);

enum format {i_format, r_format, label, word};


int main()
{
    int pc_line_counter = 0;
    char* line = "placeholder";
    char* path = "file.txt";
    FILE* f;
    int first_label = 1;
    //label_element label_list;



    f = fopen(path, "r");
    line = read_next_line(f);
    while (!(line == NULL)){
        int format = format_checker(line);

        if (format == label) {
            if (first_label)

            label_struct = (int*)malloc(10 * sizeof(int)); //using dynamic sized array for label_pc_array.
            //label.append(l); NEED TO IMPLEMENT APPEND TO BOTH THE LABEL ARRAY AND THE PC POINTERS OF IT.
            pc_line_counter += 1;
        }
        else if (format == i_format) {
            pc_line_counter += 2;
        }
        else if (format == r_format) {
            pc_line_counter += 1;
        } //format == word not included since as far as i understood there is not need to count those lines in the asm (those are direct actions performed on the memory).

        //words = init_char_matrix()
        printf("%s\n", line);
    }
    fclose(f);
    return 0;    
}


int format_checker(char* line){ //input is a line from the .asm file and output will be enums (ints) of {i_format = 0, r_format = 1, label = 2, word = 3}
    int i = 0 , len = strlen(line);
    for (; i < len - 4; i++) {
        if (line[i] == ":") 
            return label;

        if (line[i] == '#')
            return r_format; //found a comment before finding "word" or mentions of $imm.

        if (line[0] == 'w' && line[1] == 'o' && line[2] == 'r' && line[3] == 'd')
            return word;

        if ((line[i] == '$') && (line[i + 1] == 'i') && (line[i+2] == 'm') && (line[i+3] == 'm'))
            return i_format;
    }
    return r_format; //default is r_format since there might no comments.
}

void append_label(int current_size, int max_size, char** label_array, int* label_pc_array, char* label, int* first_label) {
    
    if (label_pc_array == NULL) {
        printf("Error, could not allocate memory for label_pc_array.\n");
        exit(0);
    }

        
}