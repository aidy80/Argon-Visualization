/*
 * Aidan Fike and Russell Parker
 * Comp 175 (Graphics) Homework 1
 * February 11, 2018
 * 
 * VertexList.cpp - Implementation of a data structure that is used to hold 
 * muliple Vertex structs in a 1 dimensional array. The user is able to access 
 * and modify elements in this array 
 */


#include "VertexList.h"
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <stdexcept>

using namespace std;

const string noMem = "Out of Memory\n";
const string outOfBounds = "Out of Bounds\n";

// Default constructor 
VertexList::VertexList() {
    data = NULL;
}

//Destructor to free all memory allocated for the vertexList structure
VertexList::~VertexList() {
    if (data != NULL) {
        free(data);
    }
}

// Function to manually clear all data in the vertexList
void VertexList::clear() {
    free(data);
    currSize = 0;
    capacity = 0;
}

// malloc space for an array of n Vertices 
void VertexList::initVertexList(int n) {
    capacity = n;
    currSize = 0;
    data = (Vertex *)malloc(n * sizeof(Vertex));
    if (!data) {
        throw runtime_error(noMem); //Throw error if out of memory
    }
}

/* Get data for a vertex at index i of the array. Information is placed
 * in the Vertex *v passed in */
void VertexList::getIndex(int i, Vertex *v) {
    v->point = data[i].point;
    v->normal = data[i].normal;
}

// Add information about the vertex at index i in the array. Information 
// from the parameter Vertex v is copied to the vertex at this index
void VertexList::setIndex(int i, Vertex v) {
    if (i >= capacity) {
        throw logic_error(outOfBounds); //The user is unable to access indexes 
                                        //larger than the initial size they
                                        //allocated
    }
    data[i].point = v.point;
    data[i].normal = v.normal;
}
