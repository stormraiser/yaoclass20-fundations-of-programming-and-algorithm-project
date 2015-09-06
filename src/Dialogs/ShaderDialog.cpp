#include "ShaderDialog.h"

#include <QButtonGroup>
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QScrollArea>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include <QIcon>
#include "ColorButton.h"
#include "Common.h"
#include "GeneralShaderUnsignedChar.h"

namespace CAGUI{

ShaderDialog::ShaderDialog(QWidget *parent): QDialog(parent){
    int i;
    setting = new RuleSetting;
    setting->typeID = TYPEID_UNSIGNED_CHAR;
    setting->shader = NULL;
    userShader = new GeneralShaderUnsignedChar;
    userShader->setParamCount(256);
    for (i = 0; i < 256; i++){
        userShader->setParamName(i, QString("State #%1").arg(i));
        userShader->setParamColor(i, QColor(0, 0, 0));
    }
    defaultShader = new GeneralShaderUnsignedChar(*((GeneralShaderUnsignedChar*)(userShader)));

    modeBox = new QGroupBox("Shading mode");
    modeGroup = new QButtonGroup;
    defaultButton = new QRadioButton("Use rule-specific settings");
    userButton = new QRadioButton("Use user-defined colors");
    optionBoard = new QWidget;
    optionArea = new QScrollArea;
    for (i = 0; i < 256; i++){
        optionLabel[i] = new QLabel;
        optionButton[i] = new ColorButton;
    }
    okButton = new QPushButton(QIcon(":/images/icon/ok.png"), "OK");
    cancelButton = new QPushButton(QIcon(":/images/icon/cancel.png"), "Cancel");
    setWindowTitle("Shader Settings");

    modeGroup->addButton(defaultButton);
    modeGroup->addButton(userButton);
    defaultButton->setChecked(true);

    QVBoxLayout *layoutU = new QVBoxLayout;
    layoutU->addWidget(defaultButton);
    layoutU->addWidget(userButton);
    modeBox->setLayout(layoutU);
    QGridLayout *layoutC = new QGridLayout;
    for (i = 0; i < 256; i++)
        optionButton[i]->setFixedSize(30, 30);
    for (i = 0; i < 128; i++){
        layoutC->addWidget(optionLabel[i * 2], i, 0);
        layoutC->addWidget(optionButton[i * 2], i, 1);
        layoutC->addWidget(optionLabel[i * 2 + 1], i, 2);
        layoutC->addWidget(optionButton[i * 2 + 1], i, 3);
    }
    layoutC->setColumnStretch(0, 1);
    layoutC->setColumnStretch(2, 1);
    optionBoard->setLayout(layoutC);
    optionBoard->setFixedWidth(450);
    optionArea->setWidget(optionBoard);
    optionArea->setFixedSize(470, 360);
    QHBoxLayout *layoutD = new QHBoxLayout;
    layoutD->addStretch();
    layoutD->addWidget(okButton);
    layoutD->addWidget(cancelButton);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(modeBox);
    mainLayout->addWidget(optionArea);
    mainLayout->addLayout(layoutD);
    setLayout(mainLayout);
    setFixedSize(sizeHint());

    connect(okButton, SIGNAL(clicked()), this, SLOT(confirm()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(defaultButton, SIGNAL(toggled(bool)), this, SLOT(loadProcess()));
}

ShaderDialog::~ShaderDialog(){
    delete defaultShader;
    delete userShader;
    delete setting;
}

void ShaderDialog::update(RuleSetting *p_setting){
    delete defaultShader;
    defaultShader = p_setting->shader;
    if (defaultButton->isChecked()){
        setting->shader = p_setting->shader;
        loadShader(defaultShader);
        defaultShader->update();
        emit applied(p_setting);
    }
}

void ShaderDialog::saveShader(ShaderBase *shader){
    int i;
    for (i = 0; i < shader->paramCount(); i++)
        shader->setParamColor(i, optionButton[i]->color());
}

void ShaderDialog::loadShader(ShaderBase *shader){
    int i;
    for (i = 0; i < shader->paramCount(); i++){
        optionLabel[i]->setText(shader->paramName(i));
        optionLabel[i]->setEnabled(true);
        optionButton[i]->setColor(shader->paramColor(i));
        optionButton[i]->setEnabled(true);
    }
    for (i = shader->paramCount(); i < 256; i++){
        optionLabel[i]->setText("------");
        optionLabel[i]->setEnabled(false);
        optionButton[i]->setColor(QColor(160, 160, 160));
        optionButton[i]->setEnabled(false);
    }
}

void ShaderDialog::confirm(){
    if (defaultButton->isChecked()){
        saveShader(defaultShader);
        defaultShader->update();
        setting->shader = defaultShader;
    }
    else{
        saveShader(userShader);
        userShader->update();
        setting->shader = userShader;
    }
    accept();
    emit applied(setting);
}

void ShaderDialog::loadProcess(){
    if (defaultButton->isChecked()){
        saveShader(userShader);
        loadShader(defaultShader);
    }
    else{
        saveShader(defaultShader);
        loadShader(userShader);
    }
}

}
