#ifndef READEXCELDATA_H
#define READEXCELDATA_H

#endif // READEXCELDATA_H

#include <QVariant>				//读取出的数据只能用此类型容器进行存储
#include <ActiveQt/QAxObject>   //Excel
#include <iostream>
#include <QDebug>
#include <string>
#include <stdio.h>
#include <QVector>
#include <QTime>
#include <math.h>
#include <QDir>

#include <QObject>
#include <QAxObject>
#include <QString>
#include <QStringList>
#include <QVariant>

#include<Eigen/Dense>          //尝试用c++的eigen库加快运行速度
#include<Eigen/Geometry>
#include <Eigen/Core>

using namespace std;
using namespace Eigen;

class ExcelRead
{
    /**
         * @brief readExcelData 读取Excel数据
         * @return              saveCloseQuit();
         */
public:
      ExcelRead();

      bool datarange_init(QString& filename, int& totalRow, int& totalCol);
      int getRowRange(QString& filename);                                     //获取excel的尾行

      bool readExcelData(QString& filename,MatrixXf& m);
      float Data(int row, int col);

      void Var2Qlist(QVariant var,QList<QList<QVariant> > &qlist);
      //void Var2Qvec(QVariant var,QVector<QVector<QVariant> > &qvec);

      void testmain();

public:
//      int totalRow; //总行
//      int totalCol; //总列


private:
      QAxObject* excel;                            //操作Excel文件对象(open-save-close-quit)
      QAxObject* workbooks;                        //总工作薄对象
      QAxObject* workbook;                         //操作当前工作薄对象
      QAxObject* worksheets;                       //文件中所有<Sheet>表页
      QAxObject* worksheet;                        //存储第n个sheet对象
      QAxObject* usedrange;                        //存储当前sheet的数据对象
      QAxObject* pWorkBooks;

      QVariant qvar;

private:
      int totalrow = 0;
      int totalcol = 0;

      int iRow;
      int iCol;



};
