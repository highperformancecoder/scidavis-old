#include "ApplicationWindow.h"
#include "ArrowMarker.h"
#include "MultiLayer.h"
#include <iostream>
using namespace std;

#include <UnitTest++/UnitTest++.h>
#include "qstringStream.h"

static double sqr(double x) {return x*x;}

SUITE(ArrowMarker)
{
  TEST_FIXTURE(ApplicationWindow, arrowMarker)
    {
      ArrowMarker arrow;

      auto graph=newGraph();
      auto layer=graph->activeGraph();
      layer->addArrow(&arrow);

      arrow.attach(layer->d_plot);
      
      arrow.setStartPoint(50,200);
      arrow.setEndPoint(400,400);
      arrow.setWidth(2);
      arrow.drawStartArrow(false);
      arrow.drawEndArrow(true);
      layer->replot();

      auto bbox=arrow.rect();

      CHECK_CLOSE(sqrt(sqr(bbox.left()-bbox.right())+sqr(bbox.top()-bbox.bottom())), arrow.length(), 0.1);
      CHECK_CLOSE(sqrt(sqr(bbox.left())+sqr(bbox.bottom())), arrow.dist(0,0), 0.1);
    }
}
