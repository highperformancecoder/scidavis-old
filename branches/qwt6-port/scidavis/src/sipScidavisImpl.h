#ifndef SIPSCIDAVISIMPL_H
#define SIPSCIDAVISIMPL_H
#include "src/ApplicationWindow.h"
namespace sipscidavis
{
  void SetCurveColour(QwtPlotCurve& curve, const QColor& colour);
  double qwtPlotCurve_x(const QwtPlotCurve&,int);
  double qwtPlotCurve_y(const QwtPlotCurve&,int);
  void qwtPlotCurve_setSymbol(QwtPlotCurve& curve,const QwtSymbol &s);
  const QwtSymbol& qwtPlotCurve_symbol(const QwtPlotCurve& curve);
}
#endif
