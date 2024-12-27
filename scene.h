//File: scene.h
//Author: Giovanni Busdon
//ID: SM3000678

#ifndef _SCENE
#define _SCENE

//viene usato AoS invece che SoA perché a ogni accesso
//si usano tutte le info della struttura
struct pointVector {
    float floatVal[3];
};
typedef struct pointVector * pVec;

struct __attribute__((packed)) pixelColor {
    int intVal[3];
};
typedef struct pixelColor * pCol;

struct WorkSpace {
    float viewport[3];
    int background[3];
    int object_num;
};
typedef struct WorkSpace * WS;

struct SphereObject {
    pVec centre;
    float radius;
    pCol colour;
};
typedef struct SphereObject * Obj;


pVec* getunitVectors(int render_w, int render_h, float width, float height, float dist);

Obj* getSpheres(FILE* fd, int object_num);

pCol* getColours(Obj* Spheres, int object_num, pVec* vectors, int render_w, int render_h, int * background);

#endif