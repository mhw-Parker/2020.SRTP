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
#include <vector>

using namespace std;

class ExcelRead
{
    /**
         * @brief readExcelData 读取Excel数据
         * @return              saveCloseQuit();
         */
public:
      ExcelRead();

      bool datarange_init(QString& filename, int& totalRow, int& totalCol);

      bool readExcelData(QString& filename, vector<vector<float> >Data);
      float Data(int row, int col);

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

private:
      int iRow;
      int iCol;



};
