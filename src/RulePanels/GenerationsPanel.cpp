#include "GenerationsPanel.h"
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTextBrowser>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QRegExp>
#include <QString>
#include <QChar>
#include <QCheckBox>
#include "GenerationsShader.h"
#include "Generations.h"

namespace CAGUI{

#define GENERATIONS_PRESET_COUNT 44

const QString generationsPreset[GENERATIONS_PRESET_COUNT][2] = {
    {QString("Banners"         ), QString(    "2367/3457/5"  )},
    {QString("BelZhab"         ), QString(      "23/23/8"    )},
    {QString("BelZhab Sediment"), QString(  "145678/23/8"    )},
    {QString("Bloomerang"      ), QString(     "234/34678/24")},
    {QString("Bombers"         ), QString(     "345/24/25"   )},
    {QString("Brain 6"         ), QString(       "6/246/3"   )},
    {QString("Brian's Brain"   ), QString(        "/2/3"     )},
    {QString("Burst"           ), QString( "0235678/3468/9"  )},
    {QString("Burst II"        ), QString(  "235678/3468/9"  )},
    {QString("Caterpillars"    ), QString(  "124567/378/4"   )},
    {QString("Chenille"        ), QString(   "05678/24567/6" )},
    {QString("Circuit Genesis" ), QString(    "2345/1234/8"  )},
    {QString("Cooties"         ), QString(      "23/2/8"     )},
    {QString("Ebb&Flow"        ), QString(  "012478/36/18"   )},
    {QString("Ebb&Flow II"     ), QString(  "012468/37/18"   )},
    {QString("Faders"          ), QString(       "2/2/25"    )},
    {QString("Fireworks"       ), QString(       "2/13/21"   )},
    {QString("Flaming Starbows"), QString(     "347/23/8"    )},
    {QString("Frogs"           ), QString(      "12/34/3"    )},
    {QString("Frozen spirals"  ), QString(     "356/23/6"    )},
    {QString("Glisserati"      ), QString(  "035678/245678/7")},
    {QString("Glissergy"       ), QString(  "035678/245678/5")},
    {QString("Lava"            ), QString(   "12345/45678/8" )},
    {QString("Lines"           ), QString(  "012345/458/3"   )},
    {QString("LivingOn TheEdge"), QString(     "345/3/6"     )},
    {QString("Meteor Guns"     ), QString("01245678/3/8"     )},
    {QString("Nova"            ), QString(   "45678/2478/25" )},
    {QString("OrthoGo"         ), QString(       "3/2/4"     )},
    {QString("Prairie on fire" ), QString(     "345/34/6"    )},
    {QString("RainZha"         ), QString(       "2/23/8"    )},
    {QString("Rake"            ), QString(    "3467/2678/6"  )},
    {QString("SediMental"      ), QString(   "45678/25678/4" )},
    {QString("Snake"           ), QString(   "03467/25/6"    )},
    {QString("SoftFreeze"      ), QString(   "13458/38/6"    )},
    {QString("Spirals"         ), QString(       "2/234/5"   )},
    {QString("Star Wars"       ), QString(     "345/2/4"     )},
    {QString("Sticks"          ), QString(    "3456/2/6"     )},
    {QString("Swirl"           ), QString(      "23/34/8"    )},
    {QString("ThrillGrill"     ), QString(    "1234/34/48"   )},
    {QString("Transers"        ), QString(     "345/26/5"    )},
    {QString("Transers II"     ), QString(    "0345/26/6"    )},
    {QString("Wanderers"       ), QString(     "345/34678/5" )},
    {QString("Worms"           ), QString(    "3467/25/6"    )},
    {QString("Xtasy"           ), QString(    "1456/2356/16" )}
};

GenerationsPanel::GenerationsPanel(QWidget *parent): RulePanel(parent){
    int i, j;
    presetLabel = new QLabel("Preset:");
    ruleStringLabel = new QLabel("RuleString:");
    bornLabel = new QLabel("Born:");
    surviveLabel = new QLabel("Survive:");
    stateCountLabel = new QLabel("Number of states:");
    ruleStringEdit = new QLineEdit("345/2/4");
    presetCombo = new QComboBox;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 9; j++)
            buttonArray[i][j] = new QPushButton(QString("%1").arg(j));
    stateCountSpin = new QSpinBox;
    descriptionBrowser = new QTextBrowser;
    ruleStringRegExp = QRegExp("[0-8]*/[0-8]*/[0-8]+");
    hexCheck = new QCheckBox("Use hexagonal neighbourhood");

    for (i = 0; i < 2; i++)
        for (j = 0; j < 9; j++){
            buttonArray[i][j]->setFixedSize(25, 25);
            buttonArray[i][j]->setCheckable(true);
        }
    for (i = 0; i < GENERATIONS_PRESET_COUNT; i++)
        presetCombo->addItem(generationsPreset[i][0]);
    stateCountSpin->setRange(3, 256);
    updateFromRuleString();

