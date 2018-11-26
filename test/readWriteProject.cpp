#include "ApplicationWindow.h"
#include "MultiLayer.h"
#include "Graph3D.h"
#include "testPaintDevice.h"
#include "Note.h"
#include <QMdiArea>

#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
using namespace std;

#include <UnitTest++/UnitTest++.h>
#include "utils.h"

extern "C"
{
void file_compress(const char  *file, const char  *mode);
}

SUITE(ReadWriteProject)
{
  TEST_FIXTURE(ApplicationWindow, readWriteProject)
    {
      unique_ptr<ApplicationWindow> app(open("testProject.sciprj"));
      CHECK(app.get());
      app->saveFolder(app->projectFolder(), "testProject1.sciprj");
      unique_ptr<ApplicationWindow> app1(open("testProject1.sciprj"));
      // TODO check that app1 is the same as app?
      CHECK(app1.get());
      file_compress("testProject1.sciprj", "wb9");
      app1.reset(open("testProject1.sciprj.gz"));
      CHECK(app1.get());
    }

  TEST_FIXTURE(ApplicationWindow, exportTestProject)
    {
      {
        unique_ptr<ApplicationWindow> app(open("testProject.sciprj"));
        CHECK(app.get());
        QList<MyWidget*> windows = app->windowsList();
        for (auto i: windows)
          if (auto w=dynamic_cast<MultiLayer*>(i))
            {
              w->exportSVG(i->windowTitle()+".svg");
              w->exportImage(i->windowTitle()+".png");
            }
      }
  
      {
        unique_ptr<ApplicationWindow> app(open("3dplot.sciprj"));
        CHECK(app.get());
        QList<MyWidget*> windows = app->windowsList();
        for (auto i: windows)
          if (auto w=dynamic_cast<Graph3D*>(i))
            w->exportImage(i->windowTitle()+".png");
      }
    }

  // checks that the large file Origin import problem (ticket #238) remains fixed
  TEST_FIXTURE(ApplicationWindow, largeOriginImport)
    {
      unique_ptr<ApplicationWindow> app(importOPJ("Histo.opj"));
      CHECK(app.get());
    }

  TEST_FIXTURE(ApplicationWindow, note)
    {
      Note& note=*newNote();
      activateSubWindow(&note);
    }
  TEST_FIXTURE(ApplicationWindow, convertMatrix)
    {
      unique_ptr<ApplicationWindow> app(open("testProject.sciprj"));
      CHECK(app.get());
      QList<MyWidget*> windows = app->windowsList();
      for (auto i: windows)
        if (auto table=dynamic_cast<Table*>(i))
          {
        CHECK(table->name()=="Table1");
        CHECK_EQUAL(30, table->numRows());
        CHECK_EQUAL(2, table->numCols());
        app->activateSubWindow(table);
        CHECK(table==app->d_workspace.activeSubWindow());
        app->convertTableToMatrix();
        // active window should switch to a matrix
        Matrix* matrix=dynamic_cast<Matrix*>(app->d_workspace.activeSubWindow());
        CHECK(matrix);
        CHECK_EQUAL(matrix->numRows(),table->numRows());
        CHECK_EQUAL(matrix->numCols(),table->numCols());
        for (int r=0; r<matrix->numRows(); ++r)
          for (int c=0; c<matrix->numCols(); ++c)
            CHECK_CLOSE(matrix->cell(r,c),table->cell(r,c), 1e-5);
      }
    }

}
