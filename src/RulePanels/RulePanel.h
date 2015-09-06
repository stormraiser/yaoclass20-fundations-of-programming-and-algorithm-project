/**
  * RulePanel.h
  * Defines class RulePanel
  * This is the base class of specific widgets for configuring
  * various familys of rules.
  */

#pragma once

#include <QWidget>

namespace CACore{

class RuleBase;

}

namespace CAGUI{

class ShaderBase;

class RulePanel: public QWidget{
public:
    RulePanel(QWidget *parent = 0): QWidget(parent){};
    virtual CACore::RuleBase *getRule() = 0;
    virtual ShaderBase *getShader() = 0;
    virtual int getTypeID() = 0;
};

}
