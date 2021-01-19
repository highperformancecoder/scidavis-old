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

#ifdef _WIN32
#include <UnitTest++.h>
#else
#include <UnitTest++/UnitTest++.h>
#endif
#include "utils.h"

SUITE(Plot3d)
{
    TEST_FIXTURE(ApplicationWindow, plot3d)
    {
        unique_ptr<ApplicationWindow> app(open("3dplotTable.sciprj"));
        CHECK(app.get());
        QList<MyWidget *> windows = app->windowsList();
        Table *table = nullptr;
        for (auto i : windows)
            CHECK(!dynamic_cast<Graph3D *>(i));

        for (auto i : windows)
            if ((table = dynamic_cast<Table *>(i)))
                break;
        app->activateSubWindow(table);
        table->deselectAll();

        // if nothing is selected, we should get a warning
        unsigned prevWarnings = numWarnings;
        app->plot3DRibbon();
        CHECK_EQUAL(prevWarnings + 1, numWarnings);

        // more than one column selected should generate a warning
        table->setCellsSelected(0, 1, 30, 2, true);
        CHECK_EQUAL(table->selectedColumns().count(), 2);
        app->activateSubWindow(table);
        app->plot3DRibbon();
        CHECK_EQUAL(numWarnings, prevWarnings + 2);

        table->deselectAll();
        table->setCellsSelected(0, 2, 30, 2, true);
        app->activateSubWindow(table);
        app->plot3DRibbon();
        windows = app->windowsList();
        // should be one new Graph3D window, so find it
        Graph3D *plot = nullptr;
        for (auto i : windows)
            if ((plot = dynamic_cast<Graph3D *>(i)))
                break;

        CHECK(plot);
        app->closeWindow(plot);
        windows = app->windowsList();
        for (auto i : windows)
            CHECK(!dynamic_cast<Graph3D *>(i));

        // more than one column selected should generate a warning
        table->deselectAll();
        table->setCellsSelected(0, 1, 30, 2, true);
        CHECK_EQUAL(2, table->selectedColumns().count());
        app->activateSubWindow(table);
        app->plot3DBars();
        CHECK_EQUAL(prevWarnings + 3, numWarnings);

        table->deselectAll();
        table->setCellsSelected(0, 2, 30, 2, true);
        app->activateSubWindow(table);
        app->plot3DBars();
        windows = app->windowsList();
        // should be one new Graph3D window, so find it
        plot = nullptr;
        for (auto i : windows)
            if ((plot = dynamic_cast<Graph3D *>(i)))
                break;

        CHECK(plot);
        app->closeWindow(plot);
        windows = app->windowsList();
        for (auto i : windows)
            CHECK(!dynamic_cast<Graph3D *>(i));

        // more than one column selected should generate a warning
        table->deselectAll();
        table->setCellsSelected(0, 1, 30, 2, true);
        CHECK_EQUAL(2, table->selectedColumns().count());
        app->activateSubWindow(table);
        app->plot3DScatter();
        CHECK_EQUAL(prevWarnings + 4, numWarnings);

        table->deselectAll();
        table->setCellsSelected(0, 2, 30, 2, true);
        app->activateSubWindow(table);
        app->plot3DScatter();
        windows = app->windowsList();
        // should be one new Graph3D window, so find it
        plot = nullptr;
        for (auto i : windows)
            if ((plot = dynamic_cast<Graph3D *>(i)))
                break;

        CHECK(plot);
        app->closeWindow(plot);
        windows = app->windowsList();
        for (auto i : windows)
            CHECK(!dynamic_cast<Graph3D *>(i));

        // more than one column selected should generate a warning
        table->deselectAll();
        table->setCellsSelected(0, 1, 30, 2, true);
        CHECK_EQUAL(2, table->selectedColumns().count());
        app->activateSubWindow(table);
        app->plot3DTrajectory();
        CHECK_EQUAL(prevWarnings + 5, numWarnings);

        table->deselectAll();
        table->setCellsSelected(0, 2, 30, 2, true);
        app->activateSubWindow(table);
        app->plot3DTrajectory();
        windows = app->windowsList();
        // should be one new Graph3D window, so find it
        plot = nullptr;
        for (auto i : windows)
            if ((plot = dynamic_cast<Graph3D *>(i)))
                break;

        CHECK(plot);
        app->closeWindow(plot);
        windows = app->windowsList();
        for (auto i : windows)
            CHECK(!dynamic_cast<Graph3D *>(i));
    }
}
