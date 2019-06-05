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
#ifndef PYTHON_CLASSDESC_H
#define PYTHON_CLASSDESC_H

#include "python_base.h"
#include "QtEnums.h"

template <class K, class V> class QMap;
template <class T> class SciDAVisObject;

void exposeApplicationWindow(classdesc::python_t& p);

namespace classdesc_access
{
  template <>
  struct access_python<QString>
  {
    template <class C> 
    void type(classdesc::python_t& targ, const classdesc::string&)
    {
      auto& c=targ.getClass<C>();
      if (!c.completed)
        c.def("__str__",&QString::toStdString);
    }
  };

  template <class T>
  struct access_python<QList<T>>:
    public classdesc::NullDescriptor<classdesc::python_t> {};

  template <class E, class Q>
  struct access_python<QtEnumWrapper<E,Q>>
  {
    template <class C> 
    void type(classdesc::python_t& targ, const classdesc::string& desc)
    {
      targ.addEnum<C>(desc,&QtEnumWrapper<E,Q>::value);
    }
  };
}

namespace classdesc
{
  template <class E, class Q>
  struct tn<QtEnumWrapper<E,Q>>
  {
    static string name() {return typeName<E>();}
  };
  
  // Use Qt's MOC system for reflection on Qt objects
  template <class T>
  typename enable_if<is_base_of<QObject,T>, string>::T
  mocTypeName() {return T::staticMetaObject.className();}
}

#define DEF_TYPENAME(X)                             \
  namespace classdesc {                             \
    template <> struct tn<X>                        \
    {static string name() {return #X;}};            \
  }                                                 \

#define DEF_CTYPENAME(X)                        \
  class X;                                      \
  DEF_TYPENAME(X)                               \
  
  DEF_CTYPENAME(QString);
  DEF_CTYPENAME(QLocale);
  DEF_CTYPENAME(QColor);
  DEF_CTYPENAME(QSize);
  DEF_CTYPENAME(QFont);
  DEF_CTYPENAME(QPoint);
  DEF_CTYPENAME(QPointF);
  DEF_CTYPENAME(QStringList);
  DEF_CTYPENAME(QChar);
  DEF_CTYPENAME(QRect);
  DEF_CTYPENAME(QRectF);
  DEF_CTYPENAME(QPen);
  DEF_CTYPENAME(QIcon);
  DEF_CTYPENAME(QBrush);
  DEF_CTYPENAME(QPainter);
  DEF_CTYPENAME(QVariant);
  DEF_CTYPENAME(QwtPlotCurve);
  DEF_CTYPENAME(QwtPlotZoomer);
  DEF_CTYPENAME(QTreeWidgetItem);
  DEF_CTYPENAME(QDate);
  DEF_CTYPENAME(QDateTime);
  DEF_CTYPENAME(QTime);
  DEF_CTYPENAME(QwtPlotPrintFilter);
  DEF_CTYPENAME(QwtScaleMap);
  DEF_CTYPENAME(QPixmap);
  DEF_TYPENAME(Qt::WindowType);
  DEF_CTYPENAME(QwtSymbol);
  DEF_CTYPENAME(QTransform);

namespace Qwt3D {struct RGBA;}
DEF_TYPENAME(Qwt3D::RGBA);

#define DEF_TYPENAME_TEMPLATE1(X)                                \
  template <class T> class X;                                    \
  namespace classdesc {                                          \
    template <class T> struct tn<X<T>>                           \
    {static string name() {return "X<"+typeName<T>()+">";}};     \
  }                                                              \

DEF_TYPENAME_TEMPLATE1(QList);
DEF_TYPENAME_TEMPLATE1(QVector);
DEF_TYPENAME_TEMPLATE1(QPointer);
DEF_TYPENAME_TEMPLATE1(QFlags);

namespace classdesc
{
  template <class K, class V> struct tn<QMap<K,V>>
  {
    static string name() {return "QMap<"+typeName<K>()+","+typeName<V>()+">";}
  };

  template <class T> struct tn
  {
    static string name() {return mocTypeName<T>();}
  };

  // generates list semantics for QStringList
  template <> struct is_sequence<QStringList> {static const bool value=true;};
}

#define NULLDESC(X)                                                     \
  class X;                                                              \
  namespace classdesc_access {                                          \
  template <> struct access_python<X,void>:                             \
      public classdesc::NullDescriptor<classdesc::python_t> {};         \
  }

// define any dependent library class to have the null
// descriptor. Wrap these classes with SciDAVis classes if
// functionality needs exposing.
NULLDESC(QBrush);
NULLDESC(QChar);
NULLDESC(QColor);
NULLDESC(QDate);
NULLDESC(QDateTime);
NULLDESC(QDialog);
NULLDESC(QDockWidget);
NULLDESC(QFile);
NULLDESC(QFont);
NULLDESC(QIcon);
NULLDESC(QLocale);
NULLDESC(QMdiArea);
NULLDESC(QMdiSubWindow);
NULLDESC(QObject);
NULLDESC(QPainter);
NULLDESC(QPen);
NULLDESC(QPixmap);
NULLDESC(QPoint);
NULLDESC(QPointF);
NULLDESC(QRect);
NULLDESC(QRectF);
NULLDESC(QSize);
NULLDESC(QTextEdit);
NULLDESC(QTime);
NULLDESC(QTransform);
NULLDESC(QWidget);

NULLDESC(QwtPlotCurve);
NULLDESC(QwtPlotGrid);
NULLDESC(QwtPlotMarker);
NULLDESC(QwtPlotPrintFilter);
NULLDESC(QwtScaleMap);
NULLDESC(QwtSymbol);

#define NULLDESC1(X)                                                    \
  template <class T> class X;                                           \
  namespace classdesc_access {                                          \
    template <class T> struct access_python<X<T>,void>:                 \
      public classdesc::NullDescriptor<classdesc::python_t> {};         \
  }

NULLDESC1(SciDAVisObject);
NULLDESC1(QFlags);
NULLDESC1(QVector);  // TODO - maybe expose QVector as a sequence???
NULLDESC1(QPointer);

namespace Qwt3D {struct RGBA;}

namespace classdesc_access
{
  template <class K,class V> struct access_python<QMap<K,V>,void>:
    public classdesc::NullDescriptor<classdesc::python_t> {};

  template <> struct access_python<Qwt3D::RGBA>:
    public classdesc::NullDescriptor<classdesc::python_t> {};
}


#endif
