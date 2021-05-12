#include"readExcelData.h"
//#include <QElapsedTimer>

ExcelRead::ExcelRead()
{
    excel = NULL;			//在构造函数中进行初始化操作
    workbooks = NULL;
    workbook = NULL;
    worksheets = NULL;
    worksheet = NULL;
    usedrange = NULL;
    //pWorkBooks = NULL;
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
    usedrange = worksheet->querySubObject("Usedrange");							//获取权限

    iRow = usedrange->property("Row").toInt();             					    //数据起始行数和列数(可以解决不规则Excel)
    iCol = usedrange->property("Column").toInt();
    //cout<<"start_row= "<<iRow<<"\t start_col="<<iCol<<endl;
    totalRow = usedrange->querySubObject("Rows")->property("Count").toInt();    //获取数据总行数
    totalCol = usedrange->querySubObject("Columns")->property("Count").toInt();
    //
    //qvar = usedrange->dynamicCall("Value");
    qvar = usedrange->dynamicCall("Value2");
    delete usedrange;
    //pWorkBooks->dynamicCall("Close(Boolean)",false);
    excel->dynamicCall("Quit(void)");
    delete excel;
    return true;
}

int ExcelRead::getRowRange(QString &filename)
{
    int lastrow;
    int lastcol;
    if(datarange_init(filename,lastrow,lastcol))
    {
        return lastrow-1;
    }
    else return 0;
}

bool ExcelRead::readExcelData(QString& filename,MatrixXf& m)    //读了全表数据
{

    if(datarange_init(filename,totalrow,totalcol))
        cout<<"excel_init successed !"<<endl;
    m = MatrixXf::Zero(totalrow,totalcol);
    QList<QList<QVariant> > vec;
//    QElapsedTimer timer;
//    timer.start();
    QTime startTime = QTime::currentTime();
    // 逐行读取主表
    Var2Qlist(qvar,vec);
    for (int i = iRow+1; i <= totalrow; i++)
    {
        for(int j = iCol; j <= totalcol; j++)
        {
            //m(i-1,j-1) = worksheet->querySubObject("Cells(int,int)",i,j)->dynamicCall(("Value2()")).value<float>();
            m(i-1,j-1) = vec[i-1][j-1].toFloat();
        }

    }
    QTime stopTime = QTime::currentTime();
    int elapsed = startTime.msecsTo(stopTime);
    //qDebug()<<filename<<" data has been put in Matrix, it took: "<<timer.elapsed()<<"ms";
    qDebug()<<filename<<" data has been put in Matrix, it took: "<<elapsed<<"ms";
    qvar.clear();
//    vec.clear();
    //cout << m.rows()<<endl;
    return true;
}

void ExcelRead::Var2Qlist(QVariant var, QList<QList<QVariant> > &qlist)
{
    QVariantList varRows = var.toList();
    const int rowCount = varRows.size();
    QVariantList rowData;
    for(int i=0;i<rowCount;++i)
    {
        rowData = varRows[i].toList();
        qlist.push_back(rowData);
    }
}

//void ExcelRead::Var2Qvec(QVariant var, QVector<QVector<QVariant> > &qvec)
//{
//    QVariantList varRows = var.toList();
//    QVariantList rowData;
//    for(int i=0;i<totalrow;i++)
//    {
//        //rowData = varRows[i].toFloat();
//    }
//}

void ExcelRead::testmain()
{
   QString OptimalData_Path = QDir::currentPath() + "/3.xlsx";
   MatrixXf mat;
   readExcelData(OptimalData_Path,mat);
}
