#ifndef LEGENDMARKER_H
#define LEGENDMARKER_H

#include <qfont.h>
#include <qpen.h>

#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_array.h>
#include <qwt_text.h>

#include "graph.h"
#include "plot.h"
	
class LegendMarker: public QwtPlotMarker
{
public:
    LegendMarker(QwtPlot *);
	~LegendMarker();

	enum FrameStyle{None = 0, Line = 1, Shadow=2};
	
    virtual void draw(QPainter *p, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRect &r) const;

	QString getText();
	void setText(const QString& s);

	QRect rect();

	void setOrigin(const QPoint & p);

	//! Sets the position of the top left corner in axis coordinates
	void setOriginCoord(double x, double y);

	//! Keep the markers on screen each time the scales are modified by adding/removing curves
	void updateOrigin();

	QColor getTextColor();
	void setTextColor(const QColor& c);
	
	QColor backgroundColor(){return bkgColor;};
	void setBackgroundColor(const QColor& c);

	int getBkgType();
	void setBackground(int bkg);

	QFont getFont();
	void setFont(const QFont& font);

	int getAngle(){return angle;};
	void setAngle(int ang){angle=ang;};
	
	void drawFrame(QPainter *p, int type, const QRect& rect) const;

	void drawSymbols(QPainter *p, const QRect& rect,
					QwtArray<long> height, int symbolLineLength) const;

	void drawLegends(QPainter *p, const QRect& rect, 
					QwtArray<long> height, int symbolLineLength) const;

	void drawVector(QPainter *p, int x, int y, int l, int curveIndex) const;
														
	QwtArray<long> itemsHeight(int y, int symbolLineLength, int &width, int &height) const;
	int symbolsMaxLineLength() const;

protected:
	//! Parent plot
	Plot *d_plot;

	//! Frame type
	int d_frame;
	
	//! Rotation angle: not implemented yet
	int angle;
	
	//! Pointer to the QwtText object
	QwtText* d_text;

	//! Background color
	QColor bkgColor;

	//! TopLeft position in pixels
	QPoint d_pos;

	//!Distance between symbols and legend text
	int hspace;

	//!Distance between frame and content
	int left_margin, top_margin;
};

#endif
