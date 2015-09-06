/**
  * RuleDialog.h
  * Defines class RuleDialog
  * This dialog is used to select a rule.
  *
  * This dialog serves as a framework. Detailed settings for each family of
  * rule are handeled by separate GUI modules.
  */

#pragma once

#include <QDialog>
#include "Common.h"
#include "RuleSetting.h"

class QLabel;
class QComboBox;
class QPushButton;
class QStackedLayout;

namespace CAGUI{

class RulePanel;

class RuleDialog: public QDialog{
    Q_OBJECT

private:
    RuleSetting *setting;
    QLabel *ruleFamilyLabel;
    QComboBox *ruleFamilyCombo;
    QPushButton *okButton, *cancelButton;
    QStackedLayout *layoutM;
    RulePanel *rulePanels[RULE_FAMILY_COUNT];

public:
    RuleDialog(QWidget *parent = 0);

signals:
    void applied(RuleSetting *setting);

public slots:
    void confirm();
};

}
