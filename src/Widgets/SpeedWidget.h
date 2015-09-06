#pragma once

#include <QWidget>

class QSlider;
class QLabel;

namespace CAGUI{

class SpeedWidget: public QWidget{
    Q_OBJECT

private:
    QSlider *slider;
    QLabel *label;

public:
    SpeedWidget(QWidget *parent = 0);

signals:
    void valueChanged(int value);

public slots:
    void process();
};

}
