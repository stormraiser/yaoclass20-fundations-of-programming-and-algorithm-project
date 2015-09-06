/**
  * RandomizeDialog.h
  * Defines class RandomizeDialog
  * This dialog is used to set parameters for randomized initialization.
  */

#pragma once

#include <QDialog>

class QScrollArea;
class QWidget;
class QLabel;
class QDoubleSpinBox;
class QPushButton;

namespace CACore{

class RuleBase;

}

namespace CAGUI{

class RuleSetting;

class RandomizeDialog: public QDialog{
    Q_OBJECT

private:
    double pos[256];
    int stateCount;
    QScrollArea *optionArea;
    QWidget *optionBoard;
    QLabel *mainLabel, *optionLabel[256];
    QDoubleSpinBox *optionSpin[256];
    QPushButton *okButton, *cancelButton;

public:
    RandomizeDialog(QWidget *parent = 0);
    void update(RuleSetting *setting);

signals:
    void applied(double*);

public slots:
    void confirm();
};

}
