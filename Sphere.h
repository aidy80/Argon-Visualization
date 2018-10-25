/*
 * Aidan Fike and Russell Parker
 * Comp 175 (Graphics) Homework 1
 * February 11, 2018
 * 
 * Sphere.h - Declaration and implementation of a class that is able to render 
 * a sphere onto the user's screen, as well as normals at each vertex of the
 * sphere.
 */

#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
 
#define RADIUS 0.5

class Sphere : public Shape {
public:
    // Create a default sphere - assume no user input for m_segmentsX/Y is
    // given yet
    Sphere() {
        curr_segmentsX = 10;
        curr_segmentsY = 10;
    };
	~Sphere() {};


    /*
     * This function will go through all the vertices calculated for the sphere
     * and draw triangles between them in such a way that a fully filled sphere
     * is rendered. If either m_segmentX or m_segment& changed since the last
     * time this function was called, it will recalculate the vertices needed
     * to draw the cube using the new values of m_segmentsX/Y
     */
    void draw() {
        //Recalculate vertices for the cube if m_segmentsX or Y changed since
        //the last iteration of this function
        if (m_segmentsX != curr_segmentsX || m_segmentsY != curr_segmentsY) {
            barrelVertices.clear();
            capVertices.clear();
            barrelVertices.initVertexList(m_segmentsX * (m_segmentsY - 1));
            capVertices.initVertexList((m_segmentsX + 2) * 2);
            calcVertices();
            curr_segmentsX = m_segmentsX;
            curr_segmentsY = m_segmentsY;
        }

        // Loop through the vertices of the sphere and create the corresponding
        // triangles needed to fill the surface excluding the caps of the
        // sphere (The "north and south poles").
        for (int y = 0; y < m_segmentsY - 2; y++) {
            glBegin(GL_TRIANGLE_STRIP);
            for (int x = 0; x < m_segmentsX; x++) {
                Vertex currVertex;
                barrelVertices.getIndex(y * (m_segmentsX) + x, &currVertex);
                glNormal3f(currVertex.normal[0], 
                           currVertex.normal[1], 
                           currVertex.normal[2]);
                glVertex3f(currVertex.point[0],
                           currVertex.point[1],
                           currVertex.point[2]);

                /*
                 * After drawing a point, draw the point "underneath" it before
                 * returning ot the original level. This is needed to draw a
                 * triangle with height
                 */
                barrelVertices.getIndex(((y + 1) * (m_segmentsX)) + x, 
                                                                 &currVertex);
                glNormal3f(currVertex.normal[0], 
                           currVertex.normal[1], 
                           currVertex.normal[2]);
                glVertex3f(currVertex.point[0],
                           currVertex.point[1],
                           currVertex.point[2]);

                /* At the final iteration of this loop, it is necessary to
                 * redraw the first points drawn. This is done to close the
                 * loop of the current stack. */
                if (x == m_segmentsX - 1) {
                    barrelVertices.getIndex((y * (m_segmentsX)), &currVertex);
                    glNormal3f(currVertex.normal[0], 
                               currVertex.normal[1], 
                               currVertex.normal[2]);
                    glVertex3f(currVertex.point[0],
                               currVertex.point[1],
                               currVertex.point[2]);

                    barrelVertices.getIndex(((y + 1) * (m_segmentsX)), 
                                                                 &currVertex);
                    glNormal3f(currVertex.normal[0], 
                               currVertex.normal[1], 
                               currVertex.normal[2]);
                    glVertex3f(currVertex.point[0],
                               currVertex.point[1],
                               currVertex.point[2]);
    
                }
            }  
            glEnd();
        }

        /* 
         * Draw the "caps" (the "north and south poles") of the sphere. This is
         * done using triangle fanning - first specifying the center of the
         * cap, then the points surrounding it.
         */
        for (int i = 0; i < 2; i++) {
            glBegin(GL_TRIANGLE_FAN);
            for (int j = 0; j < m_segmentsX + 2; j++) {
                Vertex currVertex;
                capVertices.getIndex((i * (m_segmentsX + 2)) + j, &currVertex);
                glNormal3f(currVertex.normal[0],
                           currVertex.normal[1],
                           currVertex.normal[2]);
                glVertex3f(currVertex.point[0],
                           currVertex.point[1],
                           currVertex.point[2]);
            }
            glEnd();
        }
	};

