#include"TrainDynamic.h"


Train::Train()
{

}

void Train::filepath()
{
    Car_Parameter_Path = QDir::currentPath() + "/车辆参数.xlsx";
    Grd_data_Path = QDir::currentPath() + "/武广上行坡道_绝对里程_带坡长.xlsx";
    CrvData_Path = QDir::currentPath() + "/Curve_init.xlsx";
    OptimalData_Path = QDir::currentPath() + "/2.xlsx";
    cout<<"filepath init successed !"<<endl;
}

bool Train::data_init()
{
    //vector<vector<float> >testdata;

    QFile file1(Car_Parameter_Path);
    QFile file2(Grd_data_Path);
    QFile file3(CrvData_Path);
    if(! (file1.exists()&&file2.exists()&&file3.exists()) )
    {
        qDebug() << "部分文件不存在";
        return false;
    }
    else qDebug() << "文件路径均存在";
    ExcelRead test;
    test.readExcelData(Car_Parameter_Path,TrainData_);
    test.readExcelData(OptimalData_Path,OptimalData);
    test.readExcelData(Grd_data_Path,GrdData_);
    test.readExcelData(CrvData_Path,CrvData_);
    qDebug()<<"The target array has been build !";

    TrainNumber = TrainData_.col(0).size();
    GradientNumber = GrdData_.col(0).size();
    CurveNumber = CrvData_.col(0).size();
    Tmp_SoverNum = OptimalData.col(0).size();
    qDebug() << "TrainNumber = "<<TrainNumber;
    qDebug() << "GradientNumber ="<<GradientNumber;
    qDebug() << "CurveNumber = "<<CurveNumber;
    qDebug() << "Tmp_SoverNum = "<<Tmp_SoverNum;
    return true;
//    if( test.readExcelData(Car_Parameter_Path,TrainData_)&&
//            test.readExcelData(Grd_data_Path,GrdData_)&&
//            test.readExcelData(CrvData_Tmp_Path,CrvData_)&&
//            test.readExcelData(OptimalData_Path,OptimalData) )
//        return true;
//    else return false;


//    if(!test.datarange_init(Car_Parameter_Path,row,col))
//    {
//        cout<<"data range init failed ! "<<endl;
//         return false;
//    }
//    else
//    {
//        cout<<"The rows & cols : "<<row<<"\t"<<col<<endl;
//        //vector<vector<float> > TrainData_(row+1,vector<float>(col));
//        cout<<"array init successed ! "<<endl;
//        test.readExcelData(Car_Parameter_Path,TrainData_);
//        return true;
//    }
}

void Train::train_basic_info()    //获取列车基本的重量及长度信息，或许可以考虑写死提升运算速度
{
    TrainTotalWeight = TrainData_.col(1).sum();
    TrainTotalLength = TrainData_.col(0).sum();
    cout<<"TotalWeight = "<<TrainTotalWeight<<"\tTotalLength = "<<TrainTotalLength<<endl;
    //直接.size()会统计矩阵元素的所有个数，这里只要列数，所以要取列再求长度

}

bool Train::Ramp_init()
{
    ExcelRead Grd;
    int GrdData_Size_ = GrdData_.size();
    //int lastrow = Grd.getRowRange(Grd_data_Path);
    if(GrdData_(1,1) > TrainPos_Start_)             //数据实际位置可能需要修改
    {
        qDebug()<<"Error: [train starting point] smaller than first Grade.";
    }
    if(GrdData_(GrdData_Size_,2) < TrainPos_End_)
    {
        qDebug()<<"Error: [train finishing point] bigger that last Grade.";
    }
    return 1;
}

//bool Train::Curve_init()                            //haven't fineshed，取了数据的一二四列，可以考虑直接处理调整后的表格,matlab中的fix貌似只能多写一个函数代替
//{
//    vector<vector<float> > CrvData_Tmp_;
//    ExcelRead Crv;
//    int CrvData_Size_Tmp_ = Crv.getRowRange(CrvData_Tmp_Path);
//    int CrvData_Size_ = CrvData_Size_Tmp_*2+1;


//}

