/**
  * Gas.h
  * Defines two lattice gas rules, HPPGas and FHPGas.
  * <- Still under testing ->
  * These rules are used as examples of physics system modeling
  * with cellular automata.
  */

#pragma once

#include "Rule.h"
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

using std::stringstream;
using std::string;
using std::vector;

namespace CACore{

class HPPGas: public RuleType0<unsigned char>{
public:
    vector<string> stateNameStr;
    bool table[4][16];

    HPPGas();
    int stateCount(){return 16;};
    bool needConversion(){return false;};
    int period(){return 1;};
    int shape(){return SquareShape;};
    unsigned char real2Nominal(unsigned char state, int time){return state;};
    unsigned char nominal2Real(unsigned char state, int time){return state;};
    string stateName(unsigned char state){return stateNameStr[state];};
    unsigned char calc(unsigned char state1[], unsigned char state2[], unsigned char state3[]);
};

HPPGas::HPPGas(){
    int i, j;
    stateNameStr.resize(16);
    stateNameStr[0] = "Empty";
    stateNameStr[1] = "Gas v";
    stateNameStr[2] = "Gas <";
    stateNameStr[3] = "Gas < v";
    stateNameStr[4] = "Gas ^";
    stateNameStr[5] = "Gas ^ v";
    stateNameStr[6] = "Gas ^ <";
    stateNameStr[7] = "Gas ^ < v";
    stateNameStr[8] = "Gas >";
    stateNameStr[9] = "Gas > v";
    stateNameStr[10] = "Gas > <";
    stateNameStr[11] = "Gas > < v";
    stateNameStr[12] = "Gas > ^";
    stateNameStr[13] = "Gas > ^ v";
    stateNameStr[14] = "Gas > ^ <";
    stateNameStr[15] = "Gas > ^ < v";
    for (i = 0; i < 4; i++)
        for (j = 0; j < 16; j++)
            table[i][j] = 0;
    table[0][1] = 1;
    table[0][3] = 1;
    table[0][10] = 1;
    table[0][7] = 1;
    table[0][9] = 1;
    table[0][11] = 1;
    table[0][13] = 1;
    table[0][15] = 1;
    table[1][2] = 1;
    table[1][3] = 1;
    table[1][6] = 1;
    table[1][7] = 1;
    table[1][5] = 1;
    table[1][11] = 1;
    table[1][14] = 1;
    table[1][15] = 1;
    table[2][4] = 1;
    table[2][10] = 1;
    table[2][6] = 1;
    table[2][7] = 1;
    table[2][12] = 1;
    table[2][13] = 1;
    table[2][14] = 1;
    table[2][15] = 1;
    table[3][8] = 1;
    table[3][9] = 1;
    table[3][5] = 1;
    table[3][11] = 1;
    table[3][12] = 1;
    table[3][13] = 1;
    table[3][14] = 1;
    table[3][15] = 1;
}

unsigned char HPPGas::calc(unsigned char state1[], unsigned char state2[], unsigned char state3[]){
    return table[0][state2[2]] | (table[1][state3[1]] << 1) | (table[2][state2[0]] << 2) | (table[3][state1[1]] << 3);
}

class FHPGas: public RuleType0<unsigned char>{
public:
    vector<string> stateNameStr;

    FHPGas();
    int stateCount(){return 64;};
    bool needConversion(){return false;};
    int period(){return 1;};
    int shape(){return HexagonalShape;};
    unsigned char real2Nominal(unsigned char state, int time){return state;};
    unsigned char nominal2Real(unsigned char state, int time){return state;};
    string stateName(unsigned char state){return stateNameStr[state];};
    unsigned char calc(unsigned char state1[], unsigned char state2[], unsigned char state3[]);
};

FHPGas::FHPGas(){
    int i;
    bool f;
    stringstream stream;
    stateNameStr.resize(64);
    stateNameStr[0] = "Empty";
    for (i = 1; i < 64; i++){
        stateNameStr[i] = "Gas";
        f = false;
        if ((i & 1) != 0){
            if (f)
                stateNameStr[i].append(",ul");
            else{
                stateNameStr[i].append(":ul");
                f = true;
            }
        }
        if ((i & 2) != 0){
            if (f)
                stateNameStr[i].append(",ur");
            else{
                stateNameStr[i].append(":ur");
                f = true;
            }
        }
        if ((i & 4) != 0){
            if (f)
                stateNameStr[i].append(",r");
            else{
                stateNameStr[i].append(":r");
                f = true;
            }
        }
        if ((i & 8) != 0){
            if (f)
                stateNameStr[i].append(",dr");
            else{
                stateNameStr[i].append(":dr");
                f = true;
            }
        }
        if ((i & 16) != 0){
            if (f)
                stateNameStr[i].append(",dl");
            else{
                stateNameStr[i].append(":dl");
                f = true;
            }
        }
        if ((i & 32) != 0){
            if (f)
                stateNameStr[i].append(",l");
            else{
                stateNameStr[i].append(":l");
                f = true;
            }
        }
    }
}

unsigned char FHPGas::calc(unsigned char state1[], unsigned char state2[], unsigned char state3[]){
    int t = (state2[0] & 1) | (state1[0] & 2) | (state1[1] & 4) | (state2[2] & 8) | (state3[2] & 16) | (state3[1] & 32);
    switch (t){
    case 9:
        return rand() % 2 ? 18 : 36;
    case 18:
        return rand() % 2 ? 9 : 36;
    case 36:
        return rand() % 2 ? 9 : 18;
    case 21:
        return 42;
    case 42:
        return 21;
    default:
        return t;
    }
}

}
