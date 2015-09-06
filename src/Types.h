#pragma once

#include <cmath>

using std::abs;

namespace CACore{

struct DoublePair{
    double a, b;

    explicit DoublePair(double p_a = 0, double p_b = 0): a(p_a), b(p_b){};
    DoublePair(unsigned char c): a(double(c / 16) / 15), b(double(c % 16) / 15){};
    operator unsigned char(){return ((int)(a * 15) * 16 + (int)(b * 16)) % 256;};
    bool operator ==(DoublePair &other){return (abs(a - other.a) < 1e-9) && (abs(b - other.b) < 1e-9);};
};

}
