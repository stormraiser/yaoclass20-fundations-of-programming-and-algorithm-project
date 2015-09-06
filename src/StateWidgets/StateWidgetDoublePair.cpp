#include "StateWidgetDoublePair.h"
#include "Types.h"
#include "Common.h"
#include <QLabel>
#include <QDoubleSpinBox>
#include <QGridLayout>

using CACore::DoublePair;
using CACore::RuleBase;

namespace CAGUI{

StateWidgetDoublePair::StateWidgetDoublePair(QWidget *parent): StateWidgetBase(parent){
    rule = NULL;
    shader = NULL;
    label = new QLabel;
    spin1 = new QDoubleSpinBox;
    spin2 = new QDoubleSpinBox;
    plate = new ColorPlate;

    plate->setFixedSize(25, 25);
    spin1->setRange(0, 1);
    spin1->setSingleStep(0.02);
    spin1->setDecimals(4);
    spin1->setFixedSize(spin1->sizeHint().width(), 20);
    spin2->setRange(0, 1);
    spin2->setSingleStep(0.02);
    spin2->setDecimals(4);
    spin2->setFixedSize(spin2->sizeHint().width(), 20);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(label, 0, 0, 1, 2);
    mainLayout->addWidget(spin1, 1, 0);
    mainLayout->addWidget(spin2, 1, 1);
    mainLayout->addWidget(plate, 0, 2, 2, 1);
    mainLayout->setContentsMargins(3, 3, 3, 3);
    mainLayout->setSpacing(2);
    setLayout(mainLayout);
    setFixedSize(sizeHint());

    connect(spin1, SIGNAL(valueChanged(double)), this, SLOT(process()));
    connect(spin2, SIGNAL(valueChanged(double)), this, SLOT(process()));
}

void StateWidgetDoublePair::setRule(RuleBase *p_rule){
    rule = RCAST(DoublePair, p_rule);
    process();
}

void StateWidgetDoublePair::setShader(ShaderBase *p_shader){
    shader = SCAST(DoublePair, p_shader);
    process();
}

void StateWidgetDoublePair::process(){
    state = DoublePair(spin1->value(), spin2->value());
    label->setText(QString(rule->stateName(state).c_str()));
    if (shader != NULL){
        plate->setColor(shader->shade(state));
    }
    emit valueChanged();
}

void *StateWidgetDoublePair::getState(){
    return &state;
}

}
