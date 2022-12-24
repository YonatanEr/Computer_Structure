#include <stdio.h>
#include <assert.h>

char* read_next_line(FILE* f){
    char line[255];
    return fgets(line, 255, (FILE*)f);
}


int main1()
{
    FILE *f;
    char* line;
    char* path = "file.txt";
    f = fopen(path, "r");
    while (1){
        line = read_next_line(f);
        int i=0;
        while(1){
            printf("%c", line[i]);
            i++;
            if (i==4){
                break;
            }
        }
        if (line == NULL){
            break;    
        }

        printf("%s", line);
    }
    fclose(f);
    return 0;    
}




int main(){
    int* x = {1,2,3};
    int* x = {1,2,3};

}
