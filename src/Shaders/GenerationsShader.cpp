#include "GenerationsShader.h"

namespace CAGUI{

GenerationsShader::GenerationsShader(): Shader<unsigned char>(3){
    m_paramName[0] = QString("Dead");
    m_paramName[1] = QString("Living");
    m_paramName[2] = QString("Middle-aged");
    m_paramColor[0] = QColor(0, 0, 0);
    m_paramColor[1] = QColor(255, 255, 32);
    m_paramColor[2] = QColor(224, 0, 0);
}

void GenerationsShader::setStateCount(int p_stateCount){
    m_stateCount = p_stateCount;
    shadeColor.resize(m_stateCount);
}

void GenerationsShader::update(){
    int i, r0, g0, b0, r1, g1, b1, r2, g2, b2;
    double t;
    shadeColor[0] = m_paramColor[0];
    shadeColor[1] = m_paramColor[1];
    m_paramColor[0].getRgb(&r0, &g0, &b0);
    m_paramColor[1].getRgb(&r1, &g1, &b1);
    m_paramColor[2].getRgb(&r2, &g2, &b2);
    for (i = 2; i < m_stateCount; i++){
        t = double(i - 1) / (m_stateCount + 1) * 2;
        if (t <= 1)
            shadeColor[i] = QColor(int(r2 * t + r1 * (1 - t)), int(g2 * t + g1 * (1 - t)), int(b2 * t + b1 * (1 - t)));
        else
            shadeColor[i] = QColor(int(r0 * (t - 1) + r2 * (2 - t)), int(g0 * (t - 1) + g2 * (2 - t)), int(b0 * (t - 1) + b2 * (2 - t)));
    }
}

}
