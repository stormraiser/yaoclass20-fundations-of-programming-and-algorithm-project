#pragma once

#include <vector>
#include <QString>
#include <QColor>

using std::vector;

namespace CAGUI{

class ShaderBase{
protected:
    int m_paramCount;
    vector<QString> m_paramName;
    vector<QColor> m_paramColor;

public:
    ShaderBase(int p_paramCount = 0);
    virtual ~ShaderBase(){};
    void setParamCount(int p_paramCount);
    void setParamName(int index, QString name){m_paramName[index] = name;};
    void setParamColor(int index, QColor color){m_paramColor[index] = color;};
    int paramCount(){return m_paramCount;};
    QString paramName(int index){return m_paramName[index];};
    QColor paramColor(int index){return m_paramColor[index];};
    virtual void update() = 0;
};

}
