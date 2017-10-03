#include "unittests.h"
#include "ApplicationWindow.h"
#include "MultiLayer.h"
#include "Graph3D.h"
#include "testPaintDevice.h"
#include "Note.h"
#include "near.h"
#include <QWorkspace>

#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
using namespace std;

void Unittests::plot3d()
{
  unique_ptr<ApplicationWindow> app(open("3dplotTable.sciprj"));
  QVERIFY(app.get());
  unique_ptr<QWidgetList> windows(app->windowsList());
  Table* table=nullptr;
  for (auto i: *windows)
    QVERIFY(!dynamic_cast<Graph3D*>(i));

  for (auto i: *windows)
      if ((table=dynamic_cast<Table*>(i)))
        break;
  app->activateWindow(table);
  table->deselectAll();

  // if nothing is selected, we should get a warning
  unsigned prevWarnings=numWarnings;
  app->plot3DRibbon();
  QCOMPARE(prevWarnings+1,numWarnings);

  // more than one column selected should generate a warning
  table->setCellsSelected(0,1,30,2,true);
  QCOMPARE(table->selectedColumns().count(),2);
  app->activateWindow(table);
  app->plot3DRibbon();
  QCOMPARE(numWarnings,prevWarnings+2);
  
  
  table->deselectAll();
  table->setCellsSelected(0,2,30,2,true);
  app->activateWindow(table);
  app->plot3DRibbon();
  windows.reset(app->windowsList());
  // should be one new Graph3D window, so find it
  Graph3D* plot=nullptr;
  for (auto i: *windows)
    if ((plot=dynamic_cast<Graph3D*>(i)))
      break;

  QVERIFY(plot);
  app->closeWindow(plot);
  windows.reset(app->windowsList());
  for (auto i: *windows)
    QVERIFY(!dynamic_cast<Graph3D*>(i));

  // more than one column selected should generate a warning
  table->deselectAll();
  table->setCellsSelected(0,1,30,2,true);
  QCOMPARE(table->selectedColumns().count(),2);
  app->activateWindow(table);
  app->plot3DBars();
  QCOMPARE(numWarnings,prevWarnings+3);
  
  
  table->deselectAll();
  table->setCellsSelected(0,2,30,2,true);
  app->activateWindow(table);
  app->plot3DBars();
  windows.reset(app->windowsList());
  // should be one new Graph3D window, so find it
  plot=nullptr;
  for (auto i: *windows)
    if ((plot=dynamic_cast<Graph3D*>(i)))
      break;

  QVERIFY(plot);
  app->closeWindow(plot);
  windows.reset(app->windowsList());
  for (auto i: *windows)
    QVERIFY(!dynamic_cast<Graph3D*>(i));

   // more than one column selected should generate a warning
  table->deselectAll();
  table->setCellsSelected(0,1,30,2,true);
  QCOMPARE(table->selectedColumns().count(),2);
  app->activateWindow(table);
  app->plot3DScatter();
  QCOMPARE(numWarnings,prevWarnings+4);
  
  
  table->deselectAll();
  table->setCellsSelected(0,2,30,2,true);
  app->activateWindow(table);
  app->plot3DScatter();
  windows.reset(app->windowsList());
  // should be one new Graph3D window, so find it
  plot=nullptr;
  for (auto i: *windows)
    if ((plot=dynamic_cast<Graph3D*>(i)))
      break;

  QVERIFY(plot);
  app->closeWindow(plot);
  windows.reset(app->windowsList());
  for (auto i: *windows)
    QVERIFY(!dynamic_cast<Graph3D*>(i));

   // more than one column selected should generate a warning
  table->deselectAll();
  table->setCellsSelected(0,1,30,2,true);
  QCOMPARE(table->selectedColumns().count(),2);
  app->activateWindow(table);
  app->plot3DTrajectory();
  QCOMPARE(numWarnings,prevWarnings+5);
  
  
  table->deselectAll();
  table->setCellsSelected(0,2,30,2,true);
  app->activateWindow(table);
  app->plot3DTrajectory();
  windows.reset(app->windowsList());
  // should be one new Graph3D window, so find it
  plot=nullptr;
  for (auto i: *windows)
    if ((plot=dynamic_cast<Graph3D*>(i)))
      break;

  QVERIFY(plot);
  app->closeWindow(plot);
  windows.reset(app->windowsList());
  for (auto i: *windows)
    QVERIFY(!dynamic_cast<Graph3D*>(i));


  
}

