//File: main.c
//Author: Giovanni Busdon
//ID: SM3000678

//libraries
#include <stdio.h>
#include <sys/stat.h>
#include "ppm.h"
#include "scene.h"

int main(){

    //gesù bastardo
    char filename[30];
    char render[30];
    int render_h,render_w;

    printf("Nome file scena ");
    scanf("%29s", filename);
    printf("Nome file render ");
    scanf("%29s", render);
    printf("Altezza render ");
    scanf("%d", &render_h);
    printf("Larghezza render ");
    scanf("%d", &render_w);


    FILE* fd = fopen(filename, 'r');

    if(fd == NULL){
        printf("Errore apertura file scena\n");
        return 1;
    }
    
    struct stat sbuf;
    stat(filename, &sbuf);

    WS scene;

    int Err = fscanf(fd, "VP %f %f %f\nBG %d %d %d\nOBJ_N %d\n", 
        scene->viewport,scene->viewport + 1, scene->viewport + 2, scene->background, 
        scene->background + 1, scene->background + 2, scene->object_num);

    if( Err =! 2 ){

            //se non trovo tutte le info termino il programma
            
            printf("File non leggibile");
            return 2;
        }
    
    pVec * Vectors = getunitVectors(render_w, render_h, scene->viewport[0], scene->viewport[1], scene->viewport[2]);
    Obj* Spheres = getSpheres(fd, scene->object_num);

    if (Spheres == NULL){
        return 3;
    }

    pCol * pixels = getColours(Spheres, scene->object_num, Vectors, render_w, render_h, scene->background);

    int Err = createFile(pixels, render_w, render_h);

    if(Err != 0){
        return 4;
    }
    
    fclose(fd); 
    free(Vectors);
    free(Spheres);
    free(pixels);  

    return 0;
}