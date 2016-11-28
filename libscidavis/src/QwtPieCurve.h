/***************************************************************************
    File                 : QwtPieCurve.h
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Ion Vasilief, Tilman Benkert
    Email (use @ for *)  : ion_vasilief*yahoo.fr, thzs*gmx.net
    Description          : Pie plot class

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
#include <qwt_plot.h>
#include "PlotCurve.h"

//! Pie plot class
class QwtPieCurve: public DataCurve
{
public:
	QwtPieCurve(Table *t, const char *name, int startRow, int endRow);

public slots:
	QColor color(int i) const;

	int ray(){return d_pie_ray;};
	void setRay(int size){d_pie_ray = size; updateBoundingRect();};

	Qt::BrushStyle pattern(){return QwtPlotCurve::brush().style();};
	void setBrushStyle(const Qt::BrushStyle& style);

	void setFirstColor(int index){d_first_color = index;};
	int firstColor(){return d_first_color;};

	virtual bool loadData();
	void updateBoundingRect();

private:
    void draw(QPainter *painter,/*const QwtScaleMap &xMap,
        const QwtScaleMap &yMap,*/ int from, int to) const;

    void drawPie(QPainter *painter,/* const QwtScaleMap &xMap,
        const QwtScaleMap &yMap,*/ int from, int to) const;

	int d_pie_ray, d_first_color;
	//! Keeps track of the left side position of the pie bounding rectangle in scale coordinates.
	double d_left_coord;
};
