#pragma once

#include <QWidget>
#include <QPushButton>

class QColor;
class QPaintEvent;

namespace CAGUI{

class ColorButton: public QPushButton{
    Q_OBJECT

private:
    QColor m_color;

public:
    ColorButton(QWidget *parent = 0);
    QColor color();
    void setColor(QColor p_color);
    void paintEvent(QPaintEvent *event);

public slots:
    void changeColor();
};

}
