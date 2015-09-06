#pragma once

#include "Rule.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

namespace CACore{

class Generations: public RuleType0<unsigned char>{
public:
    vector<string> stateNameStr;

    unsigned char table[256][9];
    int m_stateCount;

    Generations();
    int period(){return 1;};
    int stateCount(){return m_stateCount;};
    int shape(){return SquareShape;};
    bool needConversion(){return false;};
    unsigned char real2Nominal(unsigned char state, int time){return state;};
    unsigned char nominal2Real(unsigned char state, int time){return state;};
    unsigned char calc(unsigned char state1[], unsigned char state2[], unsigned char state3[]);
    string stateName(unsigned char state){return state < 2 ? stateNameStr[state] : stateNameStr[2];};
};

Generations::Generations(){
    stateNameStr.clear();
    stateNameStr.push_back("Dead");
    stateNameStr.push_back("Living");
    stateNameStr.push_back("Aged");
}

unsigned char Generations::calc(unsigned char state1[], unsigned char state2[], unsigned char state3[]){
    int c = 0, i;
    for (i = 0; i < 3; i++){
        if (state1[i] == 1)
            c++;
        if ((state2[i] == 1) && (i != 1))
            c++;
        if (state3[i] == 1)
            c++;
    }
    return table[state2[1]][c];
}

class GenerationsHex: public Generations{
public:
    unsigned char calc(unsigned char state1[], unsigned char state2[], unsigned char state3[]);
    int shape(){return HexagonalShape;};
};

unsigned char GenerationsHex::calc(unsigned char state1[], unsigned char state2[], unsigned char state3[]){
    int c = (state1[0] == 1) + (state1[1] == 1) + (state2[0] == 1)
          + (state2[2] == 1) + (state3[1] == 1) + (state3[2] == 1);
    return table[state2[1]][c];
}

}
