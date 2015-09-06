#pragma once

#include <QWidget>
#include <QColor>

namespace CAGUI{

class MargPlate: public QWidget{
private:
    QColor color[4];
    int m_lValue, m_rValue, m_stateCount, lArray[4], rArray[4];
    void updateArray();
    void updateValue();

public:
    MargPlate(QWidget *parent = 0);
    int rValue();
    void setLValue(int p_lValue);
    void setRValue(int p_rValue);
    void setStateCount(int p_stateCount);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
};

}
