#ifndef TRAINDYNAMIC_H
#define TRAINDYNAMIC_H

#include<iostream>
#include<vector>
#include"ExcelRead/readExcelData.h"

#define PI 3.14

class Train
{
public:
    Train();

    /*导入的excel文件名*/
    void filepath();
    bool data_init();

    /*获取列车基本的重量及长度信息*/
    void train_basic_info();

    /*用于判断表格数据是否满足起点和终点条件*/
    bool Ramp_init();

    //bool Curve_init();//该函数已废弃，直接导入matlab的数据

    /*速度PID的数组长度初始化*/
    void PID_init();

    /*速度跟踪主函数*/
    void PID();

    /*计算列车的运行阻力*/
    VectorXf Cal_TrainResistance(VectorXf CurX);

    /* @ brief
     * 列车电机投切算法
     *数组参量小标0表示能量损耗，小标1表示投切电机数量
     */
    float* PowerDistribution(float F, float v);
    float* PowerUnDistribution(float F, float v);
//    void PowerDistribution(float F, float v, float &Ploss, float &ActormotorNum);  //
//    void PowerUnDistribution(float F, float v, float &Ploss, float &ActormotorNum);
    float* newPowerDistribution(float F, float v);
//    void DistributionInit();  //已整合到 PID_init

    /*一些功能函数*/
    VectorXf cum_sum(VectorXf vec);

    void Pra_clear();

    void testmain();


public:

    VectorXf Tmp_Step_X;
    VectorXf Tmp_Step_V;
    VectorXf Actmotornum_PD;
    VectorXf Actmotornum_PUD;
    VectorXf Controlspeed;
    VectorXf ControlPos;
    VectorXf ControlForce;
    VectorXf Energy_Cosum_PD;
    VectorXf Energy_Cosum_PUD;
    VectorXf Efficiency_PD;
    VectorXf Efficiency_PUD;


    float TractionPower;
    float IMSpeed;

    int Actmotor_num;
    float PlossIM_sun;

public:
    int Tmp_SoverNum;

private:
    int TrainPos_Start_ = 0;
    int TrainPos_End_ = 967000;

    int TrainAirBrakeForce = -600;
    int a = 0;
    float Effi_PD_Ave = 0;
    float Effi_PUD_Ave = 0;
    float Energy_Cosum_PD_Total= 0;
    float Energy_Cosum_PUD_Total= 0;

    QString Car_Parameter_Path;
    QString Grd_data_Path;
    QString CrvData_Path;
    QString OptimalData_Path;

    MatrixXf TrainData_ ;
    MatrixXf GrdData_ ;
    MatrixXf CrvData_;
    MatrixXf OptimalData;
    MatrixXf selectData;

    //待初始化列向量长度，VectorXf貌似不能动态长度

        VectorXf Tmp_W_total;
        VectorXf Speed;
        VectorXf Pos;

        VectorXf ControlDeltaspeed;
        VectorXf ControlAcc;

        VectorXf ControlTime;
        VectorXf ControlPower;
        VectorXf TrainTractiveForce;
        VectorXf TrainBrakeForce;
        VectorXf PlossIM_PD;
        VectorXf PlossIM_PUD;

        VectorXf BasicRunningForce;

        VectorXf mid_pra_1;
        VectorXf mid_pra_2;//用于取Efficiency_PUD的绝对值
        VectorXf mid_pra_3;

        float p;
        bool judge_1;
        float* mid_arr_PD;
        float* mid_arr_PUD;



private:
    float wr = 4103.5;          //额定转速r/min
    float Pmax = 600/0.97;      //最大功率kW
    int Pcu_r = 30;
    float Peddy_r = 7.8;
    float Phys_r = 0.8;
    int Pmech_r = 23;
    int Padd_r = 1;             //额定附加损耗kw
    int num = 16;
    int P_min = 100000;

    /*
     *用于存放每次计算电机投切数的数据
     *不能定义数组长度，编译器可能无法区分这是一个成员函数声明还是一个成员变量声明，也就是产生歧义
     *只能在类函数中定义，已经整合到 PID_init 中
     */
    VectorXf Pa;
    VectorXf wa;
    VectorXf Pcu_a;
    VectorXf Pcore_a;
    VectorXf Pmech_a;
    VectorXf Padd_a;
    VectorXf PlossIM_Total;
//    QVector<int> Pa = QVector<int>(17,0);
//    QVector<float> wa = QVector<float>(17,0);
//    QVector<float> Pcu_a = QVector<float>(17,0);
//    QVector<float> Pcore_a = QVector<float>(17,0);
//    QVector<float> Pmech_a = QVector<float>(17,0);
//    QVector<float> Padd_a = QVector<float>(17,0);
//    QVector<float> PlossIM_Total = QVector<float>(17,0);

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

#endif // TRAINDYNAMIC_H
