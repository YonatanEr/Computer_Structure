#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#define MONITOR_DIM 256

typedef struct monitor {
	// a 256x256 char matrix containing the pixel 
	int** matrix;
} monitor;


monitor* init_monitor(){
    // returns a monitor
    // allocating 256x256 matrix filled with 0

    int i, j;
    monitor* display = (monitor*) malloc (sizeof(monitor));
    assert(display);

    display->matrix = (int**) malloc (MONITOR_DIM*sizeof(int*));
    assert(display->matrix);

    for (i=0; i<MONITOR_DIM; i++){
        display->matrix[i] = (int*) malloc (MONITOR_DIM*sizeof(int));
        for (j=0; j<MONITOR_DIM; j++){
            display->matrix[i][j] = 0;
        }
    }

    return display;
}


int get_pixel(monitor* disp, int i, int j){
    assert(0<=i && i<MONITOR_DIM);
    assert(0<=j && j<MONITOR_DIM);
    return disp->matrix[i][j];    
}


void set_pixel(monitor* disp, int i, int j, int p){
    assert(0<=i && i<MONITOR_DIM);
    assert(0<=j && j<MONITOR_DIM);
    assert(0<=p && j<256);
    disp->matrix[i][j] = p;
}


void display_monitor(monitor* dispaly){
    int i, j;
    for(i=0; i<MONITOR_DIM; i++){
        for(j=0; j<MONITOR_DIM; j++){
            printf(" %02X ", get_pixel(dispaly, i, j));
        }
        printf("\n");
    }
}


void monitor_to_txt(monitor* dispaly, char* monitor_path){
    FILE* fptr = fopen(monitor_path, "w");
    assert(fptr);
    int i, j;
    for(i=0; i<MONITOR_DIM; i++){
        for(j=0; j<MONITOR_DIM; j++){
            fprintf(fptr, "%02X\n", get_pixel(dispaly, i, j));
        }
    }
	fclose(fptr);
}


void free_monitor(monitor* disp){
    int i, j;
    for (i=0; i<MONITOR_DIM; i++){
        free(disp->matrix[i]);
        disp->matrix[i] = NULL;
    }
    free(disp->matrix);
    disp->matrix = NULL;
    free(disp);
    disp = NULL;
}


