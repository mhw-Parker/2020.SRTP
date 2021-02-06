#include"readExcelData.h"
#include<iostream>

using namespace std;

ExcelRead::ExcelRead()
{
    excel = NULL;			//在构造函数中进行初始化操作
    workbooks = NULL;
    workbook = NULL;
    worksheets = NULL;
    worksheet = NULL;
    usedrange = NULL;
}

bool ExcelRead::datarange_init(QString &filename, int& totalRow, int& totalCol)
{
    excel = new QAxObject("Excel.Application");									//创建Excel对象连接驱动
    excel->dynamicCall("SetVisible(bool)",false);								//ture的打开Excel表 false不打开Excel表
    excel->setProperty("DisplayAlerts",false);

    workbooks = excel->querySubObject("WorkBooks");
    workbook = workbooks->querySubObject("Open(const QString&)",filename);		//打开指定Excel
    worksheets = workbook->querySubObject("WorkSheets");            			//获取表页对象
    worksheet = worksheets->querySubObject("Item(int)",1);          			//获取第1个sheet表
    usedrange =worksheet->querySubObject("Usedrange");							//获取权限

    iRow = usedrange->property("Row").toInt();             					//数据起始行数和列数(可以解决不规则Excel)
    iCol = usedrange->property("Column").toInt();

    totalRow = usedrange->querySubObject("Rows")->property("Count").toInt();  //获取数据总行数
    totalCol = usedrange->querySubObject("Columns")->property("Count").toInt();
    //
    return true;
}

bool ExcelRead::readExcelData(QString& filename, vector<vector<float> >Data)
{
    int row = 0;
    int col = 0;
    if(datarange_init(filename,row,col))
        cout<<"datarange_init successed !"<<endl;
    else
        return false;
    double time_start = (double)clock();
    // 逐行读取主表
    for (int i = iRow+1; i <= row; i++)
    {
        for(int j = iCol; j <= col; j++)
        {
            QString data = worksheet->querySubObject("Cells(int,int)",i,j)->dynamicCall(("Value2()")).value<QString>();
            Data[i][j] = data.toFloat();
            string strdata = data.toStdString();
            printf("%s\t",strdata.c_str());
            //qDebug()<< data;
        }
        cout<<endl;
//        QString number = worksheet->querySubObject("Cells(int,int)",i,1)->dynamicCall(("Value2()")).value<QString>();
//        QString name = worksheet->querySubObject("Cells(int,int)",i,2)->dynamicCall(("Value2()")).value<QString>();
//        QString id = worksheet->querySubObject("Cells(int,int)",i,3)->dynamicCall(("Value2()")).value<QString>();
//        QString desc = worksheet->querySubObject("Cells(int,int)",i,4)->dynamicCall(("Value2()")).value<QString>();

//        qDebug() << number << name << id << desc << endl;//打印验证数据
    }
    double time_end = (double)clock();
    cout<<"read data finished, it takes: "<<(time_end - time_start)/1000<<"s"<<endl;
    //return saveCloseQuit();
    return 1;
}

