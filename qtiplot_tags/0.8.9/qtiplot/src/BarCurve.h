#ifndef BARCURVE_H
#define BARCURVE_H

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

class QwtBarCurve: public QwtPlotCurve
{
public:
	enum BarStyle{Vertical = 0, Horizontal=1};
	QwtBarCurve(const char *name=0);
	QwtBarCurve(BarStyle style, const char *name=0);

	void copy(const QwtBarCurve *b);

	virtual void draw(QPainter *painter,const QwtScaleMap &xMap, 
		const QwtScaleMap &yMap, int from, int to) const;

	virtual QwtDoubleRect boundingRect() const;

	BarStyle orientation(){return bar_style;};

	void setGap (int gap);  
	int gap() const {return bar_gap;};

	void setOffset(int offset);
	int offset() const {return bar_offset;};

	double dataOffset();
	
private:
	int bar_gap, bar_offset;
	BarStyle bar_style;
};

#endif 

