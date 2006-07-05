#ifndef IMAGEMARKER_H
#define IMAGEMARKER_H

#include <qpaintdevicemetrics.h>
#include <qpixmap.h>
#include <qwt_plot_marker.h>
	
class ImageMarker: public QwtPlotMarker
{
public:
	ImageMarker(const QPixmap& p);
    virtual void draw(QPainter *p, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRect &r) const;
	
	QRect rect();

	QSize size();
	void setSize(const QSize& size);
	
	void setFileName(const QString& fn){fileName=fn;};
	QString getFileName(){return fileName;};
	
	QPixmap image(){return pic;};

	void setOrigin(const QPoint& p);
	QPoint getOrigin();

//! Keep the markers on screen each time the scales are modified by adding/removing curves
	void updateOrigin();

	QwtDoubleRect boundingRect() const;

private:
	QPoint origin;
	QPixmap pic;
	QSize picSize;
	QString fileName;
	QwtDoubleRect d_rect;
};

#endif
