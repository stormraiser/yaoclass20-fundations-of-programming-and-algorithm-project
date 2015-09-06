/**
  * GraphDialog.h
  * Defines class GraphDialog
  * This dialog is used to set background color, and color and geometry of the grid.
  *
  * More graphics settings to come in the future.
  */

#pragma once

#include <QDialog>

class QCheckBox;
class QSpinBox;
class QPushButton;
class QWidget;

namespace CAGUI{

class ColorButton;

struct GraphSetting{
    bool showGrid1, showGrid2, boardHexFlag;
    QColor grid1Color, grid2Color;
    int gridInterval;
};

class GraphDialog: public QDialog{
    Q_OBJECT

private:
    GraphSetting *setting;
    QCheckBox *grid1Check, *grid2Check, *hexCheck;
    QSpinBox *grid2Spin;
    QPushButton *okButton, *cancelButton;
    ColorButton *grid1Button, *grid2Button;

public:
    GraphDialog(QWidget *parent = 0);

signals:
    void applied(GraphSetting*);

private slots:
    void process(int state);

public slots:
    void confirm();

};

}
