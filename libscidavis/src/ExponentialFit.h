/***************************************************************************
    File                 : fitclasses.h
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Ion Vasilief, Tilman Benkert
    Email (use @ for *)  : ion_vasilief*yahoo.fr, thzs*gmx.net
    Description          : Exponential fit classes
                           
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
#ifndef EXPONENTIALFIT_H
#define EXPONENTIALFIT_H

#include "Fit.h"

class ExponentialFit : public Fit
{
  Q_OBJECT

public:
  ExponentialFit() {}
  ExponentialFit(ApplicationWindow *parent, Graph *g,  bool expGrowth = false);
  ExponentialFit(ApplicationWindow *parent, Graph *g, const QString& curveTitle, bool expGrowth = false);
  ExponentialFit(ApplicationWindow *parent, Graph *g, const QString& curveTitle, 
                 double start, double end, bool expGrowth = false);
  ExponentialFit(Graph& g, const QString& curveTitle, 
                 double start, double end, bool expGrowth):
    ExponentialFit(0,&g,curveTitle,start,end,expGrowth) {}
  ExponentialFit(Graph& g, const QString& curveTitle, 
                 double start, double end):
    ExponentialFit(0,&g,curveTitle,start,end,false) {}
  ExponentialFit(Graph& g, const QString& curveTitle, bool expGrowth):
    ExponentialFit(0,&g,curveTitle,expGrowth) {}
  ExponentialFit(Graph& g, const QString& curveTitle):
    ExponentialFit(0,&g,curveTitle,false) {}

private:
  void init();
  void storeCustomFitResults(const std::vector<double>&) override;
  void calculateFitCurveData(const std::vector<double>&, double *, double *) override;

  bool is_exp_growth;
};

class TwoExpFit : public Fit
{
  Q_OBJECT

public:
  TwoExpFit(ApplicationWindow *parent, Graph *g);
  TwoExpFit(ApplicationWindow *parent, Graph *g, const QString& curveTitle);
  TwoExpFit(ApplicationWindow *parent, Graph *g, const QString& curveTitle, double start, double end);
  TwoExpFit(Graph &g): TwoExpFit(0,&g) {}
  TwoExpFit(Graph &g, const QString& curveTitle): TwoExpFit(0,&g,curveTitle) {}
  TwoExpFit(Graph &g, const QString& curveTitle, double start, double end):
    TwoExpFit(0,&g,curveTitle,start,end) {}

private:
  void init();
  void storeCustomFitResults(const std::vector<double>&) override;
  void calculateFitCurveData(const std::vector<double>&, double *, double *) override;
};

class ThreeExpFit : public Fit
{
  Q_OBJECT

public:
  ThreeExpFit(ApplicationWindow *parent, Graph *g);
  ThreeExpFit(ApplicationWindow *parent, Graph *g, const QString& curveTitle);
  ThreeExpFit(ApplicationWindow *parent, Graph *g, const QString& curveTitle, double start, double end);
  ThreeExpFit(Graph &g): ThreeExpFit(0,&g) {}
  ThreeExpFit(Graph &g, const QString& curveTitle): ThreeExpFit(0,&g,curveTitle) {}
  ThreeExpFit(Graph &g, const QString& curveTitle, double start, double end):
    ThreeExpFit(0,&g,curveTitle,start,end) {}

private:
  void init();
  void storeCustomFitResults(const std::vector<double>& par) override;
  void calculateFitCurveData(const std::vector<double>&, double *, double *) override;
};
#endif
