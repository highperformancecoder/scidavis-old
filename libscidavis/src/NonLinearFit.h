/***************************************************************************
    File                 : NonLinearFit.h
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Ion Vasilief, Tilman Benkert
    Email (use @ for *)  : ion_vasilief*yahoo.fr, thzs*gmx.net
    Description          : NonLinearFit class

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
#ifndef NONLINEARFIT_H
#define NONLINEARFIT_H

#include "Fit.h"

class NonLinearFit : public Fit
{
  Q_OBJECT

public:
  NonLinearFit() {}
  NonLinearFit(ApplicationWindow *parent, Graph *g);
  NonLinearFit(ApplicationWindow *parent, Graph *g, const QString& curveTitle);
  NonLinearFit(ApplicationWindow *parent, Graph *g, const QString& curveTitle, double start, double end);
  NonLinearFit(Graph& g): NonLinearFit(0,&g) {}
  NonLinearFit(Graph& g, const QString& c): NonLinearFit(0,&g,c) {}
  NonLinearFit(Graph& g, const QString& c, double s, double e):
    NonLinearFit(0,&g,c,s,e) {}

  void setParametersList(const QStringList& lst);
  void setFormula(const QString& s);
  void setParameters(const pyobject&);
  
private:
  void calculateFitCurveData(const std::vector<double>&, double*, double*) override;
  void init();
};
#endif

