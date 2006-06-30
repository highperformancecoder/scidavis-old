#ifndef PLOT_H
#define PLOT_H

#include <qobject.h>
#include <qmap.h>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>

class Plot: public QwtPlot
{	
    Q_OBJECT

public:	
	Plot(QWidget *parent = 0, const char *name = 0);
	
	enum TicksType{None = 0, Out = 1, Both = 2, In = 3};
	
	QwtPlotGrid *grid(){return d_grid;};
	QValueList<int> curveKeys(){return d_curves.keys();};

	int insertCurve(QwtPlotCurve *c);
	void removeCurve(int index);

	int closestCurve(int xpos, int ypos, int &dist, int &point);
	QwtPlotCurve* curve(int index){return d_curves[index];};

	QwtPlotMarker* marker(int index){return d_markers[index];};
	QValueList<int> markerKeys(){return d_markers.keys();};
	int insertMarker(QwtPlotMarker *m);
	void removeMarker(int index);

	QValueList <int> getMajorTicksType(){return majorTicksType;};
	void setMajorTicksType(int axis, int type){majorTicksType[axis]=type;}

	QValueList <int> getMinorTicksType(){return minorTicksType;};
	void setMinorTicksType(int axis, int type){minorTicksType[axis]=type;}

	int minorTickLength() const;
	int majorTickLength() const;
	void setTickLength (int minLength, int majLength);

	int axesLinewidth() const;
	void setAxesLinewidth(int width);

	void printFrame(QPainter *painter, const QRect &rect) const;

	QColor frameColor();

	void mousePressEvent ( QMouseEvent * e );
	void mouseReleaseEvent ( QMouseEvent * e );
	void mouseMoveEvent ( QMouseEvent * e );

	void drawPixmap(QPainter *painter, const QRect &rect);
	/*virtual void print(QPainter *, const QRect &rect,
        const QwtPlotPrintFilter & = QwtPlotPrintFilter()) const;*/
	
protected:
	/*
	void printCanvas(QPainter *painter, const QRect &canvasRect,
   			 const QwtArray<QwtScaleMap> &map, const QwtPlotPrintFilter &pfilter) const;*/

	virtual void drawItems (QPainter *painter, const QRect &rect,
			const QwtArray< QwtScaleMap > &map, const QwtPlotPrintFilter &pfilter) const;

	void drawInwardTicks(QPainter *painter, const QRect &rect, 
							const QwtScaleMap&map, int axis) const;

	void drawInwardMinorTicks(QPainter *painter, const QRect &rect, 
							const QwtScaleMap &map, int axis) const;

	void drawInwardMajorTicks(QPainter *painter, const QRect &rect, 
							const QwtScaleMap &map, int axis) const;
signals:
	void selectPlot();
	void moveGraph(const QPoint&);
	void releasedGraph();
	void resizeGraph(const QPoint&);
	void resizedGraph();

protected:
	QwtPlotGrid *d_grid;
	QMap<int, QwtPlotCurve*> d_curves;
	QMap<int, QwtPlotMarker*> d_markers;

	QValueList <int> minorTicksType;
	QValueList <int> majorTicksType;

	int minTickLength, majTickLength;
	bool movedGraph, ShiftButton, graphToResize;
	QPoint presspos;
	int marker_key;
	int curve_key;
};

#endif
