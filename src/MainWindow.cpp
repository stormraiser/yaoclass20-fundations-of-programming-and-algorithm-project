#include "MainWindow.h"
#include <QAction>
#include <QIcon>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QTimer>
#include <QTime>
#include <QLabel>
#include <QStatusBar>
#include <QFont>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QInputDialog>
#include <QProgressDialog>
#include "Board.hpp"
#include "StateWidgetUnsignedChar.h"
#include "StateWidgetDoublePair.h"
#include "Types.h"
#include "File.h"

using CACore::DoublePair;

namespace CAGUI{

MainWindow::MainWindow(){
    int i, j;
    typeID = TYPEID_UNSIGNED_CHAR;
    currentTime = 0;
    for (i = 0; i < 1000; i++)
        for (j = 0; j < 257; j++)
            stat[i][j] = 0;

    board = new Board<unsigned char>;
    mainMenuBar = new QMenuBar;
    mainToolBar = new QToolBar;
    fileMenu = new QMenu(tr("&File"));
    optionMenu = new QMenu(tr("&Settings"));
    newAction = new QAction(QIcon(":/icon/new.png"), tr("&New..."), this);
    openAction = new QAction(QIcon(":/icon/open.png"), tr("&Open..."), this);
    saveAction = new QAction(QIcon(":/icon/save.png"), tr("&Save..."), this);
    fastAction = new QAction(QIcon(":/icon/fast.png"), tr("Goto Generation..."), this);
    editAction = new QAction(QIcon(":/icon/edit.png"), tr("&Edit mode"), this);
    ruleAction = new QAction(QIcon(":/icon/settings.png"), tr("&Rule..."), this);
    shaderAction = new QAction(QIcon(":/icon/color.png"), tr("&Shader..."), this);
    randomAction = new QAction(QIcon(":/icon/map.png"), tr("&Randomize..."), this);
    playAction = new QAction(QIcon(":/icon/play.png"), tr("Start"), this);
    stopAction = new QAction(QIcon(":/icon/stop.png"), tr("Stop"), this);
    graphAction = new QAction(QIcon(":/icon/grid.png"), tr("&Graphics..."), this);
    zoomInAction = new QAction(QIcon(":/icon/zoom in.png"), tr("Zoom in"), this);
    zoomOutAction = new QAction(QIcon(":/icon/zoom out.png"), tr("Zoom out"), this);
    mainStatusBar = new QStatusBar;
    gpsLabel = new QLabel("GPS: ---");
    fpsLabel = new QLabel("FPS: ---");
    coordLabel = new QLabel("x = ---, y = ---");
    timeLabel = new QLabel("Generation = 0");
    populationLabel = new QLabel("Population = 0");
    newFileDialog = new NewFileDialog(this);
    ruleDialog = new RuleDialog(this);
    shaderDialog = new ShaderDialog(this);
    randomizeDialog = new RandomizeDialog(this);
    graphDialog = new GraphDialog(this);
    timer = new QTimer;
    speedWidget = new SpeedWidget;
    stateWidget = new StateWidgetUnsignedChar;
    holder = new QWidget;

    stopAction->setEnabled(false);
    timer->setInterval(0);

    QFont tFont = font();
    tFont.setPointSize(10);
    setFont(tFont);
    gpsLabel->setFixedWidth(95);
    fpsLabel->setFixedWidth(95);
    coordLabel->setFixedWidth(215);
    timeLabel->setFixedWidth(215);
    populationLabel->setFixedWidth(215);
    setWindowTitle("StormRaiser's Cellular Automata");

    holderLayout = new QHBoxLayout;
    holderLayout->addWidget(stateWidget);
    holderLayout->setContentsMargins(0, 0, 0, 0);
    holder->setLayout(holderLayout);
    editAction->setCheckable(true);
    mainToolBar->addAction(newAction);
    mainToolBar->addAction(openAction);
    mainToolBar->addAction(saveAction);
    mainToolBar->addAction(randomAction);
    mainToolBar->addAction(editAction);
    mainToolBar->addAction(ruleAction);
    mainToolBar->addAction(shaderAction);
    mainToolBar->addAction(graphAction);
    mainToolBar->addAction(playAction);
    mainToolBar->addAction(stopAction);
    mainToolBar->addAction(fastAction);
    mainToolBar->addAction(zoomInAction);
    mainToolBar->addAction(zoomOutAction);
    mainToolBar->addWidget(speedWidget);
    mainToolBar->addWidget(holder);
    addToolBar(Qt::TopToolBarArea, mainToolBar);
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(randomAction);
    mainMenuBar->addMenu(fileMenu);
    optionMenu->addAction(ruleAction);
    optionMenu->addAction(shaderAction);
    optionMenu->addAction(graphAction);
    mainMenuBar->addMenu(optionMenu);
    setMenuBar(mainMenuBar);
    mainStatusBar->addWidget(gpsLabel);
    mainStatusBar->addWidget(fpsLabel);
    mainStatusBar->addWidget(coordLabel);
    mainStatusBar->addWidget(timeLabel);
    mainStatusBar->addWidget(populationLabel);
    setStatusBar(mainStatusBar);
    setCentralWidget(board);

    connect(newAction, SIGNAL(triggered()), newFileDialog, SLOT(exec()));
    connect(newFileDialog, SIGNAL(applied(NewFileSetting*)), this, SLOT(newFile(NewFileSetting*)));
    connect(ruleAction, SIGNAL(triggered()), ruleDialog, SLOT(exec()));
    connect(ruleDialog, SIGNAL(applied(RuleSetting*)), this, SLOT(setRule(RuleSetting*)));
    connect(shaderDialog, SIGNAL(applied(RuleSetting*)), this, SLOT(setShader(RuleSetting*)));
    connect(playAction, SIGNAL(triggered()), this, SLOT(playProcess()));
    connect(stopAction, SIGNAL(triggered()), this, SLOT(stopProcess()));
    connect(timer, SIGNAL(timeout()), this, SLOT(control()));
    connect(speedWidget, SIGNAL(valueChanged(int)), this, SLOT(setEvolveInterval(int)));
    connect(shaderAction, SIGNAL(triggered()), shaderDialog, SLOT(exec()));
    connect(randomAction, SIGNAL(triggered()), randomizeDialog, SLOT(exec()));
    connect(graphAction, SIGNAL(triggered()), graphDialog, SLOT(exec()));
    connect(editAction, SIGNAL(toggled(bool)), board, SLOT(toggleEdit(bool)));
    connect(randomizeDialog, SIGNAL(applied(double*)), board, SLOT(randomize(double*)));
    connect(graphDialog, SIGNAL(applied(GraphSetting*)), board, SLOT(setGraphSetting(GraphSetting*)));
    connect(board, SIGNAL(coordinateChanged(QString)), coordLabel, SLOT(setText(QString)));
    connect(board, SIGNAL(populationChanged(QString)), populationLabel, SLOT(setText(QString)));
    connect(stateWidget, SIGNAL(valueChanged()), this, SLOT(setActiveState()));
    connect(zoomInAction, SIGNAL(triggered()), board, SLOT(zoomIn()));
    connect(zoomOutAction, SIGNAL(triggered()), board, SLOT(zoomOut()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(fastAction, SIGNAL(triggered()), this, SLOT(fastForward()));

    ruleDialog->confirm();
    graphDialog->confirm();
    speedWidget->process();

    resize(800, 600);
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    switch (event->key()){
    case Qt::Key_Shift:
        editAction->toggle();
        break;
    case Qt::Key_Space:
        currentTime++;
        board->evolve();
        board->render();
        timeLabel->setText(QString("Generation = %1").arg(currentTime));
        populationLabel->setText(QString("Population = %1").arg(board->population()));
        break;
    case Qt::Key_Return:
        if (playAction->isEnabled())
            playProcess();
        else
            stopProcess();
    }
}

void MainWindow::newFile(NewFileSetting *setting){
    board->setup(setting->width, setting->height);
    randomAction->setEnabled((setting->width != 0) && (setting->height != 0));
    currentTime = 0;
    timeLabel->setText(QString("Generation = %1").arg(currentTime));
    populationLabel->setText(QString("Population = %1").arg(board->population()));
}

void MainWindow::setRule(RuleSetting *setting){
    if (setting->rule == NULL)
        return;
    rule = setting->rule;
    if (setting->typeID != typeID){
        typeID = setting->typeID;
        switch (typeID){
        case TYPEID_UNSIGNED_CHAR:
            tBoard = new Board<unsigned char>;
            tStateWidget = new StateWidgetUnsignedChar;
            break;
        case TYPEID_DOUBLE_PAIR:
            tBoard = new Board<DoublePair>;
            tStateWidget = new StateWidgetDoublePair;
            break;
        }

        disconnect(editAction, SIGNAL(toggled(bool)), board, SLOT(toggleEdit(bool)));
        disconnect(randomizeDialog, SIGNAL(applied(double*)), board, SLOT(randomize(double*)));
        disconnect(graphDialog, SIGNAL(applied(GraphSetting*)), board, SLOT(setGraphSetting(GraphSetting*)));
        disconnect(board, SIGNAL(coordinateChanged(QString)), coordLabel, SLOT(setText(QString)));
        disconnect(board, SIGNAL(populationChanged(QString)), populationLabel, SLOT(setText(QString)));
        disconnect(stateWidget, SIGNAL(valueChanged()), this, SLOT(setActiveState()));
        disconnect(zoomInAction, SIGNAL(triggered()), board, SLOT(zoomIn()));
        disconnect(zoomOutAction, SIGNAL(triggered()), board, SLOT(zoomOut()));

        setCentralWidget(tBoard);
        delete board;
        board = tBoard;
        holderLayout->takeAt(0);
        delete stateWidget;
        stateWidget = tStateWidget;
        holderLayout->addWidget(stateWidget);

        connect(editAction, SIGNAL(toggled(bool)), board, SLOT(toggleEdit(bool)));
        connect(randomizeDialog, SIGNAL(applied(double*)), board, SLOT(randomize(double*)));
        connect(graphDialog, SIGNAL(applied(GraphSetting*)), board, SLOT(setGraphSetting(GraphSetting*)));
        connect(board, SIGNAL(coordinateChanged(QString)), coordLabel, SLOT(setText(QString)));
        connect(board, SIGNAL(populationChanged(QString)), populationLabel, SLOT(setText(QString)));
        connect(stateWidget, SIGNAL(valueChanged()), this, SLOT(setActiveState()));
        connect(zoomInAction, SIGNAL(triggered()), board, SLOT(zoomIn()));
        connect(zoomOutAction, SIGNAL(triggered()), board, SLOT(zoomOut()));

        board->setRule(rule);
        stateWidget->setRule(rule);
        shaderDialog->update(setting);
        randomizeDialog->update(setting);
        graphDialog->confirm();
        board->toggleEdit(editAction->isChecked());

        qDebug("  #5");
    }
    else{
        board->setRule(rule);
        stateWidget->setRule(rule);
        shaderDialog->update(setting);
        randomizeDialog->update(setting);
    }
}

void MainWindow::setShader(RuleSetting *setting){
    board->setShader(setting->shader);
    stateWidget->setShader(setting->shader);
}

void MainWindow::playProcess(){
    playAction->setEnabled(false);
    stopAction->setEnabled(true);
    timeStat1 = 0;
    timeStat2 = 0;
    timeStat3 = 0;
    gpsCount = 0;
    fpsCount = 0;
    clock1.restart();
    clock2.restart();
    clock3.restart();
    timer->start();
}

void MainWindow::stopProcess(){
    playAction->setEnabled(true);
    stopAction->setEnabled(false);
    gpsLabel->setText("GPS: ---");
    fpsLabel->setText("FPS: ---");
    timer->stop();
    board->render();
}

void MainWindow::setEvolveInterval(int value){
    evolveInterval = value;
}

void MainWindow::restartTimer(){
    if (stopAction->isEnabled())
        timer->start();
}

void MainWindow::control(){
    int t, i;
    board->evolve();
    gpsCount++;
    currentTime++;
    for (i = 0; i < 256; i++)
        stat[currentTime % 1000][i] = board->population(i);
    stat[currentTime % 1000][256] = board->population();
    t = clock1.restart();
    timeStat1 += evolveInterval - t;
    timeStat2 += t;
    if ((clock2.elapsed() >= 1000) || (((timeStat1 > 0) || (timeStat2 > 0)) && (clock2.elapsed() >= 10))){
        clock2.restart();
        board->render();
        fpsCount++;
        timeStat2 -= clock2.elapsed() * RENDER_TIME_MULTIPLE;
        timeStat1 -= clock1.restart();
        timeLabel->setText(QString("Generation = %1").arg(currentTime));
        populationLabel->setText(QString("Population = %1").arg(board->population()));
    }
    if (timeStat1 > 0){
        timer->stop();
        QTimer::singleShot(timeStat1, Qt::PreciseTimer, this, SLOT(restartTimer()));
    }
    if (timeStat1 < -100000)
        timeStat1 = -100000;
    if (timeStat2 > 100000)
        timeStat2 = 100000;
    if (timeStat2 < -100000)
        timeStat2 = -100000;
    if (clock3.elapsed() >= 1000){
        t = clock3.restart();
        gpsLabel->setText(QString("GPS: %1").arg(gpsCount / double(t) * 1000, 0, 'f', 2));
        fpsLabel->setText(QString("FPS: %1").arg(fpsCount / double(t) * 1000, 0, 'f', 2));
        gpsCount = 0;
        fpsCount = 0;
    }
}

void MainWindow::setActiveState(){
    board->setActiveState(stateWidget->getState());
}

void MainWindow::openFile(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
    if (!fileName.isNull())
        decode(BCAST(unsigned char, board)->getGrid(), fileName.toStdString());
}

void MainWindow::saveFile(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"));
    if (!fileName.isNull())
        Code(BCAST(unsigned char, board)->getGrid(), fileName.toStdString(), rule->stateCount());
}

void MainWindow::fastForward(){
    int i, target = QInputDialog::getInt(this, tr("Goto Generation..."), QString(), currentTime);
    if (target > currentTime){
        QProgressDialog progress("Computing", "Abort", 0, (target - currentTime) / 250 + 1, this);
        progress.setWindowModality(Qt::WindowModal);
        progress.setMinimumDuration(1000);
        for (i = 0; i < target - currentTime; i++){
            board->evolve();
            if (i % 250 == 0)
                progress.setValue(i / 250);
        }
        progress.setValue((target - currentTime) / 250 + 1);
        board->update();
        currentTime = target;
        timeLabel->setText(QString("Generation = %1").arg(currentTime));
    }
}

}
