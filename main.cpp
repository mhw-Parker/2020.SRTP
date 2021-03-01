#include "mainwindow.h"
#include<iostream>
#include <QApplication>
#include <QDir>
#include "TrainDynamic.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    vector<vector<float> >testdata;
//    int row = 0;
//    int col = 0;
//    QString strPath = QDir::currentPath() + "/车辆参数.xlsx";
//   // QString strPath = QDir::currentPath() + "/G1108实车运行数据_空气制动力.xls";

//    QFile file(strPath);
//    if(!file.exists())
//       {
//           qDebug()<<QDir::currentPath()<<endl;
//           qDebug() << "文件不存在";
//           return a.exec();
//       }
//    ExcelRead test;
//    if(!test.datarange_init(strPath,row,col))
//    {
//         return a.exec();
//    }
//    else
//    {
//        cout<<"The rows & cols : "<<row<<"\t"<<col<<endl;
//        vector<vector<float> > testdata(row,vector<float>(col));
//        cout<<"array init successed ! "<<endl;
//    }
//    test.readExcelData(strPath,testdata);
//    cout<<testdata[2][2];

    Train train;
    if(!train.testmain())
    {
        return false;
    }
    return a.exec();
}
