#include"TrainDynamic.h"
#include <QDir>
#include<iostream>

using namespace std;

Train::Train()
{

}

void Train::filepath()
{
    Car_Parameter_Path = QDir::currentPath() + "/车辆参数.xlsx";

}

bool Train::data_init()
{
    //vector<vector<float> >testdata;
    int row = 0;
    int col = 0;
    QFile file1(Car_Parameter_Path);
    if(!file1.exists())
       {
           qDebug()<<QDir::currentPath()<<endl;
           qDebug() << "文件不存在";
           return false;
       }
    ExcelRead test;
    if(!test.datarange_init(Car_Parameter_Path,row,col))
    {
        cout<<"data range init failed ! "<<endl;
         return false;
    }
    else
    {
        cout<<"The rows & cols : "<<row<<"\t"<<col<<endl;
        vector<vector<float> > TrainData_(row+1,vector<float>(col));
        cout<<"array init successed ! "<<endl;
        test.readExcelData(Car_Parameter_Path,TrainData_);
        return true;
    }
}

bool Train::train_basic_info()
{
    for(int i=2;i<=9;i++)
        Train_TotalLength_ = Train_TotalLength_ + TrainData_[i][3];
    cout<<Train_TotalLength_;
    return 1;
}


void Train::PowerDistribution(float F, float v)
{
    TractionPower = F*v;
    IMSpeed = v*3.6*1000*2.517/60/PI/0.85;
    for(int i=16;i>=0;i=i-2)
    {

    }
}


bool Train::testmain()
{
    filepath();
    cout<<"*****"<<endl;
    data_init();
    qDebug()<<"The target array has been build !";
    train_basic_info();
    return 1;

}

