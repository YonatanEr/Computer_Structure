#include "header.h"

enum format {i_format, r_format, label, word};

int format_checker(char*);
label_element* init_labels(char*);

int main()
{   
    int rounds = 20;
    char* path = "test_file.txt";
    /*
    label_element* labels = init_labels(path);
    while (labels != NULL){
        print_label_element(labels);
    }
    */
   
    FILE* f;
    f = fopen(path, "r");
    char* line = (char*) malloc (MAX_LINE_SIZE+1);
    line[MAX_LINE_SIZE] = '\0';
    printf("\nCLEAN ME: main\n\n");
    line = read_next_line(f);
    line = read_next_line(f);
    while (line != NULL){
        printf("LINE = %s\n", line);
        line = read_next_line(f);
    }
    fclose(f);
    return 0;
}

int format_checker(char* line){ //input is a line from the .asm file and output will be enums (ints) of {i_format = 0, r_format = 1, label = 2, word = 3}
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
    int pc_line_counter = 0, first_label = 1, rounds=20;
    char* line;
    FILE* f;
    label_element* head_of_label_list;
    f = fopen(path, "r");
    line = read_next_line(f);
    while (line != NULL){
        printf("LINE = %s\n", line);
        int format = format_checker(line);
        if (format == label) {
            if (first_label) {
                first_label = 0;
                head_of_label_list = new_label(line, pc_line_counter);
            }
            else {        
                if (get_pc_label(head_of_label_list, line) == -1){
                    append_to_label_list(head_of_label_list, new_label(line, pc_line_counter));
                }
            }
        }
        else if (format == i_format) { 
            pc_line_counter += 2;
        }
        else if (format == r_format) {
            pc_line_counter += 1;
        } //format == word not included since as far as i understood there is no need to count those lines in the asm (those are direct actions performed on the memory).

        line = read_next_line(f);
    }
    fclose(f);
    return head_of_label_list;    
}



















