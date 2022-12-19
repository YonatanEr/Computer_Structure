#include "header.h"

bool is_label(char*);
bool is_iformat(char*);



int main()
{
    int pc_line_counter = 0;
    char* line;
    char* path = "file.txt";
    char_array words;

    FILE *f;
    
    f = fopen(path, "r");
    while (true){
        line = read_next_line(f);
        
        if (line == NULL){
            break;    
        }

        if (is_label(line)) {
            label.append(l);
            continue;
        }
        
        else if (is_iformat(line)) {
            pc_line_counter += 2;
        }

        else is_iformat(line) {
            pc_line_counter += 2;
        }

        words = init_char_matrix()
        printf("%s\n", line);
    }
    fclose(f);
    return 0;    
}




bool is_label(char* line){
    int i, len = string_len(line);
    for (i=0; i<len; i++){
        if (line[i] == "#"){
            return false;
        }
        if (line[i]==":"){
            return true;
        }
        i++;
    }
    return false;
}


bool is_iformat(char* line){
    int i, len = string_len(line);
    for (i=0; i<len-4; i++){
        if (line[i] == "#" || line[i+1] == "#" || line[i+2] == "#" || line[i+3] == "#"){
            return false;
        }
        if (line[i]=="$"){
            if (line[i+1]=="i"){
                if (line[i]=="m"){
                    if (line[i]=="m"){
                        return true;
                    }
                }
            }
        }
        i++;
    }
    return false;
}