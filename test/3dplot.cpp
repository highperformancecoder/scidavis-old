#include "ApplicationWindowTest.h"
#include "MultiLayer.h"
#include "Graph3D.h"
#include "testPaintDevice.h"
#include "Note.h"
#include <QMdiArea>

#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>

#include "utils.h"

TEST_F(ApplicationWindowTest, plot3d)
{
    std::unique_ptr<ApplicationWindow> app(open("3dplotTable.sciprj"));
    EXPECT_TRUE(app.get());
    QList<MyWidget *> windows = app->windowsList();
    Table *table = nullptr;
    for (auto i : windows)
        EXPECT_TRUE(!dynamic_cast<Graph3D *>(i));

    for (auto i : windows)
        if ((table = dynamic_cast<Table *>(i)))
            break;
    app->activateSubWindow(table);
    table->deselectAll();

    // if nothing is selected, we should get a warning
    unsigned prevWarnings = numWarnings;
    app->plot3DRibbon();
    EXPECT_EQ(prevWarnings + 1, numWarnings);

    // more than one column selected should generate a warning
    table->setCellsSelected(0, 1, 30, 2, true);
    EXPECT_EQ(table->selectedColumns().count(), 2);
    app->activateSubWindow(table);
    app->plot3DRibbon();
    EXPECT_EQ(numWarnings, prevWarnings + 2);

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

    EXPECT_TRUE(plot);
    app->closeWindow(plot);
    windows = app->windowsList();
    for (auto i : windows)
        EXPECT_TRUE(!dynamic_cast<Graph3D *>(i));

    // more than one column selected should generate a warning
    table->deselectAll();
    table->setCellsSelected(0, 1, 30, 2, true);
    EXPECT_EQ(2, table->selectedColumns().count());
    app->activateSubWindow(table);
    app->plot3DBars();
    EXPECT_EQ(prevWarnings + 3, numWarnings);

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

    EXPECT_TRUE(plot);
    app->closeWindow(plot);
    windows = app->windowsList();
    for (auto i : windows)
        EXPECT_TRUE(!dynamic_cast<Graph3D *>(i));

    // more than one column selected should generate a warning
    table->deselectAll();
    table->setCellsSelected(0, 1, 30, 2, true);
    EXPECT_EQ(2, table->selectedColumns().count());
    app->activateSubWindow(table);
    app->plot3DScatter();
    EXPECT_EQ(prevWarnings + 4, numWarnings);

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

    EXPECT_TRUE(plot);
    app->closeWindow(plot);
    windows = app->windowsList();
    for (auto i : windows)
        EXPECT_TRUE(!dynamic_cast<Graph3D *>(i));

    // more than one column selected should generate a warning
    table->deselectAll();
    table->setCellsSelected(0, 1, 30, 2, true);
    EXPECT_EQ(2, table->selectedColumns().count());
    app->activateSubWindow(table);
    app->plot3DTrajectory();
    EXPECT_EQ(prevWarnings + 5, numWarnings);

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

    EXPECT_TRUE(plot);
    app->closeWindow(plot);
    windows = app->windowsList();
    for (auto i : windows)
        EXPECT_TRUE(!dynamic_cast<Graph3D *>(i));
}
