/**
  * ReactionDiffusionPanel.h
  * Configuration widget for reaction-diffusion rules.
  */

#pragma once

#include <QWidget>
#include "RulePanel.h"
#include "Rule.h"
#include "Shader.h"

class QLabel;
class QDoubleSpinBox;
class QGroupBox;

namespace CAGUI{

class ReactionDiffusionPanel: public RulePanel{
    Q_OBJECT

private:
    QDoubleSpinBox *speedSpin, *rateSpin[2], *paramSpin[3][3][2];
    QGroupBox *chemGroup[2];
    QLabel *titleLabel[2], *speedLabel, *rateLabel[2], *paramLabel[3][3][2];

public:
    ReactionDiffusionPanel(QWidget *parent = 0);
    CACore::RuleBase *getRule();
    ShaderBase *getShader();
    int getTypeID(){return TYPEID_DOUBLE_PAIR;};
};

}
