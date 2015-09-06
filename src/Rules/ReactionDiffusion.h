#pragma once

#include "Types.h"
#include "Rule.h"
#include <string>
#include <sstream>

using std::string;
using std::stringstream;

using CACore::DoublePair;

namespace CACore{

class ReactionDiffusion: public RuleType0<DoublePair>{
public:
    double diffusionRate[2], reactionSpeed, param[3][3][2];

    int stateCount(){return 256;};
    bool needConversion(){return false;};
    int period(){return 0;};
    int shape(){return SquareShape;};
    DoublePair real2Nominal(DoublePair state, int time){return state;};
    DoublePair nominal2Real(DoublePair state, int time){return state;};
    string stateName(DoublePair state);
    DoublePair calc(DoublePair state1[], DoublePair state2[], DoublePair state3[]);
};

DoublePair ReactionDiffusion::calc(DoublePair state1[], DoublePair state2[], DoublePair state3[]){
    double a, b, da, db, ta, tb;
    a = state2[1].a;
    b = state2[1].b;
    da = (state1[0].a + state1[2].a + state3[0].a + state3[2].a) * 0.05 + \
         (state1[1].a + state2[0].a + state2[2].a + state3[1].a) * 0.2 - a;
    db = (state1[0].b + state1[2].b + state3[0].b + state3[2].b) * 0.05 + \
         (state1[1].b + state2[0].b + state2[2].b + state3[1].b) * 0.2 - b;
    ta = (((param[2][2][0] * a + param[2][1][0]) * a + param[2][0][0]) * b + \
           (param[1][2][0] * a + param[1][1][0]) * a + param[1][0][0]) * b + \
           (param[0][2][0] * a + param[0][1][0]) * a + param[0][0][0];
    tb = (((param[2][2][1] * a + param[2][1][1]) * a + param[2][0][1]) * b + \
           (param[1][2][1] * a + param[1][1][1]) * a + param[1][0][1]) * b + \
           (param[0][2][1] * a + param[0][1][1]) * a + param[0][0][1];
    a += (da * diffusionRate[0] + ta) * reactionSpeed;
    b += (db * diffusionRate[1] + tb) * reactionSpeed;
    if (a < 0)
        a = 0;
    if (a > 1)
        a = 1;
    if (b < 0)
        b = 0;
    if (b > 1)
        b = 1;
    return DoublePair(a, b);
}

string ReactionDiffusion::stateName(DoublePair state){
    stringstream stream;
    string str;
    stream.precision(4);
    stream << "Density:A=" << state.a << ",B=" << state.b;
    stream >> str;
    return str;
}

}
