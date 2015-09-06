#pragma once

#include "Rule.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

namespace CACore{

// LifeLike is the simplest function in all rules, it as only two type of states, namely living and dead.

class LifeLike: public RuleType0<unsigned char>{
public:
    vector<string> stateNameStr;  // this is used to record name for different state so that it can be easily understood
    string rulename;  // used to briefly introduce the function of the rule
    // B123 would means that the cell will turn from 0 to 1 if it has 1,2 or 3 live neighbors

    unsigned char table[2][9];  // judge of the rule

    LifeLike();
    int period(){return 1;};
    int stateCount(){return 2;};
    int shape(){return SquareShape;};
    bool needConversion(){return false;};
    unsigned char real2Nominal(unsigned char state, int time){return state;};
    unsigned char nominal2Real(unsigned char state, int time){return state;};
    unsigned char calc(unsigned char state1[], unsigned char state2[], unsigned char state3[]);  // calculate the next state of the cell
    void Setrule(char** nRule);  // used to setrule to Lifelike
    string stateName(unsigned char state){return stateNameStr[state];};  // return the name of a certain state
    string ruleName(){return rulename;};  // return the rule's name
};

LifeLike::LifeLike(){
    // in the initialization, we will merely need to set the name for each states
    stateNameStr.clear();
    stateNameStr.push_back("Dead");
    stateNameStr.push_back("Living");
}

unsigned char LifeLike::calc(unsigned char state1[], unsigned char state2[], unsigned char state3[]){
    int amount = 0;
    for (int i=0; i<3; i++)
        amount += state1[i] + state2[i] + state3[i];  // calculate the surviving cells in its surrounding
    amount -= state2[1];
    return table[state2[1]][amount];  // use this number to decide the final output based on table
}

void LifeLike::Setrule(char** nRule){
    // first copy table to be the same as nRule
    for (int i=0;i<9;i++)
    {
        table[0][i] = nRule[0][i];
        table[1][i] = nRule[1][i];
    }
    rulename.clear();  // initialize the rulename
    rulename.push_back('B');
    for (int i=0;i<9;i++)
        if (table[0][i] == 1)
            // if the value is 1, then we should record this value in the $B$ list, also note that 48 is char representation for 0
            rulename.push_back(i+48);
    rulename.push_back('/');  // basic setting again
    rulename.push_back('S');
    for (int i=0;i<9;i++){
        // record the terminating list
        if (table[1][i] == 0)
            rulename.push_back(i+48);
    }
}





class LifeLikeSpecial: public RuleType1<unsigned char>{
public:
    vector<string> stateNameStr;

    unsigned char table[2][9][2];
    bool convertFlag;

    LifeLikeSpecial();
    int period(){return 2;};
    int stateCount(){return 2;};
    int shape(){return SquareShape;};
    bool needConversion(){return true;};
    unsigned char real2Nominal(unsigned char state, int time);
    unsigned char nominal2Real(unsigned char state, int time);
    unsigned char calc(unsigned char state1[], unsigned char state2[], unsigned char state3[], int x, int y, int time);
    string stateName(unsigned char state){return stateNameStr[state];};
};

LifeLikeSpecial::LifeLikeSpecial(){
    stateNameStr.clear();
    stateNameStr.push_back("Dead");
    stateNameStr.push_back("Living");
}

unsigned char LifeLikeSpecial::calc(unsigned char state1[], unsigned char state2[], unsigned char state3[], int x, int y, int time){
    int c = 0, i;
    for (i = 0; i < 3; i++)
        c += state1[i] + state2[i] + state3[i];
    c -= state2[1];
    if (convertFlag)
        return table[state2[1]][c][time % 2];
    else
        return table[state2[1]][c][time > 1];
}

unsigned char LifeLikeSpecial::real2Nominal(unsigned char state, int time){
    if (convertFlag)
        return state ^ (time % 2);
    else
        return state ^ (time > 0);
}

unsigned char LifeLikeSpecial::nominal2Real(unsigned char state, int time){
    if (convertFlag)
        return state ^ (time % 2);
    else
        return state ^ (time > 0);
}

class LifeLikeHex: public LifeLike{
public:
    unsigned char calc(unsigned char state1[], unsigned char state2[], unsigned char state3[]);
    int shape(){return HexagonalShape;};
};

unsigned char LifeLikeHex::calc(unsigned char state1[], unsigned char state2[], unsigned char state3[]){
    int c = state1[0] + state1[1] + state2[0] + state2[2] + state3[1] + state3[2];
    return table[state2[1]][c];
}

}
