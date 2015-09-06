#include "LifeLikePanel.h"
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTextBrowser>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QRegExp>
#include <QString>
#include <QChar>
#include <QCheckBox>
#include "GeneralShaderUnsignedChar.h"
#include "LifeLike.h"

namespace CAGUI{

#define LIFE_LIKE_PRESET_COUNT 28

const QString lifeLikePreset[LIFE_LIKE_PRESET_COUNT][2] = {
	{QString("2x2"                ), QString(       "125/36"    )},
	{QString("34 Life"            ), QString(        "34/34"    )},
	{QString("Amoeba"             ), QString(      "1358/357"   )},
	{QString("Assimilation"       ), QString(      "4567/345"   )},
	{QString("Coagulations"       ), QString(    "235678/378"   )},
	{QString("Conway's Life"      ), QString(        "23/3"     )},
	{QString("Coral"              ), QString(     "45678/3"     )},
	{QString("Day & Night"        ), QString(     "34678/3678"  )},
	{QString("Diamoeba"           ), QString(      "5678/35678" )},
	{QString("DotLife"            ), QString(       "023/3"     )},
	{QString("DryLife"            ), QString(        "23/37"    )},
	{QString("Fredkin"            ), QString(     "02468/1357"  )},
	{QString("Gnarl"              ), QString(         "1/1"     )},
	{QString("HighLife"           ), QString(        "23/36"    )},
	{QString("Life without death" ), QString( "012345678/3"     )},
	{QString("Live Free or Die"   ), QString(         "0/2"     )},
	{QString("Long Life"          ), QString(         "5/345"   )},
	{QString("Maze"               ), QString(     "12345/3"     )},
	{QString("Mazectric"          ), QString(      "1234/3"     )},
	{QString("Move"               ), QString(       "245/368"   )},
	{QString("Pseudo Life"        ), QString(       "238/357"   )},
	{QString("Replicator"         ), QString(      "1357/1357"  )},
	{QString("Seeds"              ), QString(          "/2"     )},
	{QString("Serviettes"         ), QString(          "/234"   )},
	{QString("Stains"             ), QString(    "235678/3678"  )},
	{QString("Vote"               ), QString(     "45678/5678"  )},
	{QString("Vote 4/5"           ), QString(     "35678/4678"  )},
	{QString("Walled cities"      ), QString(      "2345/45678" )}
};

LifeLikePanel::LifeLikePanel(QWidget *parent): RulePanel(parent){
    int i, j;
    presetLabel = new QLabel("Preset:");
    ruleStringLabel = new QLabel("RuleString:");
    bornLabel = new QLabel("Born:");
    surviveLabel = new QLabel("Survive:");
    ruleStringEdit = new QLineEdit("23/3");
    presetCombo = new QComboBox;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 9; j++)
            buttonArray[i][j] = new QPushButton(QString("%1").arg(j));
    descriptionBrowser = new QTextBrowser;
    ruleStringRegExp = QRegExp("[0-8]*/[0-8]*");
    hexCheck = new QCheckBox("Use hexagonal neighbourhood");

    for (i = 0; i < 2; i++)
        for (j = 0; j < 9; j++){
            buttonArray[i][j]->setFixedSize(25, 25);
            buttonArray[i][j]->setCheckable(true);
        }
    for (i = 0; i < LIFE_LIKE_PRESET_COUNT; i++)
        presetCombo->addItem(lifeLikePreset[i][0]);
    updateFromRuleString();

    QGridLayout *layoutU = new QGridLayout;
    layoutU->addWidget(presetLabel, 0, 0);
    layoutU->addWidget(presetCombo, 0, 1);
    layoutU->addWidget(ruleStringLabel, 1, 0);
    layoutU->addWidget(ruleStringEdit, 1, 1);
    QGridLayout *layoutM = new QGridLayout;
    layoutM->addWidget(surviveLabel, 0, 0);
    layoutM->addWidget(bornLabel, 1, 0);
    for (i = 0; i < 2; i++)
        for (j = 0; j < 9; j++)
            layoutM->addWidget(buttonArray[i][j], i, j + 1);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layoutU);
    mainLayout->addLayout(layoutM);
    mainLayout->addWidget(hexCheck);
    mainLayout->addWidget(descriptionBrowser);
    setLayout(mainLayout);

    connect(presetCombo, SIGNAL(activated(int)), this, SLOT(updateFromPreset(int)));
    connect(ruleStringEdit, SIGNAL(editingFinished()), this, SLOT(updateFromRuleString()));
    for (i = 0; i < 2; i++)
        for (j = 0; j < 9; j++)
            connect(buttonArray[i][j], SIGNAL(clicked()), this, SLOT(updateFromButton()));
}

