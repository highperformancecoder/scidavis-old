/***************************************************************************
    File                 : PolynomialFit.h
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Ion Vasilief, Tilman Benkert
    Email (use @ for *)  : ion_vasilief*yahoo.fr, thzs*gmx.net
    Description          : Polynomial Fit and Linear Fit classes

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
#ifndef POLYNOMIALFIT_H
#define POLYNOMIALFIT_H

#include "Fit.h"

class PolynomialFit : public Fit
{
  Q_OBJECT

public:
  PolynomialFit() {}
  PolynomialFit(ApplicationWindow *parent, Graph *g, int order = 2, bool legend = false);
  PolynomialFit(ApplicationWindow *parent, Graph *g, const QString& curveTitle, int order = 2, bool legend = false);
  PolynomialFit(ApplicationWindow *parent, Graph *g, const QString& curveTitle, double start, double end, int order = 2, bool legend = false);
  PolynomialFit(Graph& g, const QString& curve, double start, double end, int order):
    PolynomialFit(0,&g,curve,start,end,order) {}
  PolynomialFit(Graph& g, const QString& curve, double start, double end):
    PolynomialFit(g,curve,start,end,2) {}
  PolynomialFit(Graph& g, const QString& curve, int order):
    PolynomialFit(0,&g,curve,order) {}
  PolynomialFit(Graph& g, const QString& curve):
    PolynomialFit(g,curve,2) {}
  
  QString legendInfo() override;
  void fit() override;

  static QString generateFormula(int order);
  static QStringList generateParameterList(int order);

private:
  void init();
  void calculateFitCurveData(const std::vector<double>&, double *, double *) override;

  int d_order;
  bool show_legend;
};

class LinearFit : public Fit
{
  Q_OBJECT

public:
  LinearFit() {}
  LinearFit(ApplicationWindow *parent, Graph *g);
  LinearFit(ApplicationWindow *parent, Graph *g, const QString& curveTitle);
  LinearFit(ApplicationWindow *parent, Graph *g, const QString& curveTitle, double start, double end);
  LinearFit(Graph& g, const QString& curve, double start, double end):
    LinearFit(0,&g,curve,start,end) {}
  LinearFit(Graph& g, const QString& curve): LinearFit(0,&g,curve) {}
  
  void fit() override;

private:
  void init();
  void calculateFitCurveData(const std::vector<double>&, double *, double *) override;
};
#endif

