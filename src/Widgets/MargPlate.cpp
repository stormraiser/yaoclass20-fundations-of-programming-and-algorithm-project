#include "MargPlate.h"

#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPolygon>

namespace CAGUI{

MargPlate::MargPlate(QWidget *parent): QWidget(parent){
    color[0] = QColor(0, 0, 0);
    color[1] = QColor(255, 255, 0);
    color[2] = QColor(0, 96, 255);
    color[3] = QColor(255, 64, 32);
    setFixedSize(74, 33);
    m_lValue = 0;
    m_rValue = 0;
    m_stateCount = 2;
    updateArray();
}

void MargPlate::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(QColor(48, 48, 48));
    painter.setBrush(QColor(48, 48, 48));
    painter.fillRect(event->rect(), QColor(255, 255, 255, 0));
    painter.drawLine(3, 3, 29, 3);
    painter.drawLine(3, 16, 29, 16);
    painter.drawLine(3, 29, 29, 29);
    painter.drawLine(3, 3, 3, 29);
    painter.drawLine(16, 3, 16, 29);
    painter.drawLine(29, 3, 29, 29);
    painter.drawLine(44, 3, 70, 3);
    painter.drawLine(44, 16, 70, 16);
    painter.drawLine(44, 29, 70, 29);
    painter.drawLine(44, 3, 44, 29);
    painter.drawLine(57, 3, 57, 29);
    painter.drawLine(70, 3, 70, 29);
    painter.fillRect(4, 4, 12, 12, color[lArray[0]]);
    painter.fillRect(17, 4, 12, 12, color[lArray[1]]);
    painter.fillRect(17, 17, 12, 12, color[lArray[2]]);
    painter.fillRect(4, 17, 12, 12, color[lArray[3]]);
    painter.fillRect(45, 4, 12, 12, color[rArray[0]]);
    painter.fillRect(58, 4, 12, 12, color[rArray[1]]);
    painter.fillRect(58, 17, 12, 12, color[rArray[2]]);
    painter.fillRect(45, 17, 12, 12, color[rArray[3]]);

    QPolygon poly;
    poly.setPoints(3, 33, 9, 33, 23, 40, 16);
    painter.drawPolygon(poly);
}

void MargPlate::updateArray(){
    lArray[0] = m_lValue % m_stateCount;
    lArray[1] = (m_lValue / m_stateCount) % m_stateCount;
    lArray[2] = (m_lValue / m_stateCount / m_stateCount) % m_stateCount;
    lArray[3] = m_lValue / m_stateCount / m_stateCount / m_stateCount;
    rArray[0] = m_rValue % m_stateCount;
    rArray[1] = (m_rValue / m_stateCount) % m_stateCount;
    rArray[2] = (m_rValue / m_stateCount / m_stateCount) % m_stateCount;
    rArray[3] = m_rValue / m_stateCount / m_stateCount / m_stateCount;
}

void MargPlate::updateValue(){
    m_rValue = ((rArray[3] * m_stateCount + rArray[2]) * m_stateCount + rArray[1]) * m_stateCount + rArray[0];
}

void MargPlate::setLValue(int p_lValue){
    m_lValue = p_lValue;
    updateArray();
    update();
}

void MargPlate::setRValue(int p_rValue){
    m_rValue = p_rValue;
    updateArray();
    update();
}

void MargPlate::setStateCount(int p_stateCount){
    m_stateCount = p_stateCount;
    updateArray();
    update();
}

int MargPlate::rValue(){
    return m_rValue;
}

void MargPlate::mousePressEvent(QMouseEvent *event){
    if ((event->x() >= 45) && (event->x() <= 56) && (event->y() >= 4) && (event->y() <= 15))
        rArray[0] = (rArray[0] + 1) % m_stateCount;
    if ((event->x() >= 58) && (event->x() <= 69) && (event->y() >= 4) && (event->y() <= 15))
        rArray[1] = (rArray[1] + 1) % m_stateCount;
    if ((event->x() >= 58) && (event->x() <= 69) && (event->y() >= 17) && (event->y() <= 28))
        rArray[2] = (rArray[2] + 1) % m_stateCount;
    if ((event->x() >= 45) && (event->x() <= 56) && (event->y() >= 17) && (event->y() <= 28))
        rArray[3] = (rArray[3] + 1) % m_stateCount;
    updateValue();
    update();
}

}
