#pragma once

#include "StateWidgetBase.h"
#include "Types.h"
#include "ColorPlate.h"
#include "Shader.h"
#include "Rule.h"

using CACore::DoublePair;
using CACore::Rule;

class QDoubleSpinBox;
class QLabel;

namespace CAGUI{

class StateWidgetDoublePair: public StateWidgetBase{
private:
    Rule<DoublePair> *rule;
    Shader<DoublePair> *shader;
    QLabel *label;
    QDoubleSpinBox *spin1, *spin2;
    ColorPlate *plate;
    DoublePair state;

public:
    StateWidgetDoublePair(QWidget *parent = 0);
    void process();
    void setRule(CACore::RuleBase *p_rule);
    void setShader(ShaderBase *p_shader);
    void *getState();
};

}
