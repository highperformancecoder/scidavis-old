#ifndef ERRORBARS_H
#define ERRORBARS_H

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

class QwtErrorPlotCurve: public QwtPlotCurve
{
public:
	enum Orientation{Horizontal = 0, Vertical = 1};

	QwtErrorPlotCurve(int orientation, const char *name);
	QwtErrorPlotCurve(const char *name=0);
			
	void copy(const QwtErrorPlotCurve *e);

	QwtDoubleRect boundingRect() const;

	virtual void draw(QPainter *painter,const QwtScaleMap &xMap, 
		const QwtScaleMap &yMap, int from, int to) const;

	virtual void drawErrorBars(QPainter *painter, const QwtScaleMap &xMap, 
		const QwtScaleMap &yMap, int from, int to) const;

	double errorValue(int i);
	QwtArray<double> errors(){return err;};
	void setErrors(const QwtArray<double>&data){err=data;};

	void setSymbolSize(const QSize& sz){size=sz;};

	int capLength(){return cap;};
	void setCapLength(int t){cap=t;};

	int width(){return pen().width ();};
	void setWidth(int w);

	QColor color(){return pen().color();};
	void setColor(const QColor& c);

	int direction(){return type;};
	void setDirection(int o){type = o;};

	bool xErrors();
	void setXErrors(bool yes);

	bool throughSymbol(){return through;};
	void drawThroughSymbol(bool yes){through=yes;};

	bool plusSide(){return plus;};
	void drawPlusSide(bool yes){plus=yes;};

	bool minusSide(){return minus;};
	void drawMinusSide(bool yes){minus=yes;};

	double xDataOffset() const {return d_xOffset;}
	void setXDataOffset(double offset){d_xOffset = offset;};

	double yDataOffset() const {return d_yOffset;}
	void setYDataOffset(double offset){d_yOffset = offset;};

private:
	//! Stores the error bar values
    QwtArray<double> err;

	//! Size of the symbol in the master curve
	QSize size;

	//! Orientation of the bars: Horizontal or Vertical
	int type;
	
	//! Length of the bar cap decoration
	int cap;

	bool plus, minus, through;
	double d_xOffset, d_yOffset;
};
#endif




