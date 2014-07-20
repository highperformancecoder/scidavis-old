#ifndef ARRAYDATA_H
#define ARRAYDATA_H
/*
  Replacement for QwtArrayData as it is used in SciDaVis
*/

#include <limits>

class ArrayData
{
  QVector<double> m_x, m_y;
  QRectF m_boundingRect;
public:
  ArrayData() {}
  ArrayData(const QVector<double>& x, const QVector<double>& y):
    m_x(x), m_y(y) {updateBB();}

  void setData(const QVector<double>& x, const QVector<double>& y)
  {m_x=x; m_y=y; updateBB();}

  void updateBB() {
    double minx=std::numeric_limits<double>::max(), 
      maxx=-minx, miny=minx, maxy=maxx;
    for (int i=0; i<std::min(m_x.size(), m_y.size()); ++i)
      {
        if (minx>x(i)) minx=x(i);
        if (maxx<x(i)) maxx=x(i);
        if (miny>y(i)) miny=y(i);
        if (maxy<y(i)) maxy=y(i);
        m_boundingRect.setLeft(minx);
        m_boundingRect.setRight(maxx);
        m_boundingRect.setBottom(miny);
        m_boundingRect.setTop(maxy);
      }
  }
    
  double x(size_t i) const {return m_x[i];}  
  double y(size_t i) const {return m_y[i];}

  QRectF boundingRect() const {return m_boundingRect;}
};
#endif
