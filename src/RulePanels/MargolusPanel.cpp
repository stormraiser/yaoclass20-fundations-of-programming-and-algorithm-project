#include "MargolusPanel.h"
#include "Margolus.h"
#include "GeneralShaderUnsignedChar.h"
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QScrollArea>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#define MARGOLUS_PRESET_COUNT 2

namespace CAGUI{

QString margolusPresetName[MARGOLUS_PRESET_COUNT] = {QString("Sand"), QString("DLA")};
int margolusPresetStateCount[MARGOLUS_PRESET_COUNT] = {4, 3};
int margolusPresetParam[MARGOLUS_PRESET_COUNT][256] =
    {  0,  64,   2,  67,  16,  80,  18,  83,   8,  72,  10,  75,  28,  92,  30,  95,
      16,  80,  18,  83,  80,  84,  82,  87,  24,  88,  26,  91,  28,  92,  30,  95,
      32,  96,  34,  99,  36, 100,  38, 103,  40, 104,  42, 107,  44, 108,  46, 111,
      48, 112,  50, 115,  52, 116,  54, 119,  56, 120,  58, 123,  60, 124,  62, 127,
      64,  80,  66,  67,  80,  81,  82,  83,  72,  88,  74,  75,  92,  93,  94,  95,
      80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
      96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
     112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
     128, 129, 130, 131, 144, 145, 146, 147, 136, 137, 138, 139, 156, 157, 158, 159,
     144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
     160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
     176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
     192, 193, 194, 195, 208, 209, 210, 211, 200, 201, 202, 203, 220, 221, 222, 223,
     208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
     224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
     240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255,

       0,   9,   2,  27,   4,   8,   6,   8,   8,   1,  30,  11,  12,  37,  14,  24,
      16,  26,  18,  19,  20,  24,  22,  23,  24,  25,  26,   3,  28,  56,  10,  39,
      32,  33,  34,  62,  36,  13,  38,  31,  40,  41,  42,  43,  44,  72,  46,  47,
      48,  49,  50,  78,  52,  53,  54,  56,  56,  57,  58,  62,  60,  61,  62,  72,
      64,  74,  66,  67,  68,  69,  70,  71,  72,  74,  74,  78,  76,  77,  78,  79,
      80,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};

MargolusPanel::MargolusPanel(QWidget *parent): RulePanel(parent){
    int i, j;
    presetLabel = new QLabel("Preset:");
    stateCountLabel = new QLabel("Number of states:");
    presetCombo = new QComboBox;
    stateCountSpin = new QSpinBox;
    for (i = 0; i < 256; i++)
        optionPlate[i] = new MargPlate;
    optionArea = new QScrollArea;
    optionBoard = new QWidget;

    stateCountSpin->setRange(2, 4);
    stateCountSpin->setValue(2);
    for (i = 0; i < 256; i++)
        optionPlate[i]->setLValue(i);
    for (i = 0; i < MARGOLUS_PRESET_COUNT; i++)
        presetCombo->addItem(margolusPresetName[i]);
    presetCombo->setCurrentIndex(-1);

    QHBoxLayout *layoutU = new QHBoxLayout;
    layoutU->addWidget(presetLabel);
    layoutU->addWidget(presetCombo);
    layoutU->addStretch();
    QHBoxLayout *layoutM = new QHBoxLayout;
    stateCountSpin->setFixedWidth(60);
    layoutM->addWidget(stateCountLabel);
    layoutM->addWidget(stateCountSpin);
    layoutM->addStretch();
    QGridLayout *layoutD = new QGridLayout;
    for (i = 0; i < 64; i++)
        for (j = 0; j < 4; j++)
            layoutD->addWidget(optionPlate[i * 4 + j], i, j);
    layoutD->setColumnStretch(0, 1);
    layoutD->setColumnStretch(1, 1);
    layoutD->setColumnStretch(2, 1);
    layoutD->setColumnStretch(3, 1);
    optionBoard->setLayout(layoutD);
    optionBoard->setFixedWidth(360);
    optionArea->setWidget(optionBoard);
    optionArea->setFixedHeight(300);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layoutU);
    mainLayout->addLayout(layoutM);
    mainLayout->addWidget(optionArea);
    stateCountChanged(2);
    setLayout(mainLayout);

    connect(stateCountSpin, SIGNAL(valueChanged(int)), this, SLOT(stateCountChanged(int)));
    connect(presetCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(loadPreset(int)));
}

void MargolusPanel::stateCountChanged(int t){
    int i, m = t * t * t * t;
    for (i = 0; i < m; i++){
        optionPlate[i]->show();
        optionPlate[i]->setRValue(i);
        optionPlate[i]->setStateCount(t);
    }
    for (i = m; i < 256; i++)
        optionPlate[i]->hide();
    optionBoard->setFixedHeight(optionBoard->sizeHint().height());
}

CACore::RuleBase *MargolusPanel::getRule(){
    int i;
    CACore::Margolus *p = new CACore::Margolus;
    p->m_stateCount = stateCountSpin->value();
    for (i = 0; i < 256; i++)
        p->param[i] = optionPlate[i]->rValue();
    p->update();
    return p;
}

ShaderBase *MargolusPanel::getShader(){
    GeneralShaderUnsignedChar *p = new GeneralShaderUnsignedChar(4);
    p->setParamName(0, "State #0");
    p->setParamName(1, "State #1");
    p->setParamName(2, "State #2");
    p->setParamName(3, "State #3");
    p->setParamColor(0, QColor(0, 0, 0));
    p->setParamColor(1, QColor(255, 255, 0));
    p->setParamColor(2, QColor(0, 96, 255));
    p->setParamColor(3, QColor(255, 64, 32));
    return p;
}

void MargolusPanel::loadPreset(int t){
    int i;
    stateCountSpin->setValue(margolusPresetStateCount[t]);
    for (i = 0; i < 256; i++)
        optionPlate[i]->setRValue(margolusPresetParam[t][i]);
}

}
