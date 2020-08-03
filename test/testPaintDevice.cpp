#include "testPaintDevice.h"
using namespace std;

namespace
{
  ostream& operator<<(ostream& o, const QRectF& x)
  {return o<<"["<<x.left()<<","<<x.top()<<","<<x.right()<<","<<x.bottom()<<"]";}

  ostream& operator<<(ostream& o, const QRect& x)
  {return o<<"["<<x.left()<<","<<x.top()<<","<<x.right()<<","<<x.bottom()<<"]";}

  ostream& operator<<(ostream& o, const QPointF& x)
  {return o<<"["<<x.x()<<","<<x.y()<<"]";}

  ostream& operator<<(ostream& o, const QPoint& x)
  {return o<<"["<<x.x()<<","<<x.y()<<"]";}

  ostream& operator<<(ostream& o, const QTextItem& x)
  {return o<<"\""<<x.text().toStdString()<<"\"";}

  ostream& operator<<(ostream& o, const QImage& x)
  {
    o<<"\"";
    auto data=x.bits();
#if QT_VERSION>=0x050A00
    auto size=x.sizeInBytes();
#else
    auto size=x.byteCount();
#endif
    for (int i=0; i<size; i++)
      o<<std::hex<<int(data[i]);
    return o<<"\"";
  }

//  ostream& operator<<(ostream& o, const QLine& x)
//  {
//    return o<<"["<<x.x1()<<","<<x.y1()<<","<<x.x2()<<","<<x.y2()<<"]";
//  }

  ostream& operator<<(ostream& o, const QPainterPath::Element& x)
  {return o<<"["<<int(x.type)<<","<<x.x<<","<<x.y<<"]";}

  ostream& operator<<(ostream& o, const QPainterPath& x)
  {
    o << "[";
    for (int i=0; i<x.elementCount(); ++i)
      {
        if (i>0) o<<",";
        o<<x.elementAt(i);
      }
    return o<<"]";
  }

  template <class T>
  ostream& arrayOut(ostream& o, T* x, int n)
  {
    o<<"[";
    cout << "n="<<n<<endl;
    for (int i=0; i<n; i++)
      {
        if (i>0) o<<",\n";
        o<<x[i];
      }
    return o<<"]";
  }
}


void TestPaintDevice::PaintEngine::drawEllipse(const QRectF& x)
{out<<"drawEllipse("<<x<<")\n";}

void TestPaintDevice::PaintEngine::drawEllipse(const QRect& x)
{out<<"drawEllipse("<<x<<")\n";}

void TestPaintDevice::PaintEngine::drawImage
(const QRectF& r, const QImage& i, const QRectF&, Qt::ImageConversionFlags f)
{out<<"drawImage("<<r<<","<<i<<","<<f<<")\n";} 

//void TestPaintDevice::PaintEngine::drawLines(const QLine* lines, int n)
//{out<<"drawLines("; arrayOut(out,lines,n)<<")\n";}

void TestPaintDevice::PaintEngine::drawPath(const QPainterPath& p)
{out<<"drawPath("<<p<<")\n";}

void TestPaintDevice::PaintEngine::drawPixmap(const QRectF& x, const QPixmap& y, const QRectF& z)
{out<<"drawPixmap("<<x<<","<<y.toImage()<<","<<z<<")\n";}

void TestPaintDevice::PaintEngine::drawPoints(const QPointF* p, int n)
{out<<"drawPoints("; arrayOut(out,p,n)<<")\n";}

//void TestPaintDevice::PaintEngine::drawPoints(const QPoint*p, int n)
//{out<<"drawPoints("; arrayOut(out,p,n)<<")\n";}

void TestPaintDevice::PaintEngine::drawPolygon(const QPointF*p, int n, PolygonDrawMode m)
{out<<"drawPolygon("; arrayOut(out,p,n)<<","<<int(m)<<")\n";}

void TestPaintDevice::PaintEngine::drawPolygon(const QPoint*p, int n, PolygonDrawMode m)
{out<<"drawPolygon("; arrayOut(out,p,n)<<","<<int(m)<<")\n";}

void TestPaintDevice::PaintEngine::drawRects(const QRectF* r, int n)
{out<<"drawRects("; arrayOut(out,r,n)<<")\n";}

//void TestPaintDevice::PaintEngine::drawRects(const QRect*r, int n)
//{out<<"drawPolygon("; arrayOut(out,r,n)<<")\n";}

void TestPaintDevice::PaintEngine::drawTextItem(const QPointF& p, const QTextItem&t)
{out<<"drawTextItem("<<p<<",\""<<t<<"\")\n";}

void TestPaintDevice::PaintEngine::drawTiledPixmap(const QRectF&r, const QPixmap&pm, const QPointF&p)
{out<<"drawTiledPixmap("<<r<<","<<pm.toImage()<<","<<p<<")\n";}
