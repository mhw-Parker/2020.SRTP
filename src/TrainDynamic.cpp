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
    Grd_data_Path = QDir::currentPath() + "武广上行坡道_绝对里程_带坡长.xlsx";
    CrvData_Tmp_Path = QDir::currentPath() + "武广上行曲线_绝对里程.xlsx";
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

bool Train::train_basic_info()    //获取列车基本的重量及长度信息，或许可以考虑写死提升运算速度
{
    for(int i=2;i<=9;i++)
        Train_TotalLength_ = Train_TotalLength_ + TrainData_[i][3];
    cout<<Train_TotalLength_;
    return 1;
}

bool Train::Ramp_init()
{
    ExcelRead Grd;
    int lastrow = Grd.getRowRange(Grd_data_Path);
    if(GrdData_[1][1] > TrainPos_Start_)             //数据实际位置可能需要修改
    {
        qDebug()<<"Error: [train starting point] smaller than first Grade.";
    }
    if(GrdData_[lastrow][2] < TrainPos_End_)
    {
        qDebug()<<"Error: [train finishing point] bigger that last Grade.";
    }
    return 1;
}

bool Train::Curve_init()                            //haven't fineshed
{
    ExcelRead Crv;
    int lastrow = Crv.getRowRange(CrvData_Tmp_Path);

}

int Train::PowerDistribution(float F, float v)      //初代动力投切判断方案，阶梯递减
{
    TractionPower = F*v;
    IMSpeed = v*3.6*1000*2.517/60/PI/0.85;

    for(int i=16;i>=0;i=i-2)             //轴控型
    {
        Pa[i] = fabs(TractionPower/i);
        if(Pa[i] > Pmax)                //若跟踪功率大于满功
        {
            PlossIM_Total[i] = 0 ;
            continue;
        }
        wa[i] = IMSpeed;
        if(wa[i] < wr/1.8)
        {
            Pcu_a[i] = (Pa[i]/Pmax)*1.8*Pcu_r;
            Pcore_a[i] = ((wa[i]/wr)*(wa[i]/wr)) * Peddy_r + wa[i]/wr*Phys_r;
            Pmech_a[i] = wa[i]/wr * Pmech_r;
            Padd_a[i] = Padd_r;
        }
        else
        {
            Pcu_a[i] = (Pa[i]/Pmax)*(Pa[i]/Pmax)*Pcu_r;
            Pcore_a[i] = Peddy_r + pow((wa[i]/wr),1.6) * Phys_r;
            Pmech_a[i] = wa[i]/wr*Pmech_r;
            Padd_a[i] = Padd_r;
        }

        PlossIM_Total[i] =  i*(Pcu_a[i] + Pcore_a[i] + Pmech_a[i] + Padd_a[i]); //电机总损耗

        if(PlossIM_Total[i]<P_min)
        {
            P_min = PlossIM_Total[i];
            num = i;
        }
    }

    PlossIM_sun = P_min;
    Actmotor_num = num;

    return Actmotor_num;
}

int Train::newPowerDistribution(float F, float v)
{
    TractionPower = F*v;
    IMSpeed = v*3.6*1000*2.517/60/PI/0.85;

    for(int i=16;i>=0;i=i-2)
    {
         Pa[i] = fabs(TractionPower/i);  //实际平均每个电机功率

         if(Pa[i] < Pmax)
         {
             wa[i] = IMSpeed;
             if(wa[i] < wr/1.8)
             {
                 Pcu_a[i] = (Pa[i]/Pmax)*1.8*Pcu_r;
                 Pcore_a[i] = ((wa[i]/wr)*(wa[i]/wr)) * Peddy_r + wa[i]/wr*Phys_r;
                 Pmech_a[i] = wa[i]/wr * Pmech_r;
                 Padd_a[i] = Padd_r;
             }
             else
             {
                 Pcu_a[i] = (Pa[i]/Pmax)*(Pa[i]/Pmax)*Pcu_r;
                 Pcore_a[i] = Peddy_r + pow((wa[i]/wr),1.6) * Phys_r;
                 Pmech_a[i] = wa[i]/wr*Pmech_r;
                 Padd_a[i] = Padd_r;
             }

             PlossIM_Total[i] =  i*(Pcu_a[i] + Pcore_a[i] + Pmech_a[i] + Padd_a[i]); //电机总损耗

             if(PlossIM_Total[i]<P_min)
             {
                 P_min = PlossIM_Total[i];
                 num = i;
             }
             continue;
         }
         else
         {
             if(Pa[i] <= Pmax*1.03)    //判断工作电机减少后，理想功率与实际功率的差值是否在3%以内
             {
                 wa[i] = IMSpeed;
                 if(wa[i] < wr/1.8)
                 {
                     Pcu_a[i] = (Pa[i]/Pmax)*1.8*Pcu_r;
                     Pcore_a[i] = ((wa[i]/wr)*(wa[i]/wr)) * Peddy_r + wa[i]/wr*Phys_r;
                     Pmech_a[i] = wa[i]/wr * Pmech_r;
                     Padd_a[i] = Padd_r;
                 }
                 else
                 {
                     Pcu_a[i] = (Pa[i]/Pmax)*(Pa[i]/Pmax)*Pcu_r;
                     Pcore_a[i] = Peddy_r + pow((wa[i]/wr),1.6) * Phys_r;
                     Pmech_a[i] = wa[i]/wr*Pmech_r;
                     Padd_a[i] = Padd_r;
                 }
                 PlossIM_Total[i] =  i*(Pcu_a[i] + Pcore_a[i] + Pmech_a[i] + Padd_a[i]); //电机总损耗
                 if(PlossIM_Total[i]<P_min)
                 {
                     P_min = PlossIM_Total[i];
                     num = i;
                 }
                 break;
             }
             else break;                 //如果减少电机后整车功率与实际功率相差太远，保持削减前的电机数量

         }
    }

    PlossIM_sun = P_min;
    Actmotor_num = num;

    return Actmotor_num;
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

