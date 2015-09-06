#include "ShaderBase.h"

namespace CAGUI{

ShaderBase::ShaderBase(int p_paramCount){
    m_paramCount = p_paramCount;
    m_paramName.resize(m_paramCount);
    m_paramColor.resize(m_paramCount);
}

void ShaderBase::setParamCount(int p_paramCount){
    m_paramCount = p_paramCount;
    m_paramName.resize(m_paramCount);
    m_paramColor.resize(m_paramCount);
}

}
