#ifndef TRAINDYNAMIC_H
#define TRAINDYNAMIC_H

#endif // TRAINDYNAMIC_H

#include<iostream>
#include"readExcelData.h"

using namespace std;

#define PI 3.14

class Train
{
public:
    Train();

    void filepath();   //导入的excel文件名
    bool data_init();
    bool train_basic_info();
    bool Ramp_init();  //坡道参数初始化
    bool Curve_init(); //曲线参数初始化

    int PowerDistribution(float F, float v);
    int newPowerDistribution(float F, float v);

    bool testmain();

    
public:
    QString Car_Parameter_Path;
    QString Grd_data_Path;
    QString CrvData_Tmp_Path;
    QString OptimalData_Path;

    vector<vector<float> > TrainData_ ;
    vector<vector<float> > GrdData_ ;
    vector<vector<float> > CrvData_Tmp_;
    vector<vector<float> > OptimalData;
    vector<vector<float> > selectData;
   
    float Train_TotalLength_ = 0;
    float Train_TotalWeight_ = 0;

    float TractionPower;
    float IMSpeed;

    int Actmotor_num;
    float PlossIM_sun;

public:
    int TrainPos_Start_ = 0;
    int TrainPos_End_ = 967000;


private:
    float wr = 4103.5;          //额定转速r/min
    float Pmax = 600/0.97;      //最大功率kW
    int Pcu_r = 30;
    float Peddy_r = 7.8;
    float Phys_r = 0.8;
    int Pmech_r = 23;
    int Padd_r = 1;             //额定附加损耗kw
    int num = 0;
    int P_min = 100000;

    int Pa[17];                 //用于存放每次计算电机投切数的数据
    float wa[17];
    float Pcu_a[17];
    float Pcore_a[17];
    float Pmech_a[17];
    float Padd_a[17];
    float PlossIM_Total[17];

    
};
