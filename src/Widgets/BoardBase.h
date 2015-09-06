#pragma once

#include <QWidget>

namespace CACore{

class RuleBase;

}

namespace CAGUI{

class ShaderBase;
class GraphSetting;

class BoardBase: public QWidget{
    Q_OBJECT

public:
    BoardBase(QWidget *parent = 0): QWidget(parent){};
    virtual ~BoardBase(){};
    virtual void setRule(CACore::RuleBase *p_rule) = 0;
    virtual void setShader(ShaderBase *p_shader) = 0;
    virtual void setup(int w, int h) = 0;
    virtual void render() = 0;
    virtual void setActiveState(void *state) = 0;
    virtual long long getTime() = 0;
    virtual long long population() = 0;
    virtual long long population(int index) = 0;

signals:
    void coordinateChanged(QString str);
    void populationChanged(QString str);
    void timeChanged(QString str);

public slots:
    virtual void clear() = 0;
    virtual void toggleEdit(bool t) = 0;
    virtual void evolve() = 0;
    virtual void randomize(double *pos) = 0;
    virtual void setGraphSetting(GraphSetting *setting) = 0;
    virtual void zoomIn() = 0;
    virtual void zoomOut() = 0;
};

}
