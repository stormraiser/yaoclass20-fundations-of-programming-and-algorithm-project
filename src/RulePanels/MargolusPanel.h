/**
  * MargolusPanel.h
  * Configuration widget for Margolus rules.
  */

#pragma once

#include "Common.h"
#include "RulePanel.h"
#include "MargPlate.h"

class QLabel;
class QSpinBox;
class QComboBox;
class QScrollArea;
class QWidget;

namespace CAGUI{

class MargolusPanel: public RulePanel{
    Q_OBJECT

private:
    QLabel *presetLabel, *stateCountLabel;
    MargPlate *optionPlate[256];
    QSpinBox *stateCountSpin;
    QComboBox *presetCombo;
    QScrollArea *optionArea;
    QWidget *optionBoard;

public:
    MargolusPanel(QWidget *parent = 0);
    CACore::RuleBase *getRule();
    ShaderBase *getShader();
    int getTypeID(){return TYPEID_UNSIGNED_CHAR;};

public slots:
    void stateCountChanged(int t);
    void loadPreset(int t);
};

}
