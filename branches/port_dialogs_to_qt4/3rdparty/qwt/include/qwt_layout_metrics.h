/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_LAYOUT_METRICS_H
#define QWT_LAYOUT_METRICS_H

#include <qsize.h>
#include "qwt_global.h"
//Added by qt3to4:
#include <Q3PointArray>

class QPainter;
class QRect;
class QPoint;
class Q3PointArray;
class QString;
class Q3SimpleRichText;
class Q3PaintDeviceMetrics;

class QWT_EXPORT QwtMetricsMap
{
public:
    QwtMetricsMap();

    bool isIdentity() const;

    void setMetrics(const Q3PaintDeviceMetrics &layoutMetrics,
        const Q3PaintDeviceMetrics &deviceMetrics);

    int layoutToDeviceX(int x) const;
    int deviceToLayoutX(int x) const;
    int screenToLayoutX(int x) const;
    int layoutToScreenX(int x) const;

    int layoutToDeviceY(int y) const;
    int deviceToLayoutY(int y) const;
    int screenToLayoutY(int y) const;
    int layoutToScreenY(int y) const;

    QPoint layoutToDevice(const QPoint &, const QPainter * = NULL) const;
    QPoint deviceToLayout(const QPoint &, const QPainter * = NULL) const;
    QPoint screenToLayout(const QPoint &) const;

    QSize layoutToDevice(const QSize &) const;
    QSize deviceToLayout(const QSize &) const;
    QSize screenToLayout(const QSize &) const;

    QRect layoutToDevice(const QRect &, const QPainter * = NULL) const;
    QRect deviceToLayout(const QRect &, const QPainter * = NULL) const;
    QRect screenToLayout(const QRect &) const;

    Q3PointArray layoutToDevice(const Q3PointArray &, 
        const QPainter * = NULL) const;
    Q3PointArray deviceToLayout(const Q3PointArray &, 
        const QPainter * = NULL) const;

    static QRect translate(const QMatrix &, const QRect &);
    static Q3PointArray translate(const QMatrix &, const Q3PointArray &);

private:
    double d_screenToLayoutX;
    double d_screenToLayoutY;

    double d_deviceToLayoutX;
    double d_deviceToLayoutY;
};


class QWT_EXPORT QwtLayoutMetrics
{
public:
    QwtLayoutMetrics();
    QwtLayoutMetrics(const QwtMetricsMap &);

    void setMap(const QwtMetricsMap &);

    QRect boundingRect(const QString &, int flags, QPainter *) const;
    QRect boundingRect(const QString &, int flags, const QFontMetrics &) const;

    int heightForWidth(const QString &,
        int width, int flags, const QFontMetrics &) const;
    int heightForWidth(const QString &,
        int width, int flags, QPainter *) const;

#ifndef QT_NO_RICHTEXT
    QRect boundingRect(const Q3SimpleRichText &, 
        int flags, QPainter * = NULL) const;
    int heightForWidth(Q3SimpleRichText &, int width) const;
#endif

private:
    QwtMetricsMap d_map;
};

inline bool QwtMetricsMap::isIdentity() const
{
    return d_deviceToLayoutX == 1.0 && d_deviceToLayoutY == 1.0;
}

inline int QwtMetricsMap::layoutToDeviceX(int x) const
{
    return qRound(x / d_deviceToLayoutX);
}

inline int QwtMetricsMap::deviceToLayoutX(int x) const
{
    return qRound(x * d_deviceToLayoutX);
}

inline int QwtMetricsMap::screenToLayoutX(int x) const
{
    return qRound(x * d_screenToLayoutX);
}

inline int QwtMetricsMap::layoutToScreenX(int x) const
{
    return qRound(x / d_screenToLayoutX);
}

inline int QwtMetricsMap::layoutToDeviceY(int y) const
{
    return qRound(y / d_deviceToLayoutY);
}

inline int QwtMetricsMap::deviceToLayoutY(int y) const
{
    return qRound(y * d_deviceToLayoutY);
}

inline int QwtMetricsMap::screenToLayoutY(int y) const
{
    return qRound(y * d_screenToLayoutY);
}

inline int QwtMetricsMap::layoutToScreenY(int y) const
{
    return qRound(y / d_screenToLayoutY);
}

inline QSize QwtMetricsMap::layoutToDevice(const QSize &size) const
{
    return QSize(layoutToDeviceX(size.width()), 
        layoutToDeviceY(size.height()));
}

inline QSize QwtMetricsMap::deviceToLayout(const QSize &size) const
{
    return QSize(deviceToLayoutX(size.width()), 
        deviceToLayoutY(size.height()));
}

inline QSize QwtMetricsMap::screenToLayout(const QSize &size) const
{
    return QSize(screenToLayoutX(size.width()), 
        screenToLayoutY(size.height()));
}

#endif
