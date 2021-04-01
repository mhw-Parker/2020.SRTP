#ifndef TRAINDYNAMIC_H
#define TRAINDYNAMIC_H

#endif // TRAINDYNAMIC_H

#include<iostream>
#include"readExcelData.h"

#define PI 3.14

class Train
{
public:
    Train();

    void filepath();          //导入的excel文件名
    bool data_init();

    void train_basic_info(); //计算列车的一些基本参数
    bool Ramp_init();        //只是用于判断表格数据是否满足起点和终点条件
    bool Curve_init();       //该函数已废弃，直接导入matlab的数据

    void PID_init();         //速度PID的数组长度初始化
    void PID();              //速度跟踪主函数

    VectorXf Cal_TrainResistance(VectorXf CurX);
    VectorXf cum_sum(VectorXf vec);

    float* PowerDistribution(float F, float v);  //数组参量小标0表示能量损耗，小标1表示投切电机数量
    float* PowerUnDistribution(float F, float v);
    float* newPowerDistribution(float F, float v);

    void testmain();

    
public:
    QString Car_Parameter_Path;
    QString Grd_data_Path;
    QString CrvData_Path;
    QString OptimalData_Path;

    MatrixXf TrainData_ ;
    MatrixXf GrdData_ ;
    MatrixXf CrvData_;
    MatrixXf OptimalData;
    MatrixXf selectData;

    VectorXf Tmp_Step_X;
    VectorXf Tmp_Step_V;
   
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
    int TrainAirBrakeForce = -600;
    int a = 0;
    float Effi_PD_Ave = 0;
    float Effi_PUD_Ave = 0;
    float Energy_Cosum_PD_Total= 0;
    float Energy_Cosum_PUD_Total= 0;

    //vector<float> Tmp_Step_X;
    //vector<float> Tmp_Step_V;
    int Tmp_SoverNum;
    
//    vector<float> Controlspeed;
//    vector<float> Speed;
//    vector<float> Pos;
//    vector<float> ControlDeltaspeed;
//    vector<float> ControlAcc;
//    vector<float> ControlForce;
//    vector<float> TrainTractiveForce;
//    vector<float> TrainBrakeForce;
//    vector<float> ControlPower;
//    vector<float> PlossIM_PD;
//    vector<float> PlossIM_PUD;
//    vector<float> Actmotornum_PD;
//    vector<float> Actmotornum_PUD;
//    vector<float> Efficiency_PD;
//    vector<float> Efficiency_PUD;
//    vector<float> ControlTime;
//    vector<float> Energy_Cosum_PD;
//    vector<float> Energy_Cosum_PUD;


    //待初始化列向量长度，VectorXf貌似不能动态长度

        VectorXf Tmp_W_total;
        VectorXf Speed;
        VectorXf Pos;

        VectorXf Controlspeed;
        VectorXf ControlPos;
        VectorXf ControlDeltaspeed;
        VectorXf ControlAcc;
        VectorXf ControlForce;
        VectorXf ControlTime;
        VectorXf ControlPower;
        VectorXf TrainTractiveForce;
        VectorXf TrainBrakeForce;
        VectorXf PlossIM_PD;
        VectorXf PlossIM_PUD;
        VectorXf Actmotornum_PD;
        VectorXf Actmotornum_PUD;
        VectorXf Efficiency_PD;
        VectorXf Efficiency_PUD;

        VectorXf Energy_Cosum_PD;
        VectorXf Energy_Cosum_PUD;
        VectorXf BasicRunningForce;

        VectorXf mid_pra_1;
        VectorXf mid_pra_2;//用于取Efficiency_PUD的绝对值
        VectorXf mid_pra_3;
        
        float p;
        bool judge_1;
        float* mid_arr_1;
        float* mid_arr_2;



private:
    float wr = 4103.5;          //额定转速r/min
    float Pmax = 600/0.97;      //最大功率kW
    int Pcu_r = 30;
    float Peddy_r = 7.8;
    float Phys_r = 0.8;
    int Pmech_r = 23;
    int Padd_r = 1;             //额定附加损耗kw
    int num = 0;
    int P_min = 1000000;

    int Pa[17];                 //用于存放每次计算电机投切数的数据
    float wa[17];
    float Pcu_a[17];
    float Pcore_a[17];
    float Pmech_a[17];
    float Padd_a[17];
    float PlossIM_Total[17];

private:
    VectorXf TrainCoM;
    VectorXf TrainCoM_Cal; //用于过程计算的
    VectorXf Train_CoM_Grd;
    VectorXf Train_CoM_Crv;
    float TrainTotalLength;
    float TrainTotalWeight;
    int GradientNumber;
    int CurveNumber;
    int TrainNumber;

    
};
