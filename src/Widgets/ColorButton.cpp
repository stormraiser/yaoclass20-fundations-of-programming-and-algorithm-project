#include "ColorButton.h"
#include <QPaintEvent>
#include <QPainter>
#include <QColorDialog>
#include <QColor>

namespace CAGUI{

ColorButton::ColorButton(QWidget *parent): QPushButton(parent){
    connect(this, SIGNAL(clicked()), this, SLOT(changeColor()));
}

QColor ColorButton::color(){
    return m_color;
}

void ColorButton::setColor(QColor p_color){
    m_color = p_color;
    update();
}

void ColorButton::paintEvent(QPaintEvent *event){
    QPushButton::paintEvent(event);
    QPainter painter(this);
    painter.setPen(QColor(0, 0, 0));
    painter.fillRect(8, 8, width() - 16, height() - 16, m_color);
    painter.drawRect(8, 8, width() - 16, height() - 16);
}

void ColorButton::changeColor(){
    QColor tmp;
    tmp = QColorDialog::getColor(m_color, this, "Select color", QColorDialog::DontUseNativeDialog);
    if (tmp.isValid())
        m_color = tmp;
    update();
}

}
