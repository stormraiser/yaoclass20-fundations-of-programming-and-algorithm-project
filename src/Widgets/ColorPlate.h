#pragma once

#include <QWidget>

class QPaintEvent;
class QColor;

namespace CAGUI{

class ColorPlate: public QWidget{
private:
    QColor color;

public:
    ColorPlate(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void setColor(QColor p_color);
};

}
