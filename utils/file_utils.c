#include <stdio.h>

int read_file(char* path){
    char line[255];
    char* words;
    FILE *f;
    f = fopen(path, "r");
    while (1){
        if (fgets(line, 255, (FILE*)f) == NULL){
            fclose(f);
            return 0;
        }
        printf("%s", line);
    }
}