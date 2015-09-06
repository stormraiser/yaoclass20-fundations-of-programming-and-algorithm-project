#pragma once

#include "StateWidgetBase.h"
#include "Rule.h"
#include "Shader.h"
#include "ColorPlate.h"

class QScrollBar;
class QLabel;

using CACore::Rule;

namespace CAGUI{

class StateWidgetUnsignedChar: public StateWidgetBase{
private:
    Rule<unsigned char> *rule;
    Shader<unsigned char> *shader;
    QScrollBar *scroll;
    QLabel *label;
    ColorPlate *plate;
    unsigned char state;

public:
    StateWidgetUnsignedChar(QWidget *parent = 0);
    void process();
    void setRule(CACore::RuleBase *p_rule);
    void setShader(ShaderBase *p_shader);
    void *getState();
};

}
