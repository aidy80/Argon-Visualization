/*
 * Aidan Fike and Russell Parker
 * Comp 175 (Graphics) Homework 1
 * February 11, 2018
 * 
 * Vertex.h - A struct used to hold information about a vertex's position and
 *            normal in cartesian space
 */


#ifndef VERTEX_H
#define VERTEX_H

#include "Algebra.h"

/* struct containg information about the position and 
 * normal for a given vertex */
struct Vertex {
    Point point;
    Vector normal;

    Vertex() {} //Default constructor

    Vertex(Point newPoint, Vector newNormal) { //Custom constructor to 
                                               //initialize the struct with a 
                                               //passed point and normal 
                                               //combination of the user's 
                                               //choosing
        point = newPoint;
        normal = newNormal;
    }
};

typedef struct Vertex Vertex;

#endif
