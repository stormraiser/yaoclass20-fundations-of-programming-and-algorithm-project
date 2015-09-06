#pragma once

#include <string>
#include <vector>
#include "Common.h"

using std::string;
using std::vector;

namespace CACore{

class RuleBase{
public:
    enum GridShape{SquareShape, HexagonalShape};
    virtual int stateCount() = 0;
    virtual int ruleType() = 0;
    virtual bool needConversion() = 0;
    virtual int period() = 0;
    virtual int shape() = 0;
    virtual ~RuleBase(){};
};

template <typename ValType> class Rule: public RuleBase{
public:
    virtual ValType real2Nominal(ValType state, int time) = 0;
    virtual ValType nominal2Real(ValType state, int time) = 0;
    virtual string stateName(ValType state) = 0;
};

template <typename ValType> class RuleType0: public Rule<ValType>{
public:
    int ruleType(){return 0;};
    virtual ValType calc(ValType state1[], ValType state2[], ValType state3[]) = 0;
};

template <typename ValType> class RuleType1: public Rule<ValType>{
public:
    int ruleType(){return 1;};
    virtual ValType calc(ValType state1[], ValType state2[], ValType state3[], int x, int y, int time) = 0;
};

template <typename ValType> class RuleType2: public Rule<ValType>{
public:
    int ruleType(){return 2;};
    virtual int radius() = 0;
    virtual ValType calc(ValType *state, int x, int y, int time) = 0;
};

}
