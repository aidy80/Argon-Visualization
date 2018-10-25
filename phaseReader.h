#ifndef PHASEREADER_H
#define PHASEREADER_H

#include <string>
#include <vector>
#include "Algebra.h"
#include "SceneData.h"

using namespace std;

struct scene_object {
    ScenePrimitive scenePrim;
    Matrix object_transformation;
};

struct frame {
    double **pos;
    double **vel;
};

class PhaseReader {
    public: 
        PhaseReader();
        ~PhaseReader();

        vector<struct frame> getFrames();
        vector<struct scene_object> getObjects();
        int readIn(string filename);
        int getNumFrames();
        void clear(struct frame);
        void clearAll();
        void print(struct frame);
        void update(int, int, int);
        int isEmpty();
        int getFrameNum();

    private:
        int numAtoms;
        int numDimensions;
        int frameNum;
        int numFrames;
        int unInitialized;
        int currDirection;

        void transform_frame_to_object(int, int);
        
        vector<struct frame> frames;
        vector<struct scene_object> object_vector;
};

#endif
