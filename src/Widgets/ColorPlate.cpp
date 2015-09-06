#include "ColorPlate.h"
#include <QColor>
#include <QPaintEvent>
#include <QPainter>

namespace CAGUI{

ColorPlate::ColorPlate(QWidget *parent): QWidget(parent){
    color = QColor(0, 0, 0);
}

void ColorPlate::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.fillRect(event->rect(), color);
    painter.setPen(QColor(0, 0, 0));
    painter.drawRect(event->rect().left(), event->rect().top(), event->rect().width() - 1, event->rect().height() - 1);
}

void ColorPlate::setColor(QColor p_color){
    color = p_color;
    update();
}

}
