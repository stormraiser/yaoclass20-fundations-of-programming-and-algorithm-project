#include <QApplication>
#include <cstdlib>
#include <ctime>
#include "MainWindow.h"

int main(int argc, char **argv){
    srand(time(NULL));
    QApplication app(argc, argv);
    CAGUI::MainWindow *window = new CAGUI::MainWindow;
    window->show();
    return app.exec();
}
