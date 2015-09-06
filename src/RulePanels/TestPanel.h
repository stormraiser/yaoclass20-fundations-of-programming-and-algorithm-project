/**
  * TestPanel.h
  * Temporary configuration widget for new rules under testing.
  */

#pragma once

#include "Common.h"
#include "RulePanel.h"
#include "Shader.h"

class QComboBox;
class QLabel;

namespace CAGUI{

class TestPanel: public RulePanel{
private:
    QComboBox *ruleCombo;
    QLabel *ruleLabel;

public:
    TestPanel(QWidget *parent = 0);
    CACore::RuleBase *getRule();
    ShaderBase *getShader();
    int getTypeID(){return TYPEID_UNSIGNED_CHAR;};
};

}
