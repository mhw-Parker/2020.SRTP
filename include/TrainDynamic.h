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

    void filepath();  //导入的excel文件名
    bool data_init();
    bool train_basic_info();

    void PowerDistribution(float F, float v);

    bool testmain();

    
public:
    QString Car_Parameter_Path;
    QString Grd_data_Path;

    vector<vector<float> > TrainData_ ;
    vector<vector<float> > GrdData_ ;
   
    float Train_TotalLength_ = 0;
    float Train_TotalWeight_ = 0;

    float TractionPower;
    float IMSpeed;

    int Actmotor_num;
    float PlossIM_sun;

private:
    float wr = 4103.5;          //额定转速r/min
    float Pmax = 600/0.97;      //最大功率kW
    int Pcu_r = 30;
    float Peddy_r = 7.8;
    float Phys_r = 0.8;
    int Pmech_r = 23;
    int Padd_r = 1;             //额定附加损耗kw

    int Pa[17];
    float wa[17];
    float Pcu_a[17];
    float Pcore_a[17];
    float Pmech_a[17];
    float Padd_a[17];
    float PlossIM_Total[17];

    
};
