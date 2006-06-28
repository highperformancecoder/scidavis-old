#include "ImageMarker.h"
#include "LineMarker.h"

#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qwt_plot.h>
#include <qwt_plot_canvas.h>

ImageMarker::ImageMarker(const QPixmap& p, QwtPlot *plot):
    QwtPlotMarker()
{
pic=p;
}

void ImageMarker::draw (QPainter *p, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRect &r) const
{
	if (p->device()->isExtDev())
		{
		QwtPlot *plot = this->plot();					
		double r0_x=xMap.invTransform(origin.x());
		double r0_y=yMap.invTransform(origin.y());	
		double r1_x=xMap.invTransform(origin.x()+picSize.width());
		double r1_y=yMap.invTransform(origin.y()+picSize.height());
			
		QwtScaleMap map=LineMarker::mapCanvasToDevice(p, plot, QwtPlot::xBottom);
		int x0=map.transform(r0_x);
		int x1=map.transform(r1_x);
	
	    map=LineMarker::mapCanvasToDevice(p, plot, QwtPlot::yLeft);		
		int y0=map.transform(r0_y);
		int y1=map.transform(r1_y);

		p->drawPixmap(QRect(QPoint(x0,y0),QPoint(x1,y1)),pic);			
		}		
	else	
		{
		QRect ir=QRect(origin,picSize);
		int clw = plot()->canvas()->lineWidth();
		ir.moveBy(r.x() - clw, r.y() - clw);
		p->drawPixmap(ir,pic);
		}
}

