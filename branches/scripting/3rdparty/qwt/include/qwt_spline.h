/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_SPLINE_H
#define QWT_SPLINE_H

#include "qwt_global.h"
#include "qwt_array.h"

/*!
  \brief A class for spline interpolation

  The QwtSpline class is used for cubical spline interpolation.
  Two types of splines, natural and periodic, are supported.
  
  \par Usage:
  <ol>
  <li>First call QwtSpline::buildSpline() to determine the spline coefficients 
      for a tabulated function y(x).
  <li>After the coefficients have been set up, the interpolated
      function value for an argument x can be determined by calling 
      QwtSpline::value().
  </ol>

  \par Example:
  \code
#include <qwt_spline.h>
#include <qwt_array.h>
#include <iostream.h>

const int numPoints = 30;
QwtArray x(numPoints); 
QwtArray y(numPoints); 

for(int i = 0; i < numPoints; i++)  // fill up x[] and y[]
   std::cin >> x[i] >> y[i];

QwtSpline s;
if ( s.buildSpline(x, y, numPoints, false) )    // build natural spline
{
    const int numValues = 300;

    QwtArray xInter(numValues); 
    QwtArray yInter(numValues);

    const double delta = (x[numPoints - 1] - x[0]) / (numValues - 1);
    for(i = 0; i < numValues; i++)  / interpolate
    {
        xInter[i] = x[0] + i * delta;
        yInter[i] = s.value( xInter[i] );
    }

    do_something(xInter, yInter);
}
else
    std::cerr << "Uhhh...\n";
  \endcode
*/

class QWT_EXPORT QwtSpline
{
public:
    QwtSpline();
    ~QwtSpline();

    double value(double x) const;
    bool buildSpline(
        const QwtArray<double> &x, const QwtArray<double> &y, 
        int n, bool periodic = false);

private:
    bool buildPeriodicSpline();
    bool buildNaturalSpline();
    int lookup(double x) const;
    void cleanup();

    class PrivateData;
    PrivateData *d_data;
};

#endif
