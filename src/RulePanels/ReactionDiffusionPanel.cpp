#include "ReactionDiffusionPanel.h"
#include "ReactionDiffusion.h"
#include "ReactionDiffusionShader.h"
#include <QLabel>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QGridLayout>

namespace CAGUI{

ReactionDiffusionPanel::ReactionDiffusionPanel(QWidget *parent): RulePanel(parent){
    int i, j, k;
    speedLabel = new QLabel("Reaction speed:");
    chemGroup[0] = new QGroupBox("Chemical A");
    chemGroup[1] = new QGroupBox("Chemical B");
    rateLabel[0] = new QLabel("Diffusion rate:");
    rateLabel[1] = new QLabel("Diffusion rate:");
    titleLabel[0] = new QLabel("Reaction:");
    titleLabel[1] = new QLabel("Reaction:");
    paramLabel[0][0][0] = new QLabel("1");
    paramLabel[0][0][1] = new QLabel("1");
    paramLabel[0][1][0] = new QLabel("a");
    paramLabel[0][1][1] = new QLabel("a");
    paramLabel[0][2][0] = new QLabel("a^2");
    paramLabel[0][2][1] = new QLabel("a^2");
    paramLabel[1][0][0] = new QLabel("b");
    paramLabel[1][0][1] = new QLabel("b");
    paramLabel[1][1][0] = new QLabel("ab");
    paramLabel[1][1][1] = new QLabel("ab");
    paramLabel[1][2][0] = new QLabel("a^2b");
    paramLabel[1][2][1] = new QLabel("a^2b");
    paramLabel[2][0][0] = new QLabel("b^2");
    paramLabel[2][0][1] = new QLabel("b^2");
    paramLabel[2][1][0] = new QLabel("ab^2");
    paramLabel[2][1][1] = new QLabel("ab^2");
    paramLabel[2][2][0] = new QLabel("a^2b^2");
    paramLabel[2][2][1] = new QLabel("a^2b^2");
    speedSpin = new QDoubleSpinBox;
    rateSpin[0] = new QDoubleSpinBox;
    rateSpin[1] = new QDoubleSpinBox;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            for (k = 0; k < 2; k++){
                paramSpin[i][j][k] = new QDoubleSpinBox;
                paramSpin[i][j][k]->setRange(-10, 10);
                paramSpin[i][j][k]->setSingleStep(0.05);
                paramSpin[i][j][k]->setValue(0);
                paramSpin[i][j][k]->setDecimals(4);
                paramSpin[i][j][k]->setFixedWidth(80);
            }

    QHBoxLayout *layoutAU = new QHBoxLayout;
    layoutAU->addWidget(rateLabel[0]);
    layoutAU->addWidget(rateSpin[0]);
    layoutAU->addStretch();
    QGridLayout *layoutA = new QGridLayout;
    layoutA->addLayout(layoutAU, 0, 0, 1, 6);
    layoutA->addWidget(titleLabel[0], 1, 0, 1, 2);
    layoutA->setContentsMargins(6, 6, 6, 6);
    layoutA->setSpacing(5);
    QHBoxLayout *layoutBU = new QHBoxLayout;
    layoutBU->addWidget(rateLabel[1]);
    layoutBU->addWidget(rateSpin[1]);
    layoutBU->addStretch();
    QGridLayout *layoutB = new QGridLayout;
    layoutB->addLayout(layoutBU, 0, 0, 1, 6);
    layoutB->addWidget(titleLabel[1], 1, 0, 1, 2);
    layoutB->setContentsMargins(6, 6, 6, 6);
    layoutB->setSpacing(5);
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++){
            layoutA->addWidget(paramLabel[i][j][0], i + 2, j * 2);
            layoutA->addWidget(paramSpin[i][j][0], i + 2, j * 2 + 1);
            layoutB->addWidget(paramLabel[i][j][1], i + 2, j * 2);
            layoutB->addWidget(paramSpin[i][j][1], i + 2, j * 2 + 1);
        }
    chemGroup[0]->setLayout(layoutA);
    chemGroup[1]->setLayout(layoutB);
    QHBoxLayout *layoutU = new QHBoxLayout;
    layoutU->addWidget(speedLabel);
    layoutU->addWidget(speedSpin);
    layoutU->addStretch();
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layoutU);
    mainLayout->addWidget(chemGroup[0]);
    mainLayout->addWidget(chemGroup[1]);
    setLayout(mainLayout);
}

CACore::RuleBase *ReactionDiffusionPanel::getRule(){
    int i, j, k;
    CACore::ReactionDiffusion *p = new CACore::ReactionDiffusion;
    p->diffusionRate[0] = rateSpin[0]->value();
    p->diffusionRate[1] = rateSpin[1]->value();
    p->reactionSpeed = speedSpin->value();
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            for (k = 0; k < 2; k++)
                p->param[i][j][k] = paramSpin[i][j][k]->value();
    return p;
}

ShaderBase *ReactionDiffusionPanel::getShader(){
    return new ReactionDiffusionShader;
}

}
