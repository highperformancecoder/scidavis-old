#include "ApplicationWindowTest.h"
#include "ArrowMarker.h"
#include "MultiLayer.h"
#include <iostream>
#include <thread>

#include "utils.h"

static double sqr(double x)
{
    return x * x;
}

TEST_F(ApplicationWindowTest, arrowMarker)
{
    ArrowMarker arrow;

    auto graph = newGraph();
    auto layer = graph->activeGraph();
    layer->addArrow(&arrow);

    arrow.attach(layer->d_plot);

    arrow.setStartPoint(50, 200);
    arrow.setEndPoint(400, 400);
    arrow.setWidth(2);
    arrow.drawStartArrow(false);
    arrow.drawEndArrow(true);
    layer->replot();

    auto bbox = arrow.rect();

    EXPECT_NEAR(sqrt(sqr(bbox.left() - bbox.right()) + sqr(bbox.top() - bbox.bottom())),
                arrow.length(), 0.1);
    EXPECT_NEAR(sqrt(sqr(bbox.left()) + sqr(bbox.bottom())), arrow.dist(0, 0), 0.1);

    // test interaction with the mouse
    arrow.setEditable(true);
    layer->replot();
    graph->exportImage("arrowEditable.png");
    auto oldr = qreal(bbox.right()), oldl = qreal(bbox.left()), oldt = qreal(bbox.top()),
         oldb = qreal(bbox.bottom());
    auto midx = (oldl + oldr) / 2, midy = (oldb + oldt) / 2;
    {
        // move ending point
        QMouseEvent mousePress(QEvent::MouseButtonPress, { oldr, oldt }, Qt::LeftButton,
                               Qt::LeftButton, Qt::NoModifier);
        QMouseEvent mouseMove(QEvent::MouseMove, { oldr + 10, oldt + 10 }, Qt::NoButton,
                              Qt::LeftButton, Qt::NoModifier);
        QMouseEvent mouseRelease(QEvent::MouseButtonRelease, { oldr + 10, oldt + 10 },
                                 Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        EXPECT_TRUE(arrow.eventFilter(nullptr, &mousePress));
        EXPECT_TRUE(arrow.eventFilter(nullptr, &mouseMove));
        EXPECT_TRUE(arrow.eventFilter(nullptr, &mouseRelease));
        auto newBBox = arrow.rect();
        EXPECT_EQ(bbox.left(), newBBox.left());
        EXPECT_EQ(bbox.right() + 10, newBBox.right());
        EXPECT_EQ(bbox.bottom(), newBBox.bottom());
        EXPECT_EQ(bbox.top() + 10, newBBox.top());
        bbox = newBBox;
    }
    {
        // move starting point
        QMouseEvent mousePress(QEvent::MouseButtonPress, { oldl, oldb }, Qt::LeftButton,
                               Qt::LeftButton, Qt::NoModifier);
        QMouseEvent mouseMove(QEvent::MouseMove, { oldl - 10, oldb - 10 }, Qt::NoButton,
                              Qt::LeftButton, Qt::NoModifier);
        QMouseEvent mouseRelease(QEvent::MouseButtonRelease, { oldl - 10, oldb - 10 },
                                 Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        EXPECT_TRUE(arrow.eventFilter(nullptr, &mousePress));
        EXPECT_TRUE(arrow.eventFilter(nullptr, &mouseMove));
        EXPECT_TRUE(arrow.eventFilter(nullptr, &mouseRelease));
        auto newBBox = arrow.rect();
        EXPECT_EQ(bbox.left() - 10, newBBox.left());
        EXPECT_EQ(bbox.right(), newBBox.right());
        EXPECT_EQ(bbox.bottom() - 10, newBBox.bottom());
        EXPECT_EQ(bbox.top(), newBBox.top());
        bbox = newBBox;
    }
    {
        // move both
        QMouseEvent mousePress(QEvent::MouseButtonPress, { midx, midy }, Qt::LeftButton,
                               Qt::LeftButton, Qt::NoModifier);
        QMouseEvent mouseMove(QEvent::MouseMove, { midx - 10, midy - 10 }, Qt::NoButton,
                              Qt::LeftButton, Qt::NoModifier);
        QMouseEvent mouseRelease(QEvent::MouseButtonRelease, { midx - 10, midy - 10 },
                                 Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        EXPECT_TRUE(arrow.eventFilter(nullptr, &mousePress));
        EXPECT_TRUE(arrow.eventFilter(nullptr, &mouseMove));
        EXPECT_TRUE(arrow.eventFilter(nullptr, &mouseRelease));
        auto newBBox = arrow.rect();
        EXPECT_EQ(bbox.left() - 10, newBBox.left());
        EXPECT_EQ(bbox.right() - 10, newBBox.right());
        EXPECT_EQ(bbox.bottom() - 10, newBBox.bottom());
        EXPECT_EQ(bbox.top() - 10, newBBox.top());
        bbox = newBBox;

        EXPECT_TRUE(!arrow.eventFilter(nullptr, &mouseRelease)); // nothing to be done
    }
    // double clicking should open dialog. Send a return (closing the dialog) on a separate
    // thread
    QMouseEvent dblClick(QEvent::MouseButtonDblClick, { 0, 0 }, Qt::LeftButton, Qt::LeftButton,
                         Qt::NoModifier);
    CloseDialog cd;
    EXPECT_TRUE(arrow.eventFilter(nullptr, &dblClick));
}
