#pragma once

#include <QWidget>
#include "Rule.h"
#include "ShaderBase.h"

namespace CAGUI{

class StateWidgetBase: public QWidget{
    Q_OBJECT

public:
    StateWidgetBase(QWidget *parent = 0): QWidget(parent){};
    virtual void *getState() = 0;
    virtual void setRule(CACore::RuleBase *p_rule) = 0;
    virtual void setShader(ShaderBase *p_shader) = 0;

signals:
    void valueChanged();

public slots:
    virtual void process() = 0;
};

}
