#ifndef ERRORBARS_H
#define ERRORBARS_H

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

class QwtErrorPlotCurve: public QwtPlotCurve
{
public:
	enum Orientation{Horizontal = 0, Vertical = 1};

	QwtErrorPlotCurve(int orientation, QwtPlot *parent, const char *name);
	QwtErrorPlotCurve(QwtPlot *parent, const char *name=0);
			
	void copy(const QwtErrorPlotCurve *e);

	QwtDoubleRect boundingRect() const;

	virtual void draw(QPainter *painter,const QwtScaleMap &xMap, 
		const QwtScaleMap &yMap, int from, int to) const;

	virtual void drawErrorBars(QPainter *painter, const QwtScaleMap &xMap, 
		const QwtScaleMap &yMap, int from, int to) const;

	QwtArray<double> errors();
	void setErrors(const QwtArray<double>&data);
	void setSymbolSize(const QSize& sz);

	int capLength();
	void setCapLength(int t);

	int width();
	void setWidth(int w);

	QColor color();
	void setColor(const QColor& c);

	int direction(){return type;};
	void setDirection(int o){type = o;};

	bool xErrors();
	void setXErrors(bool yes);

	bool throughSymbol();
	void drawThroughSymbol(bool yes);

	bool plusSide();
	void drawPlusSide(bool yes);

	bool minusSide();
	void drawMinusSide(bool yes);

	double xDataOffset() const;
	void setXDataOffset(double offset){d_xOffset = offset;};

	double yDataOffset() const;
	void setYDataOffset(double offset){d_yOffset = offset;};

private:
    QwtArray<double> err;
	QPen pen;
	QSize size;
	int type, cap;
	bool plus, minus, through;
	double d_xOffset, d_yOffset;
};

#endif


