#pragma once

#include "ShaderBase.h"
#include <QColor>

namespace CAGUI{

template <typename ValType> class Shader: public ShaderBase{
public:
    Shader(int p_paramCount = 0): ShaderBase(p_paramCount){};
    virtual QColor shade(ValType state) = 0;
};

}
