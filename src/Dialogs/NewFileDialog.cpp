#include "NewFileDialog.h"

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIntValidator>
#include <QIcon>

namespace CAGUI{

NewFileDialog::NewFileDialog(QWidget *parent): QDialog(parent){
    setting = new NewFileSetting;

    widthLabel = new QLabel("Width:");
    heightLabel = new QLabel("Height:");
    widthEdit = new QLineEdit("100");
    heightEdit = new QLineEdit("100");
    okButton = new QPushButton(QIcon(":/images/icon/ok.png"), "OK");
    cancelButton = new QPushButton(QIcon(":/images/icon/cancel.png"), "Cancel");

    setWindowTitle("New...");

    QGridLayout *layoutU = new QGridLayout;
    layoutU->addWidget(widthLabel, 0, 0);
    layoutU->addWidget(widthEdit, 0, 1);
    layoutU->addWidget(heightLabel, 1, 0);
    layoutU->addWidget(heightEdit, 1, 1);
    QHBoxLayout *layoutD = new QHBoxLayout;
    layoutD->addStretch();
    layoutD->addWidget(okButton);
    layoutD->addWidget(cancelButton);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layoutU);
    mainLayout->addLayout(layoutD);
    setLayout(mainLayout);

    QIntValidator *validator = new QIntValidator(this);
    widthEdit->setValidator(validator);
    heightEdit->setValidator(validator);

    connect(okButton, SIGNAL(clicked()), this, SLOT(confirm()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void NewFileDialog::confirm(){
    accept();
    setting->width = widthEdit->text().toInt();
    setting->height = heightEdit->text().toInt();
    emit applied(setting);
}

}
