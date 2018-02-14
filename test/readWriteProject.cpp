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
        {
          w->exportSVG(i->windowTitle()+".svg");
          w->exportImage(i->windowTitle()+".png");
        }
  }
  
  {
    unique_ptr<ApplicationWindow> app(open("3dplot.sciprj"));
    QVERIFY(app.get());
    unique_ptr<QWidgetList> windows(app->windowsList());
    for (auto i: *windows)
      if (auto w=dynamic_cast<Graph3D*>(i))
          w->exportImage(i->windowTitle()+".png");
  }
}

// checks that the large file Origin import problem (ticket #238) remains fixed
void Unittests::largeOriginImport()
{
  unique_ptr<ApplicationWindow> app(importOPJ("Histo.opj"));
  QVERIFY(app.get());
}

void Unittests::convertMatrix()
{
  unique_ptr<ApplicationWindow> app(open("testProject.sciprj"));
  QVERIFY(app.get());
  unique_ptr<QWidgetList> windows(app->windowsList());
  for (auto i: *windows)
    if (auto table=dynamic_cast<Table*>(i))
      {
        QVERIFY(table->name()=="Table1");
        QCOMPARE(table->numRows(),30);
        QCOMPARE(table->numCols(),2);
        app->activateWindow(table);
        QVERIFY(table==app->d_workspace->activeWindow());
        app->convertTableToMatrix();
        // active window should switch to a matrix
        Matrix* matrix=dynamic_cast<Matrix*>(app->d_workspace->activeWindow());
        QVERIFY(matrix);
        QCOMPARE(matrix->numRows(),table->numRows());
        QCOMPARE(matrix->numCols(),table->numCols());
        for (int r=0; r<matrix->numRows(); ++r)
          for (int c=0; c<matrix->numCols(); ++c)
            QVERIFY(near(matrix->cell(r,c),table->cell(r,c),1e-5));
      }
}

void Unittests::note()
{
  Note& note=*newNote();
  activateWindow(&note);
}
