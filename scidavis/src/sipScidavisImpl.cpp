/*
  provides alternate implementions for scidavis.sip code to handle
  conditional compilatiion that might be required, eg for the qwt5 ->
  qwt6 transition
 */

#include "sipScidavidImpl.h"

namespace sipscidavis
{
  void SetCurveColour(QwtPlotCurve& curve, const QColor& colour)
  {
    QPen pen = curve.pen();
    pen.setColor(colour);
    curve.setPen(pen);
#if QWT_VERSION<0x60000
    const QwtSymbol* symbol = curve.symbol();
    pen = symbol.pen();
    pen.setColor(colour);
    symbol.setPen(pen);
    QBrush brush = symbol.brush();
    brush.setColor(colour);
    symbol.setBrush(brush);
    curve.setSymbol(symbol);
#else
    const QwtSymbol* symbol = curve.symbol();
    pen = symbol->pen();
    pen.setColor(colour);
    symbol->setPen(pen);
    QBrush brush = symbol->brush();
    brush.setColor(colour);
    symbol->setBrush(brush);
    curve.setSymbol(symbol);
#endif
  }

  double qwtPlotCurve_x(const QwtPlotCurve& c,int i)
  {
#if QWT_VERSION<0x60000
    return c.x(i);
#else
    return c.sample(i).x();
#endif
  }

  double qwtPlotCurve_y(const QwtPlotCurve&,int);
  {
#if QWT_VERSION<0x60000
    return c.y(i);
#else
    return c.sample(i).y();
#endif
  }

  void qwtPlotCurve_setSymbol(QwtPlotCurve& c,const QwtSymbol &s)
  {
#if QWT_VERSION<0x60000
    c.setSymbol(s);
#else
    c.setSymbol(new QwtSymbol(s));
#endif
  }

  const QwtSymbol& qwtPlotCurve_symbol(const QwtPlotCurve& curve)
  {
#if QWT_VERSION<0x60000
    return c.symbol();
#else
    return *c.symbol();
#endif
  }
 
}
