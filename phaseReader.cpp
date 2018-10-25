#include "phaseReader.h"
#include <iostream>
#include <fstream>

PhaseReader::PhaseReader() {
    numAtoms = 864;
    numDimensions = 3;
    frameNum = 0;
    currDirection = 1;
    numFrames = 240;
    unInitialized = 1;
}

PhaseReader::~PhaseReader() {
    clearAll();
}

void PhaseReader::clearAll() {
    for (int x = 0; x < 2; x++) {
        struct frame currFrame = frames.back();
        frames.pop_back();
        clear(currFrame);     
    }

    unInitialized = 1;
}

int PhaseReader::isEmpty() {
    return unInitialized;
}

void PhaseReader::clear(struct frame currFrame) {
    if (currFrame.pos != NULL) {
        for (int i = 0; i < numAtoms; i++) {
            if(currFrame.pos[i] != NULL) {
                delete []currFrame.pos[i];
            }
        }
        delete []currFrame.pos;
    }
    if (currFrame.vel != NULL) {
        for (int i = 0; i < numAtoms; i++) {
            if(currFrame.vel[i] != NULL) {
                delete []currFrame.vel[i];
            }
        }
        delete []currFrame.vel;
    }
}

vector<struct frame> PhaseReader::getFrames() {
    return frames;
}

int PhaseReader::readIn(string filename) {
    ifstream phaseFile;  
    phaseFile.open(filename);

    if (!phaseFile.is_open()) {
        cerr << "UNABLE TO READ PHASEFILE\n";
        return -1; 
    }

    unInitialized = 0;
    
    for (int x = 0; x < numFrames; x++) {
        string garbage;

        struct frame newFrame;

        newFrame.vel = new double*[numAtoms];
        newFrame.pos = new double*[numAtoms];

        for (int i = 0; i < numAtoms; i++) {
            newFrame.vel[i] = new double[numDimensions];
            newFrame.pos[i] = new double[numDimensions];
        }
        
        for (int i = 0; i < numAtoms; i++) {
            phaseFile >> garbage;
            phaseFile >> garbage;
            phaseFile >> garbage;
            for (int j = 0; j < numDimensions; j++) {
                phaseFile >> newFrame.pos[i][j];
            }
            for (int j = 0; j < numDimensions; j++) {
                phaseFile >> newFrame.vel[i][j];
            }
        }

        frames.push_back(newFrame);
    }
    return 1;
}

int PhaseReader::getNumFrames() {
    return numFrames;
}

vector<struct scene_object> PhaseReader::getObjects() {
    return object_vector;
}

vector<struct scene_velocity> PhaseReader::getVectors() {
	return velocity_vector;
}

void PhaseReader::print(struct frame currFrame) {
    cout << "Velocity\n";
    for (int i = 0; i < numAtoms; i++) {
        cout << "(";
        for (int j = 0; j < numDimensions; j++) {
            cout << currFrame.vel[i][j] << ",";
        }
        cout << ")\n";
    } 
    cout << "Position\n";
    for (int i = 0; i < numAtoms; i++) {
        cout << "(";
        for (int j = 0; j < numDimensions; j++) {
            cout << currFrame.pos[i][j] << ",";
        }
        cout << ")\n";
    } 
}

void PhaseReader::update(int velocity, int speed, int expand) {
    if (frameNum >= numFrames - 1) {
        currDirection = 0;
    } else if (frameNum <= 0) {
        currDirection = 1; 
    }


    transform_frame_to_object(velocity, expand);

    
    if(currDirection) {
        frameNum++;
    } else {
        frameNum--;
    }
}

