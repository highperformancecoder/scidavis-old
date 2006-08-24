/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include "qwt_spline.h"
#include "qwt_math.h"

class QwtSpline::PrivateData
{
public:
    PrivateData():
        size(0)
    {
    }

    // coefficient vectors
    QwtArray<double> a;
    QwtArray<double> b;
    QwtArray<double> c;
    QwtArray<double> d;

    // values
    QwtArray<double> x;
    QwtArray<double> y;
    int size;
};

//! CTOR
QwtSpline::QwtSpline()
{
    d_data = new PrivateData;
}

//! DTOR
QwtSpline::~QwtSpline()
{
    delete d_data;
}

/*!
  Calculate the interpolated function value corresponding 
  to a given argument x.
*/
double QwtSpline::value(double x) const
{
    if (d_data->a.size() == 0)
        return 0.0;

    const int i = lookup(x);

    const double delta = x - d_data->x[i];
    return( ( ( ( d_data->a[i] * delta) + d_data->b[i] ) 
        * delta + d_data->c[i] ) * delta + d_data->y[i] );
}

//! Determine the function table index corresponding to a value x
int QwtSpline::lookup(double x) const
{
    int i1;
    
    if (x <= d_data->x[0])
       i1 = 0;
    else if (x >= d_data->x[d_data->size - 2])
       i1 = d_data->size -2;
    else
    {
        i1 = 0;
        int i2 = d_data->size -2;
        int i3 = 0;

        while ( i2 - i1 > 1 )
        {
            i3 = i1 + ((i2 - i1) >> 1);

            if (d_data->x[i3] > x)
               i2 = i3;
            else
               i1 = i3;

        }
    }
    return i1;
}


/*!
  \brief Calculate the spline coefficients

  Depending on the value of \a periodic, this function
  will determine the coefficients for a natural or a periodic
  spline and store them internally. 
  
  \param x
  \param y points
  \param size number of points
  \param periodic if true, calculate periodic spline
  \return true if successful
  \warning The sequence of x (but not y) values has to be strictly monotone
           increasing, which means <code>x[0] < x[1] < .... < x[n-1]</code>.
       If this is not the case, the function will return false
*/
bool QwtSpline::buildSpline(
    const QwtArray<double> &x, const QwtArray<double> &y, 
    int size, bool periodic)
{
    if (size <= 2) {
        cleanup();
        return false;
    }

    d_data->size = size;

#if QT_VERSION < 0x040000
    // With Qt3 this is a deep copy.
    d_data->x = x.copy();
    d_data->y = y.copy();
#else
    d_data->x = x;
    d_data->y = y;
#endif
    
    d_data->a.resize(size-1);
    d_data->b.resize(size-1);
    d_data->c.resize(size-1);

    bool ok;
    if(periodic)
       ok =  buildPeriodicSpline();
    else
       ok =  buildNaturalSpline();

    if (!ok) 
        cleanup();

    return ok;
}

/*!
  \brief Determines the coefficients for a natural spline
  \return true if successful
*/
bool QwtSpline::buildNaturalSpline()
{
    int i;
    
    QwtArray<double> d(d_data->size-1);
    QwtArray<double> h(d_data->size-1);
    QwtArray<double> s(d_data->size);

    const double *x = d_data->x.data();
    const double *y = d_data->y.data();
    double *a = d_data->a.data();
    double *b = d_data->b.data();
    double *c = d_data->c.data();
    const int &size = d_data->size;

    //
    //  set up tridiagonal equation system; use coefficient
    //  vectors as temporary buffers
    for (i = 0; i < size - 1; i++) 
    {
        h[i] = x[i+1] - x[i];
        if (h[i] <= 0)
            return false;
    }
    
    double dy1 = (y[1] - y[0]) / h[0];
    for (i = 1; i < size - 1; i++)
    {
        b[i] = c[i] = h[i];
        a[i] = 2.0 * (h[i-1] + h[i]);

        const double dy2 = (y[i+1] - y[i]) / h[i];
        d[i] = 6.0 * ( dy1 - dy2);
        dy1 = dy2;
    }

    //
    // solve it
    //
    
    // L-U Factorization
    for(i = 1; i < size - 2;i++)
    {
        c[i] /= a[i];
        a[i+1] -= b[i] * c[i]; 
    }

    // forward elimination
    s[1] = d[1];
    for ( i = 2; i < size - 1; i++)
       s[i] = d[i] - c[i-1] * s[i-1];
    
    // backward elimination
    s[size - 2] = - s[size - 2] / a[size - 2];
    for (i= size -3; i > 0; i--)
       s[i] = - (s[i] + b[i] * s[i+1]) / a[i];

    //
    // Finally, determine the spline coefficients
    //
    s[size - 1] = s[0] = 0.0;
    for (i = 0; i < size - 1; i++)
    {
        a[i] = ( s[i+1] - s[i] ) / ( 6.0 * h[i]);
        b[i] = 0.5 * s[i];
        c[i] = ( y[i+1] - y[i] ) 
            / h[i] - (s[i+1] + 2.0 * s[i] ) * h[i] / 6.0; 
    }

    return true;
}

