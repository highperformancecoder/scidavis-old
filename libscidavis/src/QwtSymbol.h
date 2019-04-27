#ifndef SCI_QWTSYMBOL_H
#define SCI_QWTSYMBOL_H
#include "Qt.h"
#include "ColorButton.h"
#include <qwt_symbol.h>
#include <qwt_plot_curve.h>

/// python interface to QwtSymbol
struct SciQwtSymbol: public QwtSymbol
{
  enum Style 
    { 
     NoSymbol=::QwtSymbol::NoSymbol, 
     Ellipse=::QwtSymbol::Ellipse, 
     Rect=::QwtSymbol::Rect, 
     Diamond=::QwtSymbol::Diamond, 
     Triangle=::QwtSymbol::Triangle, 
     DTriangle=::QwtSymbol::DTriangle,
     UTriangle=::QwtSymbol::UTriangle, 
     LTriangle=::QwtSymbol::LTriangle, 
     RTriangle=::QwtSymbol::RTriangle, 
     Cross=::QwtSymbol::Cross, 
     XCross=::QwtSymbol::XCross, 
     HLine=::QwtSymbol::HLine, 
     VLine=::QwtSymbol::VLine, 
     Star1=::QwtSymbol::Star1, 
     Star2=::QwtSymbol::Star2, 
     Hexagon=::QwtSymbol::Hexagon 
    };

  SciQwtSymbol() {}
  SciQwtSymbol(const QwtSymbol& x): QwtSymbol(x) {}
  SciQwtSymbol(Style st, const QtNamespace::QBrush &bd, const QtNamespace::QPen &pn, const QtCore::QSize &s):
    QwtSymbol(QwtSymbol::Style(st),bd,pn,s) {}
    
  void setSize(const QtCore::QSize &s) {QwtSymbol::setSize(s);}
  void setSize(int a, int b) {QwtSymbol::setSize(a,b);}
  void setSize(int a) {setSize(a,-1);}
  void setBrush(const QtNamespace::QBrush& b) {QwtSymbol::setBrush(b);}
  void setPen(const QtNamespace::QPen &p) {QwtSymbol::setPen(p);}
  void setStyle (Style s) {QwtSymbol::setStyle(QwtSymbol::Style(s));}

  const QtNamespace::QBrush brush() const {return QwtSymbol::brush();}
  const QtNamespace::QPen pen() const {return QwtSymbol::pen(); }
  const QtCore::QSize size() const {return QwtSymbol::size(); }
  //! Return Style
  Style style() const {return Style(QwtSymbol::style());} 

  void setOutlineColor(const QColor& c) {
    auto p=pen();
    p.setColor(c);
    setPen(p);
  }
  void setOutlineColor(int c) {setOutlineColor(ColorButton::color(c));}
  void setFillColor(const QColor& c) {
    auto b=brush();
    b.setColor(c);
    setBrush(b);
  }
  void setFillColor(int c) {setFillColor(ColorButton::color(c));}
 
  //    void draw(QPainter *p, const QPoint &pt) const; 
  //    void draw(QPainter *p, int x, int y) const;
  //    virtual void draw(QPainter *p, const QRect &r) const;
};

struct SciQwtPlotCurve
{
  QwtPlotCurve* ptr=nullptr;
  //  SciQwtPlotCurve() {}
  SciQwtPlotCurve(QwtPlotCurve& x): ptr(&x) {}

  int dataSize() const {return ptr? ptr->dataSize(): 0;}
  double x(int i) const {return ptr? ptr->x(i): 0;}
  double y(int i) const {return ptr? ptr->y(i): 0;}
  double minXValue() const {return ptr? ptr->minXValue(): 0;}
  double maxXValue() const {return ptr? ptr->maxXValue(): 0;}
  double minYValue() const {return ptr? ptr->minYValue(): 0;;}
  double maxYValue() const {return ptr? ptr->maxYValue(): 0;;}

  int xAxis() const {return ptr? ptr->xAxis(): 0;}
  void setXAxis(int i) {if (ptr) ptr->setXAxis(i);}
  int yAxis () const {return ptr? ptr->yAxis():0;}
  void setYAxis(int i) {if (ptr) ptr->setYAxis(i);}

  void setPen(const QtNamespace::QPen& p) {if (ptr) ptr->setPen(p);}
  const QtNamespace::QPen pen() const {if (ptr) return ptr->pen(); else return {};}

  void setBrush(const QtNamespace::QBrush& b)  {if (ptr) ptr->setBrush(b);}
  const QtNamespace::QBrush brush() const {if (ptr) return ptr->brush(); else return {};}

  void setSymbol(const SciQwtSymbol &s) {if (ptr) ptr->setSymbol(s);}
  const SciQwtSymbol symbol() const {if (ptr) return ptr->symbol(); else return {};}

  // convenience methods for scripting
  void setColor(const QColor& color) {
    setOutlineColor(color);
    setFillColor(color);
  }
  void setColor(int color) {setColor(ColorButton::color(color));}
  void setOutlineColor(const QColor& color) {
    auto p = pen();
    p.setColor(color);
    setPen(p);
  }
  void setOutlineColor(int color) {setOutlineColor(ColorButton::color(color));}
  void setFillColor(const QColor& color) {
	QBrush b = brush();
	b.setColor(color);
	setBrush(b);
  }
  void setFillColor(int color) {setFillColor(ColorButton::color(color));}
  void setFillStyle(QtEnums::BrushStyle style) {
	QBrush b = brush();
	b.setStyle(Qt::BrushStyle(style));
        setBrush(b);
  }

};


#endif
