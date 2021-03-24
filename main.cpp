#include "mainwindow.h"
#include "TrainDynamic.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Train train;
    train.testmain();
    MainWindow w;
    //w.show();
    return 0;
}
