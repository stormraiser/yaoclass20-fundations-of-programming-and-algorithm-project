#pragma once

#include "Shader.h"
#include <QColor>

namespace CAGUI{

class GeneralShaderUnsignedChar: public Shader<unsigned char>{
public:
    GeneralShaderUnsignedChar(int p_stateCount = 0): Shader<unsigned char>(p_stateCount){};
    void update(){};
    QColor shade(unsigned char state){return m_paramColor[state];};
};

}
