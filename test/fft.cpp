#include "unittests.h"
#include "ApplicationWindow.h"
#include "FFT.h"
#include "MultiLayer.h"
#include <QTest>
#include <QWorkspace>
#include <iostream>
using namespace std;



void Unittests::fft()
{
  Table table(scriptEnv,30,2,"");
  table.setColName(0,"x");
  table.setColName(1,"y");
  
  auto& colX=*table.column(0);
  auto& colY=*table.column(1);
  for (int r=0; r<table.numRows(); ++r)
    {
      colX.setValueAt(r,r);
      colY.setValueAt(r,sin(r));
    }

  Graph graph;
  graph.insertCurve(&table,"x","y",Graph::Line);

  FFT fft1(this,&table,"y");
  fft1.run();
  // FFT creates a hidden table, which we need to find.
  Table* table1=nullptr;
  for (auto i: *windowsList())
    if (auto t=dynamic_cast<Table*>(i))
      {
        table1=t;
        break;
      }

  

  QVERIFY(table1);

  FFT fft2(this,&graph,"y");
  fft2.run();
  Table* table2=nullptr;
  for (auto i: *windowsList())
    if (auto table=dynamic_cast<Table*>(i))
      if (table!=table1)
        {
          table2=table;
          break;
        }

   QVERIFY(table2);
   
   if (!table1 || !table2) return;
   // check for data equality of tables
   QVERIFY(table1->numCols() == table2->numCols());
   QVERIFY(table1->numRows() == table2->numRows());
   for (int c=0; c<table1->numCols(); ++c)
     {
       auto& col1=*table1->column(c);
       auto& col2=*table2->column(c);
       for (int r=0; r<table1->numRows(); ++r)
         QVERIFY(col1.valueAt(r)==col2.valueAt(r));
     }
}