/*!
  \brief Determines the coefficients for a periodic spline
  \return true if successful
*/
bool QwtSpline::buildPeriodicSpline()
{
    int i;
    
    QwtArray<double> d(d_data->size-1);
    QwtArray<double> h(d_data->size-1);
    QwtArray<double> s(d_data->size);
    
    const double *x = d_data->x.data();
    const double *y = d_data->y.data();
    double *a = d_data->a.data();
    double *b = d_data->b.data();
    double *c = d_data->c.data();
    const int &size = d_data->size;

    //
    //  setup equation system; use coefficient
    //  vectors as temporary buffers
    //
    for (i = 0; i < size - 1; i++)
    {
        h[i] = x[i+1] - x[i];
        if (h[i] <= 0.0)
            return false;
    }
    
    const int imax = size - 2;
    double htmp = h[imax];
    double dy1 = (y[0] - y[imax]) / htmp;
    for (i = 0; i <= imax; i++)
    {
        b[i] = c[i] = h[i];
        a[i] = 2.0 * (htmp + h[i]);
        const double dy2 = (y[i+1] - y[i]) / h[i];
        d[i] = 6.0 * ( dy1 - dy2);
        dy1 = dy2;
        htmp = h[i];
    }

    //
    // solve it
    //
    
    // L-U Factorization
    a[0] = sqrt(a[0]);
    c[0] = h[imax] / a[0];
    double sum = 0;

    for( i = 0; i < imax - 1; i++)
    {
        b[i] /= a[i];
        if (i > 0)
           c[i] = - c[i-1] * b[i-1] / a[i];
        a[i+1] = sqrt( a[i+1] - qwtSqr(b[i]));
        sum += qwtSqr(c[i]);
    }
    b[imax-1] = (b[imax-1] - c[imax-2] * b[imax-2]) / a[imax-1];
    a[imax] = sqrt(a[imax] - qwtSqr(b[imax-1]) - sum);
    

    // forward elimination
    s[0] = d[0] / a[0];
    sum = 0;
    for(i=1;i<imax;i++)
    {
        s[i] = (d[i] - b[i-1] * s[i-1]) / a[i];
        sum += c[i-1] * s[i-1];
    }
    s[imax] = (d[imax] - b[imax-1] * s[imax-1] - sum) / a[imax];
    
    
    // backward elimination
    s[imax] = - s[imax] / a[imax];
    s[imax-1] = -(s[imax-1] + b[imax-1] * s[imax]) / a[imax-1];
    for (i= imax - 2; i >= 0; i--)
       s[i] = - (s[i] + b[i] * s[i+1] + c[i] * s[imax]) / a[i];

    //
    // Finally, determine the spline coefficients
    //
    s[size-1] = s[0];
    for ( i=0; i < size-1; i++)
    {
        a[i] = ( s[i+1] - s[i] ) / ( 6.0 * h[i]);
        b[i] = 0.5 * s[i];
        c[i] = ( y[i+1] - y[i] ) 
            / h[i] - (s[i+1] + 2.0 * s[i] ) * h[i] / 6.0; 
    }

    return true;
}


//! Free allocated memory and set size to 0
void QwtSpline::cleanup()
{
    d_data->size = 0;
    d_data->a.resize(0);
    d_data->b.resize(0);
    d_data->c.resize(0);
    d_data->x.resize(0);
    d_data->y.resize(0);
}
