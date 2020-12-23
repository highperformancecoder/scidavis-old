/***************************************************************************
    File                 : QwtBarCurve.cpp
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Ion Vasilief, Tilman Benkert
    Email (use @ for *)  : ion_vasilief*yahoo.fr, thzs*gmx.net
    Description          : Bar curve

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/
#include "QwtBarCurve.h"
#include <QPainter>

QwtBarCurve::QwtBarCurve(BarStyle style, Table *t, const QString& xColName, const QString& name, int startRow, int endRow):
    DataCurve(t, xColName, name, startRow, endRow)
{
bar_offset=0;
bar_gap=0;
bar_style=style;

setPen(QPen(Qt::black, 1, Qt::SolidLine));
setBrush(QBrush(Qt::red));

if (bar_style == Vertical)
	setType(Graph::VerticalBars);
else
	setType(Graph::HorizontalBars);
}

void QwtBarCurve::copy(const QwtBarCurve *b)
{
bar_gap = b->bar_gap;
bar_offset = b->bar_offset;
bar_style = b->bar_style;
}

void QwtBarCurve::draw(QPainter *painter,
    const QwtScaleMap &xMap, const QwtScaleMap &yMap, int from, int to) const
{
   if ( !painter || dataSize() <= 0 )
        return;

    if (to < 0)
        to = dataSize() - 1;

    painter->save();
    painter->setPen(QwtPlotCurve::pen());
    painter->setBrush(QwtPlotCurve::brush());

    int dx, dy;
    double ref1, ref2;
	double bar_width = 0;

    if (bar_style == Vertical){
       ref1= yMap.xTransform(1e-100); //smallest positive value for log scales
       ref2=ref1;
       if (yMap.xTransform(yMap.s1()) < ref1)
            ref1 = yMap.xTransform(yMap.s1());
       if (yMap.xTransform(yMap.s2()) > ref2)
            ref2 = yMap.xTransform(yMap.s2());
    } else {
       ref1= xMap.xTransform(1e-100);
       ref2=ref1;
       if (xMap.xTransform(xMap.s1()) > ref1)
            ref1 = xMap.xTransform(xMap.s1());
       if (xMap.xTransform(xMap.s2()) < ref2)
            ref2 = xMap.xTransform(xMap.s2());
    }

	if (bar_style == Vertical)
	{
		dx = abs(xMap.transform(x(from+1))-xMap.transform(x(from)));
		for (int i=from+2; i<to; i++)
		{
			int min = abs(xMap.transform(x(i+1))-xMap.transform(x(i)));
			if (min <= dx)
				dx=min;
		}
		bar_width = dx*(1-bar_gap*0.01);
	}
	else
	{
		dy = abs(yMap.transform(y(from+1))-yMap.transform(y(from)));
		for (int i=from+2; i<to; i++)
		{
			int min = abs(yMap.transform(y(i+1))-yMap.transform(y(i)));
			if (min <= dy)
				dy=min;
		}
		bar_width = dy*(1-bar_gap*0.01);
	}

	const int half_width = int((0.5-bar_offset*0.01)*bar_width);
	int bw1 = int(bar_width) + 1;
	for (int i=from; i<=to; i++)
	{
		const int px = xMap.transform(x(i));
        const int py = yMap.transform(y(i));

		if (bar_style == Vertical)
		{
			if (y(i) < 0)
				painter->drawRect(px-half_width, ref2, bw1, qRound(py-ref2));
			else
				painter->drawRect(px-half_width, py, bw1, qRound(ref1-py+1));
		}
		else
		{
			if (x(i) < 0)
				painter->drawRect(px, py-half_width, qRound(ref2-px), bw1);
			else
				painter->drawRect(ref1, py-half_width, qRound(px-ref1), bw1);
		}
	}
	painter->restore();
}

QwtDoubleRect QwtBarCurve::boundingRect() const
{
QwtDoubleRect rect = QwtPlotCurve::boundingRect();
double n= (double)dataSize();

if (bar_style == Vertical)
	{
	double dx=(rect.right()-rect.left())/n;
	rect.setLeft(rect.left()-dx);
	rect.setRight(rect.right()+dx);
	}
else
	{
	double dy=(rect.bottom()-rect.top())/n;
	rect.setTop(rect.top()-dy);
	rect.setBottom(rect.bottom()+dy);
	}

return rect;
}

void QwtBarCurve::setGap (int gap)
{
if (bar_gap == gap)
	return;

bar_gap =gap;
}

void QwtBarCurve::setOffset(int offset)
{
if (bar_offset == offset)
	return;

bar_offset = offset;
}

double QwtBarCurve::dataOffset()
{
	if (bar_style == Vertical)
	{
		const QwtScaleMap &xMap = plot()->canvasMap(xAxis());
		int dx = abs(xMap.transform(x(1))-xMap.transform(x(0)));
		double bar_width = dx*(1-bar_gap*0.01);
		if (plot()->isVisible())
		{
			for (int i = 2; i<dataSize(); i++)
			{
				int min = abs(xMap.transform(x(i))-xMap.transform(x(i-1)));
				if (min <= dx)
					dx=min;
			}
			int x1 = xMap.transform(minXValue()) + int(bar_offset*0.01*bar_width);
			return xMap.invTransform(x1) - minXValue();
		}
		else
			return 0.5*bar_offset*0.01*bar_width;
	}
	else
	{
		const QwtScaleMap &yMap = plot()->canvasMap(yAxis());
		int dy = abs(yMap.transform(y(1))-yMap.transform(y(0)));
		double bar_width = dy*(1-bar_gap*0.01);
		if (plot()->isVisible())
		{
			for (int i=2; i<dataSize(); i++)
			{
				int min = abs(yMap.transform(y(i))-yMap.transform(y(i-1)));
				if (min <= dy)
					dy=min;
			}
			int y1 = yMap.transform(minYValue()) + int(bar_offset*0.01*bar_width);
			return yMap.invTransform(y1) - minYValue();
		}
		else
			return 0.5*bar_offset*0.01*bar_width;
	}
}
