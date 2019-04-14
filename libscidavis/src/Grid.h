/***************************************************************************
    File                 : Grid.h
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Ion Vasilief
    Email (use @ for *)  : ion_vasilief*yahoo.fr
    Description          : 2D Grid class

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
#ifndef GRID_H
#define GRID_H

#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>

//! 2D Grid class
class Grid : public QwtPlotGrid
{
public:
  Grid();

  void draw(QPainter *p, const QwtScaleMap &xMap, const QwtScaleMap &yMap,
            const QRect &rect) const;
  void drawLines(QPainter *painter, const QRect &rect,
                 QtEnums::Orientation orientation, const QwtScaleMap &map,
                 const QwtValueList &values) const;

  bool xZeroLine() const {return (mrkX >= 0)?true:false;};
  void setXZeroLine(bool enable = true);
  bool yZeroLine() const {return (mrkY >= 0)?true:false;};
  void setYZeroLine(bool enable = true);

  void setXMajorPen(const QPen &p){	setMajPen(p);};
  const QPen& xMajorPen() const {return majPen();};

  void setXMinorPen(const QPen &p){	setMinPen(p);};
  const QPen& xMinorPen() const {return minPen();};

  void setYMajorPen(const QPen &p){	if (d_maj_pen_y != p) d_maj_pen_y = p;};
  const QPen& yMajorPen() const {return d_maj_pen_y;};

  void setYMinorPen(const QPen &p){	if (d_min_pen_y != p) d_min_pen_y = p;};
  const QPen& yMinorPen() const {return d_min_pen_y;};

  void setMajor(bool x) {
    enableX(x);
    enableY(x);
  }
  void setMinor(bool x) {
    enableXMin(x);
    enableYMin(x);
  }
  bool xMajor() const {return xEnabled();}
  void setXMajor(bool x) {enableX(x);}
  bool yMajor() const {return yEnabled();}
  void setYMajor(bool x) {enableY(x);}
  bool xMinor() const {return xMinEnabled();}
  void setXMinor(bool x) {enableXMin(x);}
  bool yMinor() const {return yMinEnabled();}
  void setYMinor(bool x) {enableYMin(x);}
  
  void load(const QStringList& );
  void copy(Grid *);
  QString saveToString();

private:
  QPen d_maj_pen_y;
  QPen d_min_pen_y;

  long mrkX, mrkY;//x=0 et y=0 line markers keys
};

#endif
