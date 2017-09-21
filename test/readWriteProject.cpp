#include "unittests.h"
#include "ApplicationWindow.h"
#include "MultiLayer.h"
#include "Graph3D.h"
#include "testPaintDevice.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
using namespace std;

extern "C"
{
void file_compress(const char  *file, const char  *mode);
}

void Unittests::readWriteProject()
{
  unique_ptr<ApplicationWindow> app(open("testProject.sciprj"));
  QVERIFY(app.get());
  app->saveFolder(app->projectFolder(), "testProject1.sciprj");
  unique_ptr<ApplicationWindow> app1(open("testProject1.sciprj"));
  // TODO check that app1 is the same as app?
  QVERIFY(app1.get());
  file_compress("testProject1.sciprj", "wb9");
  app1.reset(open("testProject1.sciprj.gz"));
  QVERIFY(app1.get());
}

void Unittests::exportTestProject()
{
  {
    unique_ptr<ApplicationWindow> app(open("testProject.sciprj"));
    QVERIFY(app.get());
    unique_ptr<QWidgetList> windows(app->windowsList());
    for (auto i: *windows)
      if (auto w=dynamic_cast<MultiLayer*>(i))
          w->exportSVG(i->windowTitle()+".svg");
  }
  
  {
    unique_ptr<ApplicationWindow> app(open("3dplot.sciprj"));
    QVERIFY(app.get());
    unique_ptr<QWidgetList> windows(app->windowsList());
    for (auto i: *windows)
      if (auto w=dynamic_cast<Graph3D*>(i))
          w->exportImage("3dplot.png");
  }
}

// checks that the large file Origin import problem (ticket #238) remains fixed
void Unittests::largeOriginImport()
{
  unique_ptr<ApplicationWindow> app(importOPJ("Histo.opj"));
  QVERIFY(app.get());
}


