#pragma once

#include "Rule.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

namespace CACore{

class WireWorld: public RuleType0<unsigned char>{
public:
    vector<string> stateNameStr;
    WireWorld();
    int stateCount(){return 4;};
    bool needConversion(){return false;};
    int shape(){return SquareShape;};
    int period(){return 1;};
    unsigned char real2Nominal(unsigned char state, int time){return state;};
    unsigned char nominal2Real(unsigned char state, int time){return state;};
    string stateName(unsigned char state){return stateNameStr[state];};
    unsigned char calc(unsigned char state1[], unsigned char state2[], unsigned char state3[]);
};

WireWorld::WireWorld(){
    stateNameStr.resize(4);
    stateNameStr[0] = "Empty";
    stateNameStr[1] = "Wire";
    stateNameStr[2] = "Electron head";
    stateNameStr[3] = "Electron tail";
}

unsigned char WireWorld::calc(unsigned char state1[], unsigned char state2[], unsigned char state3[]){
    int c;
    switch (state2[1]){
    case 1:
        c = (state1[0] == 2) + (state1[1] == 2) + (state1[2] == 2) + (state2[0] == 2) +
            (state2[2] == 2) + (state3[0] == 2) + (state3[1] == 2) + (state3[2] == 2);
        return ((c == 1) || (c == 2)) ? 2 : 1;
    case 2:
        return 3;
    case 3:
        return 1;
    default:
        return 0;
    }
}

}
