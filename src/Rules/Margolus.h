#pragma once

#include "Rule.h"

namespace CACore{

class Margolus: public RuleType1<unsigned char>{
public:
    int m_stateCount, param[256], table[256][4];

    int index(unsigned char s1, unsigned char s2, unsigned char s3, unsigned char s4)
        {return ((s4 * m_stateCount + s3) * m_stateCount + s2) * m_stateCount + s1;};
    void update();
    int stateCount(){return m_stateCount;};
    bool needConversion(){return false;};
    int period(){return 2;};
    int shape(){return SquareShape;};
    unsigned char real2Nominal(unsigned char state, int time){return state;};
    unsigned char nominal2Real(unsigned char state, int time){return state;};
    string stateName(unsigned char state){return " ";};
    unsigned char calc(unsigned char state1[], unsigned char state2[], unsigned char state3[], int x, int y, int time);
};

unsigned char Margolus::calc(unsigned char state1[], unsigned char state2[], unsigned char state3[], int x, int y, int time){
    switch ((time & 1) * 4 + (x > 0 ? x & 1 : (-x) & 1) * 2 + (y > 0 ? y & 1 : (-y) & 1)){
    case 0:
    case 7:
        return table[index(state2[2], state3[2], state3[1], state2[1])][3];
    case 1:
    case 6:
        return table[index(state2[1], state3[1], state3[0], state2[0])][0];
    case 2:
    case 5:
        return table[index(state1[2], state2[2], state2[1], state1[1])][2];
    case 3:
    case 4:
        return table[index(state1[1], state2[1], state2[0], state1[0])][1];
    default:
        return 0;
    }
}

void Margolus::update(){
    int i;
    for (i = 0; i < 256; i++){
        table[i][0] = param[i] % m_stateCount;
        table[i][1] = (param[i] / m_stateCount) % m_stateCount;
        table[i][2] = (param[i] / m_stateCount / m_stateCount) % m_stateCount;
        table[i][3] = param[i] / m_stateCount / m_stateCount / m_stateCount;
    }
}

}
