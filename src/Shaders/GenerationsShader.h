#pragma once

#include "Shader.h"
#include <QColor>
#include <vector>

using std::vector;

namespace CAGUI{

class GenerationsShader: public Shader<unsigned char>{
private:
    int m_stateCount;
    vector<QColor> shadeColor;

public:
    GenerationsShader();
    void setStateCount(int p_stateCount);
    void update();
    QColor shade(unsigned char state){return shadeColor[state];};
};

}
