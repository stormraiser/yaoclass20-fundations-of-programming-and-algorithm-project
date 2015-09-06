#include "SpeedWidget.h"

#include <QSlider>
#include <QLabel>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSizePolicy>

namespace CAGUI{

#define SPEED_LEVEL_COUNT 29

static const int interval[SPEED_LEVEL_COUNT] = {1000, 800, 640, 500, 400, 320, 250, 200, 160, 125,
                                                 100,  80,  64,  50,  40,  32,  25,  20,  16,  13,
                                                  10,   8,   6,   5,   4,   3,        2,        1, 0};

SpeedWidget::SpeedWidget(QWidget *parent): QWidget(parent){
    label = new QLabel;
    slider = new QSlider(Qt::Horizontal);

    slider->setRange(0, SPEED_LEVEL_COUNT - 1);
    slider->setValue(14);
    slider->setTickPosition(QSlider::NoTicks);
    slider->setFocusPolicy(Qt::NoFocus);
    label->setText("Speed: x25.00");
    //slider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    slider->setFixedWidth(140);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(label);
    mainLayout->addWidget(slider);
    mainLayout->setContentsMargins(3, 3, 3, 3);
    mainLayout->setSpacing(3);
    mainLayout->setAlignment(label, Qt::AlignCenter);
    setLayout(mainLayout);
    setFixedSize(sizeHint());

    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(process()));
}

void SpeedWidget::process(){
    int level = slider->value();
    if (interval[level] == 0)
        label->setText(QString("Speed: unlimited"));
    else{
        double t = double(1000) / interval[level];
        label->setText(QString("Speed: x%1").arg(t, 0, 'f', 2));
    }
    emit valueChanged(interval[level]);
}

}
