//File: scene.c
//Author: Giovanni Busdon
//ID: SM3000678

//libraries
#include <omp.h>
#include <stdio.h>
#include <math.h>
#include "scene.h"


pVec* getunitVectors(int render_w, int render_h, float width, float height, float dist){

    /*data la risoluzione dell'immagine e la dimensione del viewport crea
    un insieme di vettori unitari che rappresentano le posizioni dei pixel*/

    pVec* Vectors = malloc(sizeof(struct pointVector)*render_h*render_w);

    #pragma omp parallel for collapse(2)
    for(int i = 0; i < render_w; i++){
        for(int j = 0; j < render_h; j++){
            
            float x = width/(render_w-1)*i - width/2;
            float y = height/(render_h-1)*j - height/2;
            float z = dist;

            //vettori unitari
            float norm = sqrt(pow(width/(render_w-1)*i - width/2,2) + 
                        pow(height/(render_h-1)*j - height/2,2) + pow(dist,2));

            Vectors[render_w*j + i]->floatVal[0] = x/norm;
            Vectors[render_w*j + i]->floatVal[1] = y/norm;
            Vectors[render_w*j + i]->floatVal[2] = z/norm;

        }
    }    

    return Vectors;
}

Obj* getSpheres(FILE* fd, int object_num){
    /*dal file descrive la scena estrae la descrizione delle sfere*/

    Obj* Spheres= malloc(sizeof(struct SphereObject)*object_num);

    #pragma omp parallel for
    for(int i = 0; i < object_num; i++){
        
        //rinomino puntatori per leggibilità
        float * centre = Spheres[i]->centre->floatVal;
        int * color = Spheres[i]->colour->intVal;
        
        //potrebbe non funziare!!!
        int Err = fscanf(fd, "S %f %f %f %f %d %d %d\n", centre, centre + 1, centre + 2,
         Spheres[i]->radius, color, color + 1, color + 2);
        
        if( Err =! 2 ){
            /*se non trovo tutte le info termino il programma.
            Non considero le righe oltre il numero di oggetti indicato*/
            
            printf("File non leggibile");
            return NULL;
        }
    }

    return Spheres;
}

float intersect(Obj Sphere, pVec vector){
    /*trova il minor fattore moltiplicativo per cui la
    sfera interseca la retta con vettore direzionale quello dato.
    Ritorna INFINITY se non intersecano*/


    float a = pow(vector->floatVal[0],2) + pow(vector->floatVal[1],2) + pow(vector->floatVal[2],2);

    float b = -2*(vector->floatVal[0]*Sphere->centre->floatVal[0]
        + vector->floatVal[1]*Sphere->centre->floatVal[1] + vector->floatVal[1]*Sphere->centre->floatVal[1]);

    float c = pow(Sphere->centre->floatVal[0],2) + pow(Sphere->centre->floatVal[1],2) + pow(Sphere->centre->floatVal[2],2) 
        - pow(Sphere->radius,2);

    float discriminant = pow(b,2) - 4*a*c;

    if(discriminant < 0){
        return INFINITY;
    }
    if(discriminant > 0){
        float sol1 = abs( (-b + sqrt(pow(b,2)- 4*a*c))/(2*a) );
        float sol2 = abs( (-b - sqrt(pow(b,2)- 4*a*c))/(2*a) );
        return (sol1 < sol2) ? sol1 : sol2;
    }
    else{
        return abs(-b/(2*a));
    }
}

pCol* getColours(Obj* Spheres, int object_num, pVec* vectors, int render_w, int render_h, int * background){
    /*Dato l'insieme degli oggetti di scena trova il colore
    del pixel nella direzione del vettore corrispondente*/

    pCol * pixels = malloc(sizeof(struct pixelColor)*render_w*render_h);

    #pragma omp parallel for collapse(2)
    for(int i = 0; i < render_w; i++){
        for(int j = 0; j < render_h; j++){

            int nearest;
            float min_ij = INFINITY;

            for(int k = 0; k < object_num; k++){

                float t = intersect(Spheres[k], vectors[render_w*j + i]);
                
                //provare branchless (o solo scritto più compatto)
                if(t < min_ij){
                    min_ij = t;
                    nearest = k;
                }

            }

            if(nearest != NULL){
                pixels[render_w*j + i] = Spheres[nearest]->colour;
            }
            else{
                pixels[render_w*j + i] = background;
            }


        }
    }
    return pixels;
}