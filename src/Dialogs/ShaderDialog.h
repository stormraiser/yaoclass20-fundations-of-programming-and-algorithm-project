/**
  * ShaderDialog.h
  * Defines class ShaderDialog
  * This dialog is used to configure shaders.
  */

#pragma once

#include <QDialog>
#include "ColorButton.h"
#include "RuleSetting.h"
#include "Shader.h"

class QGroupBox;
class QButtonGroup;
class QRadioButton;
class QLabel;
class QScrollArea;
class QWidget;
class QPushButton;

namespace CAGUI{

class ShaderDialog: public QDialog{
    Q_OBJECT

private:
    RuleSetting *setting;
    ShaderBase *defaultShader, *userShader;
    QGroupBox *modeBox;
    QButtonGroup *modeGroup;
    QRadioButton *defaultButton, *userButton;
    QScrollArea *optionArea;
    QWidget *optionBoard;
    QLabel *optionLabel[256];
    QPushButton *okButton, *cancelButton;
    ColorButton *optionButton[256];

    void saveShader(ShaderBase *shader);
    void loadShader(ShaderBase *shader);

public:
    ShaderDialog(QWidget *parent = 0);
    ~ShaderDialog();

signals:
    void applied(RuleSetting *setting);

public slots:
    void confirm();
    void update(RuleSetting *setting);
    void loadProcess();
};

}
