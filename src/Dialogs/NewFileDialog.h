/**
  * NewFileDialog.h
  * Defines class NewFileDialog
  * This dialog is used to create a new grid, given its width and height.
  */

#pragma once

#include <QDialog>

class QWidget;
class QLabel;
class QLineEdit;
class QPushButton;

namespace CAGUI{

struct NewFileSetting{
    int width, height;
};

class NewFileDialog: public QDialog{
    Q_OBJECT

private:
    NewFileSetting *setting;
    QLabel *widthLabel, *heightLabel;
    QLineEdit *widthEdit, *heightEdit;
    QPushButton *okButton, *cancelButton;

public:
    NewFileDialog(QWidget *parent = 0);

signals:
    void applied(NewFileSetting*);

public slots:
    void confirm();
};

}
