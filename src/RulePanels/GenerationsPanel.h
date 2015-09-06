/**
  * GenerationsPanel.h
  * Configuration widget for generations rules.
  */

#pragma once

#include <QWidget>
#include "RulePanel.h"
#include "Rule.h"
#include "Shader.h"

class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;
class QTextBrowser;
class QSpinBox;
class QRegExp;
class QString;
class QCheckBox;

namespace CAGUI{

class GenerationsPanel: public RulePanel{
    Q_OBJECT

private:
    QLabel *ruleStringLabel, *presetLabel, *bornLabel, *surviveLabel, *stateCountLabel;
    QLineEdit *ruleStringEdit;
    QComboBox *presetCombo;
    QPushButton *buttonArray[2][9];
    QSpinBox *stateCountSpin;
    QTextBrowser *descriptionBrowser;
    QCheckBox *hexCheck;
    QRegExp ruleStringRegExp;

public:
    GenerationsPanel(QWidget *parent = 0);
    CACore::RuleBase *getRule();
    ShaderBase *getShader();
    int getTypeID(){return TYPEID_UNSIGNED_CHAR;};

public slots:
    void updateFromPreset(int index);
    void updateFromRuleString();
    void updateFromButton();
};

}
