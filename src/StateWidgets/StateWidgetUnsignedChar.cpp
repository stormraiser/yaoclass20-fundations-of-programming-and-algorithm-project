#include "StateWidgetUnsignedChar.h"
#include "Common.h"
#include <QScrollBar>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <string>

using std::string;
using CACore::Rule;

namespace CAGUI{

StateWidgetUnsignedChar::StateWidgetUnsignedChar(QWidget *parent): StateWidgetBase(parent){
    scroll = new QScrollBar(Qt::Horizontal);
    label = new QLabel("State: #0");
    plate = new ColorPlate;
    shader = NULL;

    scroll->setRange(0, 1);
    scroll->setValue(0);
    scroll->setFixedSize(200, 18);
    scroll->setFocusPolicy(Qt::NoFocus);
    scroll->setSingleStep(1);
    scroll->setPageStep(1);
    plate->setFixedSize(25, 25);

    QVBoxLayout *layoutL = new QVBoxLayout;
    layoutL->addWidget(label);
    layoutL->addWidget(scroll);
    layoutL->setContentsMargins(0, 0, 0, 0);
    layoutL->setSpacing(4);
    layoutL->setAlignment(label, Qt::AlignCenter);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(layoutL);
    mainLayout->addWidget(plate);
    mainLayout->setContentsMargins(4, 4, 4, 4);
    layoutL->setSpacing(4);
    setLayout(mainLayout);
    setFixedSize(sizeHint());

    connect(scroll, SIGNAL(valueChanged(int)), this, SLOT(process()));
}

void StateWidgetUnsignedChar::setRule(CACore::RuleBase *p_rule){
    rule = RCAST(unsigned char, p_rule);
    scroll->setRange(0, rule->stateCount() - 1);
    process();
}

void StateWidgetUnsignedChar::setShader(ShaderBase *p_shader){
    shader = SCAST(unsigned char, p_shader);
    process();
}

void StateWidgetUnsignedChar::process(){
    label->setText(QString("State #%1: ").arg(scroll->value()).append(QString(rule->stateName(scroll->value()).c_str())));
    if (shader != NULL)
        plate->setColor(shader->shade(scroll->value()));
    emit valueChanged();
}

void *StateWidgetUnsignedChar::getState(){
    state = scroll->value();
    return &state;
}

}
