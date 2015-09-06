#include "RuleDialog.h"
#include "RulePanel.h"
#include "LifeLikePanel.h"
#include "GenerationsPanel.h"
#include "ReactionDiffusionPanel.h"
#include "MargolusPanel.h"
#include "TestPanel.h"
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QFont>
#include <QIcon>

namespace CAGUI{

RuleDialog::RuleDialog(QWidget *parent): QDialog(parent){
    setting = new RuleSetting;
    ruleFamilyLabel = new QLabel("Rule Family:");
    ruleFamilyCombo = new QComboBox;
    okButton = new QPushButton(QIcon(":/images/icon/ok.png"), "OK");
    cancelButton = new QPushButton(QIcon(":/images/icon/cancel.png"), "Cancel");
    rulePanels[0] = new LifeLikePanel;
    rulePanels[1] = new GenerationsPanel;
    rulePanels[2] = new ReactionDiffusionPanel;
    rulePanels[3] = new MargolusPanel;
    rulePanels[4] = new TestPanel;

    ruleFamilyCombo->addItem("Life-like");
    ruleFamilyCombo->addItem("Generations");
    ruleFamilyCombo->addItem("Reaction-diffusion");
    ruleFamilyCombo->addItem("Margolus");
    ruleFamilyCombo->addItem("Special Rules");
    ruleFamilyCombo->setCurrentIndex(0);
    setWindowTitle("Rule Settings");

    QHBoxLayout *layoutU = new QHBoxLayout;
    layoutU->addWidget(ruleFamilyLabel);
    layoutU->addWidget(ruleFamilyCombo);
    layoutU->addStretch();
    layoutM = new QStackedLayout;
    layoutM->addWidget(rulePanels[0]);
    layoutM->addWidget(rulePanels[1]);
    layoutM->addWidget(rulePanels[2]);
    layoutM->addWidget(rulePanels[3]);
    layoutM->addWidget(rulePanels[4]);
    layoutM->setCurrentIndex(0);
    QHBoxLayout *layoutD = new QHBoxLayout;
    layoutD->addStretch();
    layoutD->addWidget(okButton);
    layoutD->addWidget(cancelButton);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layoutU);
    mainLayout->addLayout(layoutM);
    mainLayout->addLayout(layoutD);
    setLayout(mainLayout);
    setFixedSize(sizeHint());

    QFont tFont = font();
    tFont.setPointSize(10);
    setFont(tFont);

    connect(okButton, SIGNAL(clicked()), this, SLOT(confirm()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ruleFamilyCombo, SIGNAL(currentIndexChanged(int)), layoutM, SLOT(setCurrentIndex(int)));
}

void RuleDialog::confirm(){
    accept();
    setting->typeID = ((RulePanel*)(layoutM->currentWidget()))->getTypeID();
    setting->rule = ((RulePanel*)(layoutM->currentWidget()))->getRule();
    setting->shader = ((RulePanel*)(layoutM->currentWidget()))->getShader();
    emit applied(setting);
}

}
