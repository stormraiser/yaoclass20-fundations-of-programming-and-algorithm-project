#include "GraphDialog.h"
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIcon>
#include "ColorButton.h"

namespace CAGUI{

GraphDialog::GraphDialog(QWidget *parent): QDialog(parent){
    setting = new GraphSetting;
    grid1Check = new QCheckBox("Show grid");
    grid2Check = new QCheckBox("Show secondary grid with interval");
    hexCheck = new QCheckBox("Draw hexagonal grid for hexagonal rules");
    grid2Spin = new QSpinBox;
    grid1Button = new ColorButton;
    grid2Button = new ColorButton;
    okButton = new QPushButton(QIcon(":/images/icon/ok.png"), "OK");
    cancelButton = new QPushButton(QIcon(":/images/icon/cancel.png"), "Cancel");

    grid2Spin->setRange(2, 500);
    grid2Spin->setValue(10);
    grid1Check->setChecked(true);
    grid2Check->setChecked(true);
    hexCheck->setChecked(true);
    grid1Button->setColor(QColor(32, 32, 32));
    grid1Button->setFixedSize(30, 30);
    grid2Button->setColor(QColor(64, 64, 64));
    grid2Button->setFixedSize(30, 30);
    setWindowTitle("Graphics settings");

    QHBoxLayout *layoutU = new QHBoxLayout;
    layoutU->addWidget(grid1Check);
    layoutU->addWidget(grid1Button);
    layoutU->addStretch();
    QHBoxLayout *layoutM = new QHBoxLayout;
    layoutM->addWidget(grid2Check);
    layoutM->addWidget(grid2Spin);
    layoutM->addWidget(grid2Button);
    layoutM->addStretch();
    QHBoxLayout *layoutD = new QHBoxLayout;
    layoutD->addStretch();
    layoutD->addWidget(okButton);
    layoutD->addWidget(cancelButton);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layoutU);
    mainLayout->addLayout(layoutM);
    mainLayout->addWidget(hexCheck);
    mainLayout->addLayout(layoutD);
    setLayout(mainLayout);
    setFixedSize(sizeHint());

    connect(grid1Check, SIGNAL(stateChanged(int)), this, SLOT(process(int)));
    connect(okButton, SIGNAL(clicked()), this, SLOT(confirm()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void GraphDialog::process(int state){
    grid2Check->setEnabled(state == Qt::Checked);
    grid2Button->setEnabled(state == Qt::Checked);
    grid2Spin->setEnabled(state == Qt::Checked);
}

void GraphDialog::confirm(){
    setting->showGrid1 = grid1Check->checkState() == Qt::Checked;
    setting->showGrid2 = grid2Check->checkState() == Qt::Checked;
    setting->grid1Color = grid1Button->color();
    setting->grid2Color = grid2Button->color();
    setting->gridInterval = grid2Spin->value();
    setting->boardHexFlag = hexCheck->checkState() == Qt::Checked;
    emit applied(setting);
    accept();
}

}
