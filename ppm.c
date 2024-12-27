//File: ppm.c
//Author: Giovanni Busdon
//ID: SM3000678


//libraries
#include <omp.h>
#include <sys/mman.h>
#include <stdio.h>
#include "ppm.h"

int createFile(pCol * pixels, int render_w, int render_h){

    FILE *fd = fopen("render.ppm", "w");

    if(fd == NULL){
        printf("Errore nella crezione render\n");
        return;
    }

    fprintf("P6\n%d %d\n%d\n", render_w, render_h, 255);

    int size = (render_h*render_w*3)*sizeof(int);
    int* datamap = mmap((void*)0, size, PROT_WRITE, MAP_SHARED, fileno(fd), 0); 

    if(datamap == MAP_FAILED){
        printf("Errore nel mapping");
        return -1;
    }

    #pragma omp parallel for
    for(int i = 0; i < render_w*render_h; i ++){

        datamap[3*i]     = pixels[i]->intVal[0];
        datamap[3*i + 1] = pixels[i]->intVal[1];
        datamap[3*i + 2] = pixels[i]->intVal[2];

    }
    
    munmap(datamap, size);

    fclose(fd);

    return 0;
}