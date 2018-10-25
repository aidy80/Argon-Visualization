#include <iostream>
#include "phaseReader.h"

using namespace std;

int main() {
    PhaseReader phaseReader; 

    phaseReader.readIn("NVE.gro");

    vector<struct frame> frames = phaseReader.getFrames(); 
    //cout << "size: " << frames.size() << "\n";
    for(int i = 0; i < 240; i++) {
        phaseReader.update(i);
    }

    return 1;
}
