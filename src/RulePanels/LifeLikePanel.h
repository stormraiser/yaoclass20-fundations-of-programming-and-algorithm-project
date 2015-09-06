/**
  * LifeLikePanel.h
  * Configuration widget for life-like rules.
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
class QRegExp;
class QString;
class QCheckBox;

namespace CAGUI{

class LifeLikePanel: public RulePanel{
    Q_OBJECT

private:
    QLabel *ruleStringLabel, *presetLabel, *bornLabel, *surviveLabel;
    QLineEdit *ruleStringEdit;
    QComboBox *presetCombo;
    QPushButton *buttonArray[2][9];
    QCheckBox *hexCheck;
    QTextBrowser *descriptionBrowser;
    QRegExp ruleStringRegExp;

public:
    LifeLikePanel(QWidget *parent = 0);
    CACore::RuleBase *getRule();
    ShaderBase *getShader();
    int getTypeID(){return TYPEID_UNSIGNED_CHAR;};

public slots:
    void updateFromPreset(int index);
    void updateFromRuleString();
    void updateFromButton();
};

}