void Train::PID()
{
    selectData = OptimalData;
    VectorXf ONE = VectorXf::Ones(Tmp_SoverNum);
    Tmp_Step_X = selectData.col(0) - selectData(1,0)*ONE;  //将线路数据整理为初始起点为0
    //Tmp_Step_X = selectData.col(1);
    Tmp_Step_V = selectData.col(1);
    //cout << Tmp_Step_X; //可以正常输出，前两步数据读入，没有问题

    PID_init(); //初始化PID所需要的数组长度

    Tmp_W_total = Cal_TrainResistance(Tmp_Step_X); //此处将原程序段封装成了一个新函数
    //
    //
    //Tmp_W_total = Tmp_W_total.adjoint();
    Pos = Tmp_Step_X * 1000;
    Speed = Tmp_Step_V / 3.6;

    Controlspeed = VectorXf::Ones(Tmp_SoverNum) * 0.01;
    ControlPos = VectorXf::Ones(Tmp_SoverNum) * Pos(1);
    ControlDeltaspeed(1) = Speed(1) - Controlspeed(1);
    ControlAcc(1) = 0;
    ControlForce(1) = 0;
    TrainTractiveForce(1) = 0;
    TrainBrakeForce(1) = 0;
    TrainAirBrakeForce = -600;
    ControlPower(1) = 0;
    PlossIM_PD(1) = 0;
    PlossIM_PUD(1) = 0;
    Actmotornum_PD(1) = 0;
    Actmotornum_PUD(1) = 0;
    Efficiency_PD(1) = 0;
    Efficiency_PUD(1) = 0;
    Effi_PD_Ave = 0;
    Effi_PUD_Ave = 0;
    ControlTime(1) = 0;
    Energy_Cosum_PD(1) = 0;
    Energy_Cosum_PD_Total= 0;
    Energy_Cosum_PUD(1) = 0;
    Energy_Cosum_PUD_Total= 0;
    a = 0;
    BasicRunningForce(1) = 9.81*(0.42+0.0018*Controlspeed(1)*3.6+0.000132*Controlspeed(1)*Controlspeed(1)*3.6*3.6)*Train_TotalWeight_/1000;

    QTime startTime = QTime::currentTime();
    for(int i=1;i <= Tmp_SoverNum-2;i++)
    {
        Controlspeed(i+1) = pow( (Controlspeed(i)*Controlspeed(i) + 2*ControlAcc(i)*(Pos(i+1)-Pos(i))) , 0.5 );
        Controlspeed = Controlspeed.real(); //取复数实部

        ControlDeltaspeed(i+1) =Speed(i+1)-Controlspeed(i+1);
        ControlAcc(i+1) = ControlDeltaspeed(i+1);
        ControlTime(i+1) = 2*(Pos(i+1)-Pos(i))/(Controlspeed(i)+Controlspeed(i+1));

        BasicRunningForce(i+1) = 9.81*(0.42+0.0018*Controlspeed(i+1)*3.6+0.000132*Controlspeed(i+1)*Controlspeed(i+1)*3.6*3.6)*Train_TotalWeight_/1000;
        ControlForce(i+1) = TrainTotalWeight*ControlAcc(i+1)+BasicRunningForce(i+1)+Tmp_W_total(i+1);
        TrainTractiveForce(i+1) = (-0.2*Controlspeed(i+1)*3.6+259.5)*(Controlspeed(i+1)*3.6<150) + 34500/3.6/ Controlspeed(i+1)*(Controlspeed(i+1)*3.6>=150);

        judge_1 = (Controlspeed(i+1)*3.6>=5)&&(Controlspeed(i+1)*3.6<105);

        TrainBrakeForce(i+1) = (-1)*(60*Controlspeed(i+1)*3.6)*(Controlspeed(i+1)*3.6<5) +
                               (-1)*(-0.3*Controlspeed(i+1)*3.6+301.5)*judge_1 +
                               (-1)*28500/3.6/Controlspeed(i+1)*(Controlspeed(i+1)*3.6>=105);

        //
        if(ControlForce(i+1) > TrainTractiveForce(i+1))
        {
            ControlForce(i+1) = TrainTractiveForce(i+1);
            ControlAcc(i+1) = (ControlForce(i+1) - BasicRunningForce(i+1) - Tmp_W_total(i+1))/Train_TotalWeight_;
        }
        ControlPower(i+1) = ControlForce(i+1)*Controlspeed(i+1);

        //
        if(ControlForce(i+1) <= TrainAirBrakeForce)
        {
            ControlForce(i+1) = TrainAirBrakeForce;
            ControlPower(i+1) = TrainBrakeForce(i+1)*Controlspeed(i+1);
            ControlAcc(i+1) = (ControlForce(i+1) - BasicRunningForce(i+1) - Tmp_W_total(i+1))/Train_TotalWeight_;
        }
        else if(ControlForce(i+1)<=TrainBrakeForce(i+1) && ControlForce(i+1)>TrainAirBrakeForce)
        {
            ControlPower(i+1) = TrainBrakeForce(i+1)*Controlspeed(i+1);
        }

        //动力投切时
        mid_arr_1 = PowerDistribution(ControlForce(i+1),Controlspeed(i+1));
        PlossIM_PD(i+1) = mid_arr_1[0];
        Actmotornum_PD(i+1) = mid_arr_1[1];
        if(ControlPower(i+1) >= 60)
        {
            Efficiency_PD(i+1) = ControlPower(i+1)/(PlossIM_PD(i+1)+ControlPower(i+1));
            Energy_Cosum_PD_Total = Energy_Cosum_PD_Total + (PlossIM_PD(i+1)+ControlPower(i+1))*ControlTime(i+1)/3600.0;
            Energy_Cosum_PD(i+1) = Energy_Cosum_PD_Total;
        }
        else if(ControlPower(i+1) <= -60)
        {
            Energy_Cosum_PD(i+1) = Energy_Cosum_PD_Total;
            if(fabs(ControlPower(i+1))-PlossIM_PD(i+1) <= 0)
            {
                Actmotornum_PD(i+1) = 0;
                Efficiency_PD(i+1) = 0;
            }
            else
                Efficiency_PD(i+1) = -(-ControlPower(i+1)-PlossIM_PD(i+1))/((-1)*ControlPower(i+1));
        }
        else
        {
            Actmotornum_PD(i+1) = 0;
            PlossIM_PD(i+1) = 0;
            Efficiency_PD(i+1) = 0;
            Energy_Cosum_PD(i+1) = Energy_Cosum_PD_Total;
        }
        mid_pra_2 = Efficiency_PD.cwiseAbs();
        Effi_PD_Ave = mid_pra_2.sum() / mid_pra_2.size();//求动力未分配后平均效率,应该写循环外面?
        //Effi_PUD_Ave = mean( fabs(Efficiency_PUD) );

        //未动力投切时
        mid_arr_2 = PowerUnDistribution(ControlForce(i+1),Controlspeed(i+1));
        PlossIM_PUD(i+1) = mid_arr_2[0];
        Actmotornum_PUD(i+1) = mid_arr_2[1];
        if(ControlPower(i+1) >= 60)
        {
            Efficiency_PUD(i+1) = ControlPower(i+1)/(PlossIM_PUD(i+1)+ControlPower(i+1));
            Energy_Cosum_PUD_Total = Energy_Cosum_PUD_Total + (PlossIM_PUD(i+1)+ControlPower(i+1))*ControlTime(i+1)/3600.0;
            Energy_Cosum_PUD(i+1) = Energy_Cosum_PUD_Total;
        }
        else if(ControlPower(i+1) <= -60)
        {
            Energy_Cosum_PUD(i+1) = Energy_Cosum_PUD_Total;
            if(fabs(ControlPower(i+1))-PlossIM_PUD(i+1) <= 0)
            {
                Actmotornum_PUD(i+1) = 16;
                Efficiency_PUD(i+1) = 0;
            }
            else
                Efficiency_PUD(i+1) = -(-ControlPower(i+1)-PlossIM_PUD(i+1))/((-1)*ControlPower(i+1));
        }
        else
        {
            Actmotornum_PUD(i+1) = 16;
            PlossIM_PUD(i+1) = 0;
            Efficiency_PUD(i+1) = 0;
            Energy_Cosum_PUD(i+1) = Energy_Cosum_PUD_Total;
        }
        mid_pra_3 = Efficiency_PUD.cwiseAbs();
        Effi_PUD_Ave = mid_pra_3.sum() / mid_pra_3.size();


    }

    QTime stopTime = QTime::currentTime();
    int elapsed = startTime.msecsTo(stopTime);
    qDebug() << elapsed <<" ms";
}