    QGridLayout *layoutU = new QGridLayout;
    layoutU->addWidget(presetLabel, 0, 0);
    layoutU->addWidget(presetCombo, 0, 1);
    layoutU->addWidget(ruleStringLabel, 1, 0);
    layoutU->addWidget(ruleStringEdit, 1, 1);
    QGridLayout *layoutM = new QGridLayout;
    layoutM->addWidget(surviveLabel, 0, 0);
    layoutM->addWidget(bornLabel, 1, 0);
    QHBoxLayout *layoutD = new QHBoxLayout;
    layoutD->addWidget(stateCountLabel);
    layoutD->addWidget(stateCountSpin);
    layoutD->addStretch();
    for (i = 0; i < 2; i++)
        for (j = 0; j < 9; j++)
            layoutM->addWidget(buttonArray[i][j], i, j + 1);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layoutU);
    mainLayout->addLayout(layoutM);
    mainLayout->addLayout(layoutD);
    mainLayout->addWidget(hexCheck);
    mainLayout->addWidget(descriptionBrowser);
    setLayout(mainLayout);

    connect(presetCombo, SIGNAL(activated(int)), this, SLOT(updateFromPreset(int)));
    connect(ruleStringEdit, SIGNAL(editingFinished()), this, SLOT(updateFromRuleString()));
    for (i = 0; i < 2; i++)
        for (j = 0; j < 9; j++)
            connect(buttonArray[i][j], SIGNAL(clicked()), this, SLOT(updateFromButton()));
    connect(stateCountSpin, SIGNAL(valueChanged(int)), this, SLOT(updateFromButton()));
}

void GenerationsPanel::updateFromPreset(int index){
    int i, j, t1, t2, l, m;
    QString str = generationsPreset[index][1];
    ruleStringEdit->setText(str);
    l = str.length();
    t1 = str.indexOf('/');
    t2 = str.indexOf('/', t1 + 1);
    for (i = 0; i < 2; i++)
        for (j = 0; j < 9; j++)
            buttonArray[i][j]->setChecked(false);
    for (i = 0; i < t1; i++)
        buttonArray[0][str[i].digitValue()]->setChecked(true);
    for (i = t1 + 1; i < t2; i++)
        buttonArray[1][str[i].digitValue()]->setChecked(true);
    m = str.right(l - t2 - 1).toInt();
    stateCountSpin->setValue(m);
}

void GenerationsPanel::updateFromRuleString(){
    int i, j, t1, t2, l, m;
    QString str, str2;
    str = ruleStringEdit->text();
    if (!ruleStringRegExp.exactMatch(str)){
        updateFromButton();
        return;
    }
    l = str.length();
    t1 = str.indexOf('/');
    t2 = str.indexOf('/', t1 + 1);
    for (i = 0; i < 2; i++)
        for (j = 0; j < 9; j++)
            buttonArray[i][j]->setChecked(false);
    for (i = 0; i < t1; i++)
        buttonArray[0][str[i].digitValue()]->setChecked(true);
    for (i = t1 + 1; i < t2; i++)
        buttonArray[1][str[i].digitValue()]->setChecked(true);
    m = str.right(l - t2 - 1).toInt();
    if (m > 256)
        m = 256;
    if (m < 3)
        m = 3;
    stateCountSpin->setValue(m);
    for (i = 0; i < 9; i++)
        if (buttonArray[0][i]->isChecked())
            str2.append(QChar('0' + i));
    str2.append('/');
    for (i = 0; i < 9; i++)
        if (buttonArray[1][i]->isChecked())
            str2.append(QChar('0' + i));
    str2.append('/').append(QString("%1").arg(m));
    ruleStringEdit->setText(str2);
    for (i = 0; i < GENERATIONS_PRESET_COUNT; i++)
        if (generationsPreset[i][1] == str2){
            presetCombo->setCurrentIndex(i);
            return;
        }
    presetCombo->setCurrentIndex(-1);
}

void GenerationsPanel::updateFromButton(){
    int i;
    QString str;
    for (i = 0; i < 9; i++)
        if (buttonArray[0][i]->isChecked())
            str.append(QChar('0' + i));
    str.append('/');
    for (i = 0; i < 9; i++)
        if (buttonArray[1][i]->isChecked())
            str.append(QChar('0' + i));
    str.append('/').append(QString("%1").arg(stateCountSpin->value()));
    ruleStringEdit->setText(str);
    for (i = 0; i < GENERATIONS_PRESET_COUNT; i++)
        if (generationsPreset[i][1] == str){
            presetCombo->setCurrentIndex(i);
            return;
        }
    presetCombo->setCurrentIndex(-1);
}

CACore::RuleBase *GenerationsPanel::getRule(){
    int i, j;
    CACore::Generations *p;
    if (hexCheck->isChecked())
        p = new CACore::GenerationsHex;
    else
        p = new CACore::Generations;
    p->m_stateCount = stateCountSpin->value();
    for (i = 0; i < 9; i++){
        p->table[0][i] = buttonArray[1][i]->isChecked();
        p->table[1][i] = buttonArray[0][i]->isChecked() ? 1 : 2;
        p->table[p->m_stateCount - 1][i] = 0;
    }
    for (i = 2; i < p->m_stateCount - 1; i++)
        for (j = 0; j < 9; j++)
            p->table[i][j] = i + 1;
    return p;
}

ShaderBase *GenerationsPanel::getShader(){
    GenerationsShader *p = new GenerationsShader;
    p->setStateCount(stateCountSpin->value());
    p->update();
    return p;
}

}
