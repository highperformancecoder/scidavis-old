#include "ApplicationWindow.h"
#include "ArrowMarker.h"
#include "MultiLayer.h"
#include <iostream>
#include <thread>
using namespace std;

#include <UnitTest++/UnitTest++.h>
#include "utils.h"

static double sqr(double x) {return x*x;}

SUITE(ArrowMarker)
{
  TEST_FIXTURE(ApplicationWindow, arrowMarker)
    {
      ArrowMarker arrow;

      auto& graph=newGraph();
      auto layer=graph.activeGraph();
      layer->addArrow(&arrow);

      arrow.attach(layer->plotWidget());
      
      arrow.setStartPoint(50,200);
      arrow.setEndPoint(400,400);
      arrow.setWidth(2);
      arrow.drawStartArrow(false);
      arrow.drawEndArrow(true);
      layer->replot();

      auto bbox=arrow.rect();

      CHECK_CLOSE(sqrt(sqr(bbox.left()-bbox.right())+sqr(bbox.top()-bbox.bottom())), arrow.length(), 0.1);
      CHECK_CLOSE(sqrt(sqr(bbox.left())+sqr(bbox.bottom())), arrow.dist(0,0), 0.1);

      // test interaction with the mouse
      arrow.setEditable(true);
      layer->replot();
      graph.exportImage("arrowEditable.png");
      auto oldr=bbox.right(), oldl=bbox.left(), oldt=bbox.top(), oldb=bbox.bottom();
      auto midx=(oldl+oldr)/2, midy=(oldb+oldt)/2;
      {
        // move ending point
        QMouseEvent mousePress(QEvent::MouseButtonPress,{oldr,oldt},Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
        QMouseEvent mouseMove(QEvent::MouseMove,{oldr+10,oldt+10},Qt::NoButton,Qt::LeftButton,Qt::NoModifier);
        QMouseEvent mouseRelease(QEvent::MouseButtonRelease,{oldr+10,oldt+10},Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
        CHECK(arrow.eventFilter(nullptr,&mousePress));
        CHECK(arrow.eventFilter(nullptr,&mouseMove));
        CHECK(arrow.eventFilter(nullptr,&mouseRelease));
        auto newBBox=arrow.rect();
        CHECK_EQUAL(bbox.left(),newBBox.left());
        CHECK_EQUAL(bbox.right()+10,newBBox.right());
        CHECK_EQUAL(bbox.bottom(),newBBox.bottom());
        CHECK_EQUAL(bbox.top()+10,newBBox.top());
        bbox=newBBox;
      }
      {
        // move starting point
        QMouseEvent mousePress(QEvent::MouseButtonPress,{oldl,oldb},Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
        QMouseEvent mouseMove(QEvent::MouseMove,{oldl-10,oldb-10},Qt::NoButton,Qt::LeftButton,Qt::NoModifier);
        QMouseEvent mouseRelease(QEvent::MouseButtonRelease,{oldl-10,oldb-10},Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
        CHECK(arrow.eventFilter(nullptr,&mousePress));
        CHECK(arrow.eventFilter(nullptr,&mouseMove));
        CHECK(arrow.eventFilter(nullptr,&mouseRelease));
        auto newBBox=arrow.rect();
        CHECK_EQUAL(bbox.left()-10,newBBox.left());
        CHECK_EQUAL(bbox.right(),newBBox.right());
        CHECK_EQUAL(bbox.bottom()-10,newBBox.bottom());
        CHECK_EQUAL(bbox.top(),newBBox.top());
        bbox=newBBox;
      }
      {
        // move both
        QMouseEvent mousePress(QEvent::MouseButtonPress,{midx,midy},Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
        QMouseEvent mouseMove(QEvent::MouseMove,{midx-10,midy-10},Qt::NoButton,Qt::LeftButton,Qt::NoModifier);
        QMouseEvent mouseRelease(QEvent::MouseButtonRelease,{midx-10,midy-10},Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
        CHECK(arrow.eventFilter(nullptr,&mousePress));
        CHECK(arrow.eventFilter(nullptr,&mouseMove));
        CHECK(arrow.eventFilter(nullptr,&mouseRelease));
        auto newBBox=arrow.rect();
        CHECK_EQUAL(bbox.left()-10,newBBox.left());
        CHECK_EQUAL(bbox.right()-10,newBBox.right());
        CHECK_EQUAL(bbox.bottom()-10,newBBox.bottom());
        CHECK_EQUAL(bbox.top()-10,newBBox.top());
        bbox=newBBox;

        CHECK(!arrow.eventFilter(nullptr,&mouseRelease)); // nothing to be done

      }
      // double clicking should open dialog. Send a return (closing the dialog) on a separate thread
      QMouseEvent dblClick(QEvent::MouseButtonDblClick,{0,0},Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
      CloseDialog cd;
      CHECK(arrow.eventFilter(nullptr,&dblClick));
    }

  
}
