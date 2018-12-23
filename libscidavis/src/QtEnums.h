
#ifndef QTENUMS_H
#define QTENUMS_H
#include <Qt>

namespace QtEnums
{
  enum PenStyle{NoPen=Qt::NoPen, SolidLine=Qt::SolidLine, DashLine=Qt::DashLine};
  enum PenCapStyle {SquareCap=Qt::SquareCap, FlatCap=Qt::FlatCap};
}

#include "QtEnumWrapper.h"

typedef QtEnumWrapper<QtEnums::PenStyle, Qt::PenStyle> QtPenStyle;
typedef QtEnumWrapper<QtEnums::PenCapStyle, Qt::PenCapStyle> QtPenCapStyle;

//#ifdef _CLASSDESC
//#pragma omit python QtEnumWrapper
//#endif

#endif
