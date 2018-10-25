/*
 * Aidan Fike and Russell Parker
 * Comp 175 (Graphics) Homework 1
 * February 11, 2018
 * 
 * VertexList.h - Declaration of a data structure that is used to hold muliple
 * Vertex structs in a 1 dimensional array. The user is able to access and
 * modify elements in this array 
 */

#ifndef VERTEXLIST_H_
#define VERTEXLIST_H_

#include "Vertex.h"
#include <stdlib.h>


class VertexList {

    private: 
        int currSize; //Num Elems in VertexList
        int capacity; //Num Elems vertexList can hold
        Vertex *data; //Array of Vertices themselves

    public:
        VertexList();
        ~VertexList();

        void initVertexList(int);
        void clear();

        void getIndex(int, Vertex *);
        void setIndex(int, Vertex);
};

#endif 
