/**
  * ForestFire.h
  * Defines rule ForestFire.
  * <- Still under testing ->
  * This rule is used as an example of stochastic rules.
  * Trees grow randomly, lightening strike trees randomly, causing fire.
  * Fire spreads to neighbouring trees.
  */

#pragma once

#include "Rule.h"
#include <string>
#include <vector>
#include <cstdlib>

using std::string;
using std::vector;

namespace CACore{

class ForestFire: public RuleType0<unsigned char>{
public:
    vector<string> stateNameStr;
    double growRate, burnRate;

    ForestFire();
    int stateCount(){return 5;};
    bool needConversion(){return false;};
    int period(){return 0;};
    int shape(){return SquareShape;};
    unsigned char real2Nominal(unsigned char state, int time){return state;};
    unsigned char nominal2Real(unsigned char state, int time){return state;};
    string stateName(unsigned char state){return stateNameStr[state];};
    unsigned char calc(unsigned char state1[], unsigned char state2[], unsigned char state3[]);
};

ForestFire::ForestFire(){
    stateNameStr.resize(5);
    stateNameStr[0] = "Empty";
    stateNameStr[1] = "Tree";
    stateNameStr[2] = "Burning tree";
    stateNameStr[3] = "Burning tree";
    stateNameStr[4] = "Burning tree";
    growRate = 0.01;
    burnRate = 0.0001;
}

unsigned char ForestFire::calc(unsigned char state1[], unsigned char state2[], unsigned char state3[]){
    switch (state2[1]){
    case 2:
        return 3;
    case 3:
        return 4;
    case 4:
        return 0;
    case 0:
        return (rand() <= RAND_MAX * growRate);
    case 1:
        if ((rand() <= RAND_MAX * burnRate) || (state1[1] > 1) || (state2[0] > 1) || (state2[2] > 1) || (state3[1] > 1))
            return 2;
        else
            return 1;
    default:
        return 0;
    }
}

}
