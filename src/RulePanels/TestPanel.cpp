#include "TestPanel.h"
#include "Gas.h"
#include "ForestFire.h"
#include "WireWorld.h"
#include "GeneralShaderUnsignedChar.h"
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace CAGUI{

TestPanel::TestPanel(QWidget *parent): RulePanel(parent){
    ruleLabel = new QLabel("Rule:");
    ruleCombo = new QComboBox;

    ruleCombo->addItem("Forest Fire");
    ruleCombo->addItem("FHP Gas");
    ruleCombo->addItem("Wireworld");

    QHBoxLayout *layoutU = new QHBoxLayout;
    layoutU->addWidget(ruleLabel);
    layoutU->addWidget(ruleCombo);
    layoutU->addStretch();
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layoutU);
    mainLayout->addStretch();
    setLayout(mainLayout);
}

CACore::RuleBase *TestPanel::getRule(){
    switch (ruleCombo->currentIndex()){
    case 0:
        return new CACore::ForestFire;
    case 1:
        return new CACore::FHPGas;
    case 2:
        return new CACore::WireWorld;
    }
}

ShaderBase *TestPanel::getShader(){
    int i, c;
    GeneralShaderUnsignedChar *p;
    switch (ruleCombo->currentIndex()){
    case 0:
        p = new GeneralShaderUnsignedChar(5);
        p->setParamName(0, "Empty");
        p->setParamName(1, "Tree");
        p->setParamName(2, "Burning tree");
        p->setParamName(3, "Burning tree");
        p->setParamName(4, "Burning tree");
        p->setParamColor(0, QColor(0, 0, 0));
        p->setParamColor(1, QColor(0, 128, 0));
        p->setParamColor(2, QColor(255, 255, 0));
        p->setParamColor(3, QColor(224, 160, 0));
        p->setParamColor(4, QColor(192, 64, 0));
        return p;
    case 1:
        p = new GeneralShaderUnsignedChar(64);
        for (i = 0; i < 64; i++){
            p->setParamName(i, QString("State #%1").arg(i));
            c = (i & 1) + ((i >> 1) & 1) + ((i >> 2) & 1) + ((i >> 3) & 1) + ((i >> 4) & 1) + ((i >> 5) & 1);
            p->setParamColor(i, QColor(255 * c / 6, 255 * c / 6, 255 * c / 6));
        }
        return p;
    case 2:
        p = new GeneralShaderUnsignedChar(4);
        p->setParamName(0, "Empty");
        p->setParamName(1, "Wire");
        p->setParamName(2, "Electron head");
        p->setParamName(3, "Electron tail");
        p->setParamColor(0, QColor(0, 0, 0));
        p->setParamColor(1, QColor(255, 64, 0));
        p->setParamColor(2, QColor(0, 192, 255));
        p->setParamColor(3, QColor(96, 96, 96));
        return p;
    }
}

}