void PhaseReader::transform_frame_to_object(int velocity, int expand) {
    if(!object_vector.empty()) {
        object_vector.clear();
    }
	if (!velocity_vector.empty()) {
		velocity_vector.clear();
	}


    for(int i = 0; i < numAtoms; i++) {
        struct scene_object newSceneObj;
        Matrix scale(0.01, 0, 0, 0,
		0, 0.01, 0, 0,
		0, 0, 0.01, 0,
		0, 0, 0, 1);

        Matrix translate(1, 0, 0, frames.at(frameNum).pos[i][0] / 3.47,
		0, 1, 0, frames.at(frameNum).pos[i][1] / 3.47,
		0, 0, 1, frames.at(frameNum).pos[i][2] / 3.47,
		0, 0, 0, 1);

        newSceneObj.object_transformation = translate * scale;
        newSceneObj.scenePrim.type = SHAPE_SPHERE;
        newSceneObj.scenePrim.material.shininess = 0;
        newSceneObj.scenePrim.material.textureMap = NULL;
        newSceneObj.scenePrim.material.bumpMap = NULL;
        newSceneObj.scenePrim.material.blend = 0.5;

        SceneColor color;
        color.r = .3; 
        color.g = .5;
        color.b = .5;
        color.a = .2;
        newSceneObj.scenePrim.material.cAmbient = color;
        newSceneObj.scenePrim.material.cDiffuse = color;
        newSceneObj.scenePrim.material.cSpecular = color;
        newSceneObj.scenePrim.material.cReflective = color;
        newSceneObj.scenePrim.material.cEmissive = color;

        object_vector.push_back(newSceneObj);

        if (expand) {
            unsigned aMask = 1;
            unsigned bMask = 2;
            unsigned cMask = 4;
            for (unsigned j = 1; j < 8; j++) {
                unsigned a = j & aMask;
                unsigned b = j & bMask;
                unsigned c = j & cMask;
                b = b >> 1;
                c = c >> 2;
                struct scene_object newObj;  
                /*
                for (int x = 0; x > (int)(-1 - a); x--) {
                    for (int y = 0; y > (int)(-1 - b); y--) {
                        for (int z = 0; z > (int)(-1 - c); z--) {
                            Matrix translate1(1, 0, 0, frames.at(frameNum).pos[i][0] / 3.47 + x,
                                              0, 1, 0, frames.at(frameNum).pos[i][1] / 3.47 + y,
                                              0, 0, 1, frames.at(frameNum).pos[i][2] / 3.47 + z,
                                              0, 0, 0, 1);
                            newObj.object_transformation = translate1 * scale;
                            newObj.scenePrim.type = SHAPE_SPHERE;
                            newObj.scenePrim.material.shininess = 0;
                            newObj.scenePrim.material.textureMap = NULL;
                            newObj.scenePrim.material.bumpMap = NULL;
                            newObj.scenePrim.material.blend = 0.5;
                            newObj.scenePrim.material.cAmbient = color;
                            newObj.scenePrim.material.cDiffuse = color;
                            newObj.scenePrim.material.cSpecular = color;
                            newObj.scenePrim.material.cReflective = color;
                            newObj.scenePrim.material.cEmissive = color;

                            object_vector.push_back(newObj);
                        }
                    }
                }
                */
                
                Matrix translate0(1, 0, 0, frames.at(frameNum).pos[i][0] / 3.47 + a,
                                  0, 1, 0, frames.at(frameNum).pos[i][1] / 3.47 + b,
                                  0, 0, 1, (frames.at(frameNum).pos[i][2] / 3.47) + c,
                                  0, 0, 0, 1);
                newObj.object_transformation = translate0 * scale;
                newObj.scenePrim.type = SHAPE_SPHERE;
                newObj.scenePrim.material.shininess = 0;
                newObj.scenePrim.material.textureMap = NULL;
                newObj.scenePrim.material.bumpMap = NULL;
                newObj.scenePrim.material.blend = 0.5;
                newObj.scenePrim.material.cAmbient = color;
                newObj.scenePrim.material.cDiffuse = color;
                newObj.scenePrim.material.cSpecular = color;
                newObj.scenePrim.material.cReflective = color;
                newObj.scenePrim.material.cEmissive = color;

                object_vector.push_back(newObj);

            }
        }

        if (/*velocity*/true) {

			scene_velocity thevector;

			thevector.begin = Point(frames.at(frameNum).pos[i][0] / 3.47, 
									frames.at(frameNum).pos[i][1] / 3.47, 
									frames.at(frameNum).pos[i][2] / 3.47);

			thevector.thevector = Vector(frames.at(frameNum).vel[i][0] / 10,
										frames.at(frameNum).vel[i][1] / 10,
										frames.at(frameNum).vel[i][2] / 10);
			velocity_vector.push_back(thevector);


            /*struct scene_object cylinder;

            double velocityMag = sqrt(pow(frames.at(frameNum).vel[i][0],2) + 
                                      pow(frames.at(frameNum).vel[i][1],2) + 
                                      pow(frames.at(frameNum).vel[i][2],2));
            Vector velocityDir(frames.at(frameNum).vel[i][0], 
                               frames.at(frameNum).vel[i][1],
                               frames.at(frameNum).vel[i][2]);
            velocityDir.normalize();

            Vector cylinderPoint(frames.at(frameNum).pos[i][0] / 3.47 
                                        + sqrt(velocityMag/100) * (velocityDir[0] / 3.47),
                                 frames.at(frameNum).pos[i][1] / 3.47 
                                        + sqrt(velocityMag / 100) * (velocityDir[1] / 3.47),
                                 frames.at(frameNum).pos[i][2] / 3.47 
                                        + sqrt(velocityMag / 100) * (velocityDir[2] / 3.47));
            Matrix cylinderTranslate = trans_mat(cylinderPoint);

            Matrix cylinderScale(0.005, 0, 0, 0,
                                 0, velocityMag / 15, 0, 0,
                                 0, 0, 0.005, 0,
                                 0, 0, 0, 1);

            Vector xHat(1, 0, 0);
            Vector crossProd = cross(xHat, velocityDir);
            crossProd.normalize();

            double alpha = acos(dot(xHat, velocityDir));
            double c = cos(alpha);
            double s = sin(alpha);
           
            Matrix toVelocity(crossProd[0] * crossProd[0] * (1 - c) + c, 
                              crossProd[0] * crossProd[1] * (1 - c) - s * crossProd[2], 
                              crossProd[0] * crossProd[2] * (1 - c) + s * crossProd[1], 0,
                              crossProd[0] * crossProd[1] * (1 - c) + s * crossProd[2], 
                              crossProd[1] * crossProd[1] * (1 - c) + c, 
                              crossProd[1] * crossProd[2] * (1 - c) - s * crossProd[0], 0,
                              crossProd[0] * crossProd[2] * (1 - c) - s * crossProd[1], 
                              crossProd[1] * crossProd[2] * (1 - c) - s * crossProd[0], 
                              crossProd[2] * crossProd[2] * (1 - c) + c, 0,
                              0,0,0,1);

            //cylinder.object_transformation = cylinderTranslate * rotY_mat(yAngle) * rotZ_mat(zAngle) * cylinderScale;
            cylinder.object_transformation = cylinderTranslate * toVelocity * rotZ_mat(PI/2) * cylinderScale;
                                             
            cylinder.scenePrim.type = SHAPE_CYLINDER;
            cylinder.scenePrim.material.shininess = 0;
            cylinder.scenePrim.material.textureMap = NULL;
            cylinder.scenePrim.material.bumpMap = NULL;
            cylinder.scenePrim.material.blend = 0.5;

            color.r = .3; 
            color.g = .8;
            color.b = .7;
            color.a = .2;
            cylinder.scenePrim.material.cAmbient = color;
            cylinder.scenePrim.material.cDiffuse = color;
            cylinder.scenePrim.material.cSpecular = color;
            cylinder.scenePrim.material.cReflective = color;
            cylinder.scenePrim.material.cEmissive = color;

            object_vector.push_back(cylinder);*/
        }
    }
}




int PhaseReader::getFrameNum() {
    return frameNum; 
}