    /*
     * This function will go through all vertices calculate for the sphere and
     * draw a line from each of them in the direction of the vertex's normal.
     * If either m_segmentX or m_segment?Y changed since the last time this
     * function was called, it will recalculate the vertices needed to draw the
     * cube using the new values of m_segmentsX/Y
     */
	void drawNormal() {
        //Recalcuate vertices for the cube if m_segmentsX or Y changed since
        //the last iteration
        if (m_segmentsX != curr_segmentsX || m_segmentsY != curr_segmentsY) {
            barrelVertices.clear();
            capVertices.clear();
            barrelVertices.initVertexList(m_segmentsX * (m_segmentsY - 1));
            capVertices.initVertexList((m_segmentsX + 2) * 2);
            calcVertices();
            curr_segmentsX = m_segmentsX;
            curr_segmentsY = m_segmentsY;
        }

        //Go through each vertex of the sphere excluding the sphere's "caps" 
        //and draw the appropriate normals at those points
        glBegin(GL_LINES);
        for(int i = 0; i < (m_segmentsX + 2) * 2; i++) {
            Vertex currVertex;
            capVertices.getIndex(i, &currVertex);

            glVertex3f(currVertex.point[0],
                       currVertex.point[1],
                       currVertex.point[2]);
            glVertex3f(currVertex.point[0] + (NORMAL_LEN * currVertex.normal[0]),
                       currVertex.point[1] + (NORMAL_LEN * currVertex.normal[1]),
                       currVertex.point[2] + (NORMAL_LEN * currVertex.normal[2]));
        }
        glEnd();

        //Go through each vertex of the sphere's cap and draw the appropriate
        //normals at those points
        glBegin(GL_LINES);
        for (int i = 0; i < m_segmentsX * (m_segmentsY - 1); i++) {
            Vertex currVertex;
            barrelVertices.getIndex(i, &currVertex);

            glVertex3f(currVertex.point[0],
                       currVertex.point[1],
                       currVertex.point[2]);
            glVertex3f(currVertex.point[0] + (NORMAL_LEN * currVertex.normal[0]),
                       currVertex.point[1] + (NORMAL_LEN * currVertex.normal[1]),
                       currVertex.point[2] + (NORMAL_LEN * currVertex.normal[2]));

        }
        glEnd();
	};




private:
    VertexList barrelVertices, capVertices;

    //These variables hold m_segmentX and m_segmentY values that 
    //were used to calculate the current vertices in vertexList
    int curr_segmentsX, curr_segmentsY;

    /*
     * This function will use the parameters m_segmentsX and m_segmentsY to
     * calculate equidistant vertices on the surface of the sphere with a
     * radius of 0.5. m_segmentsX/Y determine the angular distance betwen each
     * of the vertices calculated. Normal vectors corresponding to each of
     * these vertices will be calculated and stored alongside the position data
     * itself in the barrelVertices and capVertices structures
     */
    void calcVertices() {
        unsigned i = 0; //Record the number of vertices that have been added

        for (int y = 1; y < m_segmentsY; y++) {
            float phi = PI * (y / (float)m_segmentsY);

            for (int x = 0; x < m_segmentsX; x++) {
                float theta = x * (2 * PI) / (float)m_segmentsX;

                //Using m_segmentsY to calculate increments of phi, and
                //m_segmentsX to calculate increments of theta, use the
                //conversion from cylindrical coordinates to cartesian
                //coordinates to find the appriopriate position and normal of 
                //the vertex in cartesian space
                Point p(RADIUS * sin(phi) * cos(theta),
                        RADIUS * cos(phi),
                        RADIUS * sin(phi) * sin(theta));
                Vector v(sin(phi) * cos(theta),
                         cos(phi),
                         sin(phi) * sin(theta));

                Vertex vert(p, v);
                barrelVertices.setIndex(i, vert);
                i++;
            }
        }

        // Calculate vertices for the cap of the sphere

        //Vertex at the very top of the sphere
        i = 0;
        Point p(0, 0.5, 0);
        Vector v(0, 1, 0);
        Vertex vert(p, v);
        capVertices.setIndex(i, vert);
        i++;

        //Vertices surrounding the top of sphere
        for (int x = 0; x < m_segmentsX; x++) {
            barrelVertices.getIndex(x, &vert);
            capVertices.setIndex(i, vert);
            i++;

            //Duplicate the initial vertex to complete a circle around the top
            //of the sphere
            if (x == m_segmentsX - 1) {
                barrelVertices.getIndex(0, &vert);
                vert.normal = v;
                capVertices.setIndex(i, vert);
                i++;
            }
        }

        //Vertex at the very bottom of the sphere
        p[1] = -0.5;
        v[1] = -1.0;
        vert.point = p;
        vert.normal = v;
        capVertices.setIndex(i, vert);
        i++;

        //Vertices surrounding the bottom of the sphere
        for (int x = 0; x < m_segmentsX; x++) {
            barrelVertices.getIndex(x + (m_segmentsX * (m_segmentsY - 2)), &vert);
            vert.normal = v;
            capVertices.setIndex(i, vert);
            i++;

            //Duplicate the initial vertex to complete a circle around the
            //bottom of the sphere
            if (x == m_segmentsX - 1) {
                barrelVertices.getIndex(m_segmentsX *(m_segmentsY - 2), &vert);
                vert.normal = v;
                capVertices.setIndex(i, vert);
                i++;
            }
        }
    }
};

#endif
