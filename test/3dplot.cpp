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
  
  for (auto i: *windows)
    {
      if (auto table=dynamic_cast<Table*>(i))
        {
          app->activateWindow(table);
          table->deselectAll();
          table->setCellsSelected(0,2,30,2,true);
        }
      QVERIFY(!dynamic_cast<Graph3D*>(i));
    }

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
  
}

