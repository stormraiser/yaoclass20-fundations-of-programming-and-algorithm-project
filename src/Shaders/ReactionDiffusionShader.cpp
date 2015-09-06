#include "ReactionDiffusionShader.h"

#include <QString>
#include <QColor>

using CACore::DoublePair;

namespace CAGUI{

ReactionDiffusionShader::ReactionDiffusionShader(): Shader<DoublePair>(4){
    m_paramName[0] = QString("Empty");
    m_paramName[1] = QString("Chemical A full");
    m_paramName[2] = QString("Chemical B full");
    m_paramName[3] = QString("Chemical A&B full");
    m_paramColor[0] = QColor(0, 0, 0);
    m_paramColor[1] = QColor(255, 0, 0);
    m_paramColor[2] = QColor(255, 255, 0);
    m_paramColor[3] = QColor(255, 255, 255);
}

void ReactionDiffusionShader::update(){
    int i, j, cr, cg, cb;
    double a, b, t0, t1, t2, t3;
    for (i = 0; i <= 200; i++)
        for (j = 0; j <= 200; j++)
            if (!(((i == 0) && (j == 0)) || ((i == 200) && (j == 200)))){
                a = double(i) / 200;
                b = double(j) / 200;
                if (a + b >= 1){
                    t3 = (a + b - 1) * (a + b - 1);
                    t0 = 0;
                    t1 = (1 - t3) * ((1 - b) * (1 - b)) / ((1 - b) * (1 - b) + (1 - a) * (1 - a));
                    t2 = (1 - t3) * ((1 - a) * (1 - a)) / ((1 - b) * (1 - b) + (1 - a) * (1 - a));
                }
                else{
                    t0 = (1 - a - b) * (1 - a - b);
                    t3 = 0;
                    t1 = (1 - t0) * (a * a) / (a * a + b * b);
                    t2 = (1 - t0) * (b * b) / (a * a + b * b);
                }
                cr = int(m_paramColor[0].red() * t0 + \
                         m_paramColor[1].red() * t1 + \
                         m_paramColor[2].red() * t2 + \
                         m_paramColor[3].red() * t3);
                cg = int(m_paramColor[0].green() * t0 + \
                         m_paramColor[1].green() * t1 + \
                         m_paramColor[2].green() * t2 + \
                         m_paramColor[3].green() * t3);
                cb = int(m_paramColor[0].blue() * t0 + \
                         m_paramColor[1].blue() * t1 + \
                         m_paramColor[2].blue() * t2 + \
                         m_paramColor[3].blue() * t3);
                shadeColor[i][j] = QColor(cr, cg, cb);
            }
    shadeColor[0][0] = m_paramColor[0];
    shadeColor[200][200] = m_paramColor[3];
}

QColor ReactionDiffusionShader::shade(DoublePair state){
    return shadeColor[int(state.a * 200)][int(state.b * 200)];
}

}
