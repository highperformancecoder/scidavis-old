#ifndef LINEMARKER_H
#define LINEMARKER_H

#include <qwt_plot_marker.h>
	
class LineMarker: public QwtPlotMarker
{
public:
    LineMarker(QwtPlot *);
    virtual void draw(QPainter *p, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRect &r) const;

	void setStartPoint(const QPoint& p);
	void setEndPoint(const QPoint& p);
	QPoint startPoint();
	QPoint endPoint();
	double dist(int x, int y);
	double teta(int xs, int ys, int xe, int ye) const;
	double length();

	QwtDoublePoint coordStartPoint();
	void setCoordStartPoint(const QwtDoublePoint& p);

	QwtDoublePoint coordEndPoint();
	void setCoordEndPoint(const QwtDoublePoint& p);

	void setColor(const QColor& c);
	QColor color();

	void setWidth(int w);
	int width();

	void setStyle(Qt::PenStyle s);
	Qt::PenStyle style();

	void setStartArrow(bool on);
	bool getStartArrow();
	void setEndArrow(bool on);
	bool getEndArrow();
	
	int headLength(){return d_headLength;};
	void setHeadLength(int l);
	
	int headAngle(){return d_headAngle;};
	void setHeadAngle(int a);
	
	bool filledArrowHead(){return filledArrow;};
	void fillArrowHead(bool fill);
	
	void updateBoundingRect();
	
private:
	QPoint d_start, d_end; 
	QPen pen;
	bool startArrow,endArrow, filledArrow;
	int d_headAngle, d_headLength;
	QwtDoubleRect d_rect;
	QwtPlot *d_plot;
};
#endif
