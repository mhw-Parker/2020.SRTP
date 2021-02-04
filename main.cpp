#include "mainwindow.h"
#include<iostream>
#include <QApplication>
#include <QDir>
#include "readExcelData.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    QString strPath = QDir::currentPath() + "/车辆参数.xlsx";
    QString strPath = QDir::currentPath() + "/G1108实车运行数据_空气制动力.xls";
    QFile file(strPath);
    if(!file.exists())
       {
           qDebug()<<QDir::currentPath()<<endl;
           qDebug() << "文件不存在";
           return a.exec();
       }
    ExcelRead test;
    test.readExcelData(strPath);

    return a.exec();
}
