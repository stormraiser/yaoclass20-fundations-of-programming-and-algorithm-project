#include "RandomizeDialog.h"
#include "RuleSetting.h"
#include "Rule.h"
#include <QLabel>
#include <QDoubleSpinBox>
#include <QWidget>
#include <QScrollArea>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIcon>

namespace CAGUI{

RandomizeDialog::RandomizeDialog(QWidget *parent): QDialog(parent){
    int i;
    okButton = new QPushButton(QIcon(":/images/icon/ok.png"), "OK");
    cancelButton = new QPushButton(QIcon(":/images/icon/cancel.png"), "Cancel");
    mainLabel = new QLabel("Percentage of each state:");
    for (i = 1; i < 256; i++){
        optionLabel[i] = new QLabel(QString("#%1").arg(i));
        optionSpin[i] = new QDoubleSpinBox;
    }
    optionArea = new QScrollArea;
    optionBoard = new QWidget;

    for (i = 1; i < 256; i++){
        optionSpin[i]->setRange(0, 100);
        optionSpin[i]->setSingleStep(1);
        optionSpin[i]->setDecimals(2);
    }
    setWindowTitle("Randomized Initialization");

    QGridLayout *layoutM = new QGridLayout;
    for (i = 0; i < 85; i++){
        layoutM->addWidget(optionLabel[i * 3 + 1], i, 0);
        layoutM->addWidget(optionSpin[i * 3 + 1], i, 1);
        layoutM->addWidget(optionLabel[i * 3 + 2], i, 2);
        layoutM->addWidget(optionSpin[i * 3 + 2], i, 3);
        layoutM->addWidget(optionLabel[i * 3 + 3], i, 4);
        layoutM->addWidget(optionSpin[i * 3 + 3], i, 5);
    }
    optionBoard->setLayout(layoutM);
    optionBoard->setFixedWidth(400);
    optionArea->setWidget(optionBoard);
    optionArea->setFixedSize(420, 360);
    QHBoxLayout *layoutD = new QHBoxLayout;
    layoutD->addStretch();
    layoutD->addWidget(okButton);
    layoutD->addWidget(cancelButton);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(mainLabel);
    mainLayout->addWidget(optionArea);
    mainLayout->addLayout(layoutD);
    setLayout(mainLayout);
    setFixedSize(sizeHint());

    connect(okButton, SIGNAL(clicked()), this, SLOT(confirm()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void RandomizeDialog::update(RuleSetting *setting){
    int i;
    stateCount = setting->rule->stateCount();
    for (i = 1; i < stateCount; i++){
        optionLabel[i]->setEnabled(true);
        optionSpin[i]->setEnabled(true);
    }
    for (i = stateCount; i < 256; i++){
        optionLabel[i]->setEnabled(false);
        optionSpin[i]->setEnabled(false);
    }
}

void RandomizeDialog::confirm(){
    int i;
    double s = 0;
    for (i = 1; i < stateCount; i++)
        s += optionSpin[i]->value();
    pos[0] = s > 100 ? 0 : 100 - s;
    for (i = 1; i < stateCount; i++){
        pos[i] = pos[i - 1] + optionSpin[i]->value();
        if (pos[i] > 100)
            pos[i] = 100;
    }
    for (i = stateCount; i < 256; i++)
        pos[i] = 100;
    emit applied(pos);
    accept();
}

}
