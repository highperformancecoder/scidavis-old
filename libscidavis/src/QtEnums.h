
#ifndef QTENUMS_H
#define QTENUMS_H
#include <Qt>
#include <qprinter.h>

namespace QtEnums
{
  enum PenStyle
    {
     NoPen=Qt::NoPen,
     SolidLine=Qt::SolidLine,
     DashLine=Qt::DashLine,
     DotLine=Qt::DotLine,
     DashDotLine=Qt::DashDotLine,
     DashDotDotLine=Qt::DashDotDotLine,
     CustomDashLine=Qt::CustomDashLine
    };
  enum PenCapStyle { 
        FlatCap = Qt::FlatCap,
        SquareCap = Qt::SquareCap,
        RoundCap = Qt::RoundCap,
        MPenCapStyle = Qt::MPenCapStyle
    };
  enum PenJoinStyle {
        MiterJoin = Qt::MiterJoin,
        BevelJoin = Qt::BevelJoin,
        RoundJoin = Qt::RoundJoin,
        SvgMiterJoin = Qt::SvgMiterJoin,
        MPenJoinStyle = Qt::MPenJoinStyle
    };

  enum DateFormat
    {
     TextDate = Qt::TextDate,
     ISODate = Qt::ISODate,
     SystemLocaleDate = Qt::SystemLocaleDate,
     LocalDate = Qt::LocalDate,
     LocaleDate = Qt::LocaleDate,
     SystemLocaleShortDate = Qt::SystemLocaleShortDate,
     SystemLocaleLongDate = Qt::SystemLocaleLongDate,
     DefaultLocaleShortDate = Qt::DefaultLocaleShortDate,
     DefaultLocaleLongDate = Qt::DefaultLocaleLongDate
    };
  
  enum TimeSpec
    {
        LocalTime = Qt::LocalTime,
        UTC = Qt::UTC,
        OffsetFromUTC = Qt::OffsetFromUTC
    };

  enum Orientation
    {
     Horizontal = Qt::Horizontal,
     Vertical = Qt::Vertical
    };

    enum AspectRatioMode
      {
       IgnoreAspectRatio=Qt::IgnoreAspectRatio,
       KeepAspectRatio=Qt::KeepAspectRatio,
       KeepAspectRatioByExpanding=Qt::KeepAspectRatioByExpanding
      };

}

#include "QtEnumWrapper.h"

typedef QtEnumWrapper<QtEnums::PenStyle, Qt::PenStyle> QtPenStyle;
typedef QtEnumWrapper<QtEnums::PenCapStyle, Qt::PenCapStyle> QtPenCapStyle;
typedef QtEnumWrapper<QtEnums::PenJoinStyle, Qt::PenJoinStyle> QtPenJoinStyle;
typedef QtEnumWrapper<QtEnums::DateFormat, Qt::DateFormat> QtDateFormat;
typedef QtEnumWrapper<QtEnums::TimeSpec, Qt::TimeSpec> QtTimeSpec;

struct QPrinterEnum
{
  enum Orient { Portrait=QPrinter::Portrait, Landscape=QPrinter::Landscape };
  typedef QtEnumWrapper<Orient, QPrinter::Orientation> Orientation;
  enum PS { A4=QPrinter::A4, B5=QPrinter::B5, Letter=QPrinter::Letter,
                  Legal=QPrinter::Legal, Executive=QPrinter::Executive,
                  A0=QPrinter::A0, A1=QPrinter::A1, A2=QPrinter::A2,
                  A3=QPrinter::A3, A5=QPrinter::A5, A6=QPrinter::A6,
                  A7=QPrinter::A7, A8=QPrinter::A8, A9=QPrinter::A9,
                  B0=QPrinter::B0, B1=QPrinter::B1, B10=QPrinter::B10,
                  B2=QPrinter::B2, B3=QPrinter::B3, B4=QPrinter::B4,
                  B6=QPrinter::B6, B7=QPrinter::B7, B8=QPrinter::B8,
                  B9=QPrinter::B9, C5E=QPrinter::C5E, Comm10E=QPrinter::Comm10E,
                  DLE=QPrinter::DLE, Folio=QPrinter::Folio, Ledger=QPrinter::Ledger,
                  Tabloid=QPrinter::Tabloid, Custom=QPrinter::Custom};
  typedef QtEnumWrapper<PS, QPrinter::PageSize> PageSize;
};

//#ifdef _CLASSDESC
//#pragma omit python QtEnumWrapper
//#endif

#endif
