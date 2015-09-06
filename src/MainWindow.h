#pragma once

#include <QWidget>
#include <QMainWindow>
#include <QKeyEvent>
#include <QTime>
#include "Rule.h"
#include "ShaderBase.h"
#include "Common.h"
#include "NewFileDialog.h"
#include "RuleDialog.h"
#include "ShaderDialog.h"
#include "RandomizeDialog.h"
#include "GraphDialog.h"
#include "SpeedWidget.h"
#include "StateWidgetBase.h"
#include "BoardBase.h"

class QMenuBar;
class QMenu;
class QToolBar;
class QAction;
class QTimer;
class QLabel;
class QStatusBar;
class QWidget;
class QHBoxLayout;

namespace CAGUI{

class MainWindow: public QMainWindow{
    Q_OBJECT

private:
    int typeID, evolveInterval, timeStat1, timeStat2, timeStat3, gpsCount, fpsCount;
    int currentTime, stat[1000][257];
    BoardBase *board, *tBoard;
    CACore::RuleBase *rule;
    QLabel *gpsLabel, *fpsLabel, *coordLabel, *populationLabel, *timeLabel;
    QMenuBar *mainMenuBar;
    QMenu *fileMenu, *optionMenu;
    QToolBar *mainToolBar;
    QAction *newAction, *editAction, *ruleAction, *playAction, *stopAction;
    QAction *shaderAction, *randomAction, *graphAction, *zoomInAction, *zoomOutAction;
    QAction *openAction, *saveAction, *fastAction;
    QStatusBar *mainStatusBar;
    QTimer *timer;
    QTime clock1, clock2, clock3;
    NewFileDialog *newFileDialog;
    RuleDialog *ruleDialog;
    ShaderDialog *shaderDialog;
    RandomizeDialog *randomizeDialog;
    GraphDialog *graphDialog;
    SpeedWidget *speedWidget;
    StateWidgetBase *stateWidget, *tStateWidget;
    QWidget *holder;
    QHBoxLayout *holderLayout;

public:
    MainWindow();
    void keyPressEvent(QKeyEvent *event);

public slots:
    void newFile(NewFileSetting *setting);
    void setRule(RuleSetting *setting);
    void setShader(RuleSetting *setting);
    void playProcess();
    void stopProcess();
    void control();
    void restartTimer();
    void setEvolveInterval(int value);
    void setActiveState();
    void openFile();
    void saveFile();
    void fastForward();
};

}