void LifeLikePanel::updateFromPreset(int index){
    int i, j, t, l;
    QString str = lifeLikePreset[index][1];
    ruleStringEdit->setText(str);
    l = str.length();
    t = str.indexOf('/');
    for (i = 0; i < 2; i++)
        for (j = 0; j < 9; j++)
            buttonArray[i][j]->setChecked(false);
    for (i = 0; i < t; i++)
        buttonArray[0][str[i].digitValue()]->setChecked(true);
    for (i = t + 1; i < l; i++)
        buttonArray[1][str[i].digitValue()]->setChecked(true);
}

void LifeLikePanel::updateFromRuleString(){
    int i, j, t, l;
    QString str, str2;
    str = ruleStringEdit->text();
    if (!ruleStringRegExp.exactMatch(str)){
        updateFromButton();
        return;
    }
    l = str.length();
    t = str.indexOf('/');
    for (i = 0; i < 2; i++)
        for (j = 0; j < 9; j++)
            buttonArray[i][j]->setChecked(false);
    for (i = 0; i < t; i++)
        buttonArray[0][str[i].digitValue()]->setChecked(true);
    for (i = t + 1; i < l; i++)
        buttonArray[1][str[i].digitValue()]->setChecked(true);
    for (i = 0; i < 9; i++)
        if (buttonArray[0][i]->isChecked())
            str2.append(QChar('0' + i));
    str2.append('/');
    for (i = 0; i < 9; i++)
        if (buttonArray[1][i]->isChecked())
            str2.append(QChar('0' + i));
    ruleStringEdit->setText(str2);
    for (i = 0; i < LIFE_LIKE_PRESET_COUNT; i++)
        if (lifeLikePreset[i][1] == str2){
            presetCombo->setCurrentIndex(i);
            return;
        }
    presetCombo->setCurrentIndex(-1);
}

void LifeLikePanel::updateFromButton(){
    int i;
    QString str;
    for (i = 0; i < 9; i++)
        if (buttonArray[0][i]->isChecked())
            str.append(QChar('0' + i));
    str.append('/');
    for (i = 0; i < 9; i++)
        if (buttonArray[1][i]->isChecked())
            str.append(QChar('0' + i));
    ruleStringEdit->setText(str);
    for (i = 0; i < LIFE_LIKE_PRESET_COUNT; i++)
        if (lifeLikePreset[i][1] == str){
            presetCombo->setCurrentIndex(i);
            return;
        }
    presetCombo->setCurrentIndex(-1);
}

CACore::RuleBase *LifeLikePanel::getRule(){
    int i, j;
    if (buttonArray[1][0]->isChecked()){
        CACore::LifeLikeSpecial *p = new CACore::LifeLikeSpecial;
        if (buttonArray[0][8]->isChecked()){
            for (i = 0; i < 2; i++)
                for (j = 0; j < 9; j++)
                    p->table[1 - i][j][0] = p->table[i][8 - j][1] = 1 - int(buttonArray[i][j]->isChecked());
            p->convertFlag = false;
        }
        else{
            for (i = 0; i < 2; i++)
                for (j = 0; j < 9; j++){
                    p->table[i][8 - j][0] = buttonArray[i][j]->isChecked();
                    p->table[1 - i][j][1] = 1 - int(buttonArray[i][j]->isChecked());
                }
            p->convertFlag = true;
        }
        return p;
    }
    else{
        CACore::LifeLike *p;
        if (hexCheck->isChecked())
            p = new CACore::LifeLikeHex;
        else
            p = new CACore::LifeLike;
        for (i = 0; i < 2; i++)
            for (j = 0; j < 9; j++)
                p->table[1 - i][j] = buttonArray[i][j]->isChecked();
        return p;
    }
}

ShaderBase *LifeLikePanel::getShader(){
    GeneralShaderUnsignedChar *p = new GeneralShaderUnsignedChar(2);
    p->setParamName(0, QString("Dead"));
    p->setParamName(1, QString("Living"));
    p->setParamColor(0, QColor(0, 0, 0));
    p->setParamColor(1, QColor(255, 255, 255));
    return p;
}

}
