#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int LEN = 256;

typedef struct monitor {
	// a 256x256 char matrix containing the pixel 
	int** matrix;
} monitor;


monitor* init_monitor(){
    int i, j;
    monitor* disp = (monitor*) malloc (sizeof(monitor));
    assert(disp);

    disp->matrix = (int**) malloc (LEN*sizeof(int*));
    assert(disp->matrix);

    for (i=0; i<LEN; i++){
        disp->matrix[i] = (int*) malloc (LEN*sizeof(int));
        for (j=0; j<LEN; j++){
            disp->matrix[i][j] = 0;
        }
    }

    return disp;
}


int get_pixel(monitor* disp, int i, int j){
    return disp->matrix[i][j];    
}


void set_pixel(monitor* disp, int i, int j, int p){
    disp->matrix[i][j] = p;
}


void display(monitor* disp){
    int i, j;
    for(i=0; i<LEN; i++){
        for(j=0; j<LEN; j++){
            printf("%d ", get_pixel(disp, i, j));
        }
        printf("\n");
    }
}


void free_monitor(monitor* disp){
    int i, j;
    for (i=0; i<LEN; i++){
        free(disp->matrix[i]);
        disp->matrix = NULL;
    }
    free(disp->matrix);
    disp->matrix = NULL;
    free(disp);
    disp = NULL;
}