void Train::PID_init()
{
    Tmp_W_total = VectorXf::Zero(Tmp_SoverNum);
    Speed = VectorXf::Zero(Tmp_SoverNum);
    Pos = VectorXf::Zero(Tmp_SoverNum);

    //Controlspeed = VectorXf::Zero(Tmp_SoverNum);
    //ControlPos = VectorXf::Zero(Tmp_SoverNum);
    ControlDeltaspeed = VectorXf::Zero(Tmp_SoverNum);
    ControlAcc = VectorXf::Zero(Tmp_SoverNum);
    ControlForce = VectorXf::Zero(Tmp_SoverNum);
    ControlTime = VectorXf::Zero(Tmp_SoverNum);
    ControlPower = VectorXf::Zero(Tmp_SoverNum);
    TrainTractiveForce = VectorXf::Zero(Tmp_SoverNum);
    TrainBrakeForce = VectorXf::Zero(Tmp_SoverNum);
    PlossIM_PD = VectorXf::Zero(Tmp_SoverNum);
    PlossIM_PUD = VectorXf::Zero(Tmp_SoverNum);

    Actmotornum_PD = VectorXf::Zero(Tmp_SoverNum);
    Actmotornum_PUD = VectorXf::Zero(Tmp_SoverNum);

    Efficiency_PD = VectorXf::Zero(Tmp_SoverNum);
    Efficiency_PUD = VectorXf::Zero(Tmp_SoverNum);

    Energy_Cosum_PD = VectorXf::Zero(Tmp_SoverNum);
    Energy_Cosum_PUD = VectorXf::Zero(Tmp_SoverNum);

    BasicRunningForce = VectorXf::Zero(Tmp_SoverNum);
}

