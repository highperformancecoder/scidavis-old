#include "Qt.h"
#include <qcolor.h>

#ifdef SCRIPTING_PYTHON
#include <boost/python.hpp>
#endif

const QColor QtNamespace::white{Qt::white};
const QColor QtNamespace::black{Qt::black};
const QColor QtNamespace::red{Qt::red};
const QColor QtNamespace::darkRed{Qt::darkRed};
const QColor QtNamespace::green{Qt::green};
const QColor QtNamespace::darkGreen{Qt::darkGreen};
const QColor QtNamespace::blue{Qt::blue};
const QColor QtNamespace::darkBlue{Qt::darkBlue};
const QColor QtNamespace::cyan{Qt::cyan};
const QColor QtNamespace::darkCyan{Qt::darkCyan};
const QColor QtNamespace::magenta{Qt::magenta};
const QColor QtNamespace::darkMagenta{Qt::darkMagenta};
const QColor QtNamespace::yellow{Qt::yellow};
const QColor QtNamespace::darkYellow{Qt::darkYellow};
const QColor QtNamespace::gray{Qt::gray};
const QColor QtNamespace::darkGray{Qt::darkGray};
const QColor QtNamespace::lightGray{Qt::lightGray};
const QColor QtNamespace::transparent{Qt::transparent};
const QColor QtNamespace::color0{Qt::color0};
const QColor QtNamespace::color1{Qt::color1};

void QtNamespace::QPen::setDashPattern(const pyobject &pattern)
{
#ifdef SCRIPTING_PYTHON
  QVector<qreal> p;
  for (int i=0; i<boost::python::len(pattern); ++i)
    p.append(boost::python::extract<qreal>(pattern[i]));
  setDashPattern(p);
#endif
}