float* Train::PowerUnDistribution(float F, float v) //16个电机一起运行
{
    float* array = new float[2];
    TractionPower = F*v;
    IMSpeed = v*3.6*1000*2.517/60/PI/0.85;
    for(int i=16;i>=16;i=i-2)             //架控型
    {
        Pa[i] = fabs(TractionPower/i);
        if(Pa[i] > Pmax)                //若跟踪功率大于满功
        {
            PlossIM_Total[i] = 0 ; //超出PMAX时，损耗究竟是定义0还是无穷大
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
    array[0] = PlossIM_sun;
    array[1] = Actmotor_num;
    
    return array;
}

float *Train::PowerDistribution(float F, float v)      //初代动力投切判断方案，阶梯递减
{
    float *array = new float[2];
    TractionPower = F*v;
    IMSpeed = v*3.6*1000*2.517/60/PI/0.85;

    for(int i=16;i>=0;i=i-2)             //架控型
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
    array[0] = PlossIM_sun;
    array[1] = Actmotor_num;

    return array;
}

float *Train::newPowerDistribution(float F, float v)
{
    float *array = new float[2];
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
    array[0] = PlossIM_sun;
    array[1] = Actmotor_num;

    return array;
}

VectorXf Train::cum_sum(VectorXf vec)
{
    int arr_length = vec.size();
    VectorXf vec_back(arr_length);
    vec_back(0) = vec(0);
    for(int i=1;i<=arr_length-1;i++)
    {
       vec_back(i) = vec_back(i-1) + vec(i);
    }
   return vec_back;
}

VectorXf Train::Cal_TrainResistance(VectorXf CurX)
{
    VectorXf tmp_total_w = VectorXf::Zero(Tmp_SoverNum);
    TrainCoM = TrainData_.col(0);
    TrainCoM(1) = TrainCoM(1)*0.5;
    VectorXf mid = cum_sum(TrainCoM);
    cout << mid;
    VectorXf one = VectorXf::Ones(TrainCoM.col(0).size());
    float Wc;
    float Wg;
    for(int j=1;j<=Tmp_SoverNum-1;j++)
    {
        Train_CoM_Grd = VectorXf::Zero(TrainNumber);
        Train_CoM_Crv = VectorXf::Zero(TrainNumber);
        int Cur = CurveNumber - 1;
        int Grd = GradientNumber - 1;   //注意数组下标，避免越界报错
        //cout << Cur <<"\t"<< Grd << endl;
        int i = 0;
        TrainCoM_Cal = CurX(j)*one*1000 - mid;
        //Ramp resistance, Unit(N/kN)
        for(i = 1;i<=TrainNumber-1;i++)
        {
            while(Grd>0)
            {
                if(TrainCoM_Cal(i) < GrdData_(Grd,0))
                    Grd = Grd - 1;
                else if(GrdData_(Grd,0) <= TrainCoM_Cal(i) && TrainCoM_Cal(i)<=GrdData_(Grd,1))
                    break;
            }
            if(Grd>0)
                Train_CoM_Grd(i) = GrdData_(Grd,2);
            else
                Train_CoM_Grd(i) = 0;
        }
        Wg = 9.81*TrainData_.col(1).adjoint()*Train_CoM_Grd;
        Wg = Wg / 1000;

        //Curve resistance, Unit(N/kN)
        for(i = 1;i<=TrainNumber-1;i++)
        {
            while(Cur>0)
            {
                if(TrainCoM_Cal(i) < CrvData_(Cur,0))
                    Cur = Cur - 1;
                else if(CrvData_(Cur,0)<= TrainCoM_Cal(i) && TrainCoM_Cal(i) <= CrvData_(Cur,1))
                    break;
            }
        }
        for(i = 1;i<=TrainNumber-1;i++)
            if(Train_CoM_Crv(i)!=0)
                Train_CoM_Crv(i) = 600/Train_CoM_Crv(i);
        Wc = 9.81*TrainData_.col(1).adjoint()*Train_CoM_Crv;
        Wc = Wc / 1000;
        //Total
        tmp_total_w[j] = Wg + Wc;
        qDebug()<<tmp_total_w[j];

    }
    return tmp_total_w;
}

void Train::testmain()
{
    filepath();
    data_init();
    train_basic_info();
    PID();
    cout << Actmotornum_PUD;
    //VectorXf vec = TrainData_.col(1);//vector为列向量
    cout << TrainData_.col(1).size() <<endl;
    //cout << vec;
    //cout << TrainData_;
    //return 1;

}

