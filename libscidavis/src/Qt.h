#ifndef QT_SCIDAVIS_H
#define QT_SCIDAVIS_H
#include <qcolor.h>
#include <qprinter.h>
#include <QDateTime>
#include <QPen>
#include <QFont>

#include <QtEnums.h>

#ifdef SCRIPTING_PYTHON
#include <boost/python.hpp>
typedef boost::python::object pyobject;
namespace py=boost::python;
#else
struct pyobject;
#endif

/// wrap a Qt container with Python accessor method
template <class T>
struct PyQtList: public QList<T*>
{
  T& __getitem__(int i) {
    if (i<0)
      i+=this->size();
    if (i>=0 && i<this->size())
      return *(*this)[i];
    else
      throw std::out_of_range("index out of bounds");
  }
  size_t __len__() const {return this->size();}
  bool __contains__(const T& x) const {
    return this->count(const_cast<T*>(&x));
  }
};

template <class T>
struct PyQtVector: public QVector<T>
{
  PyQtVector() {}
  PyQtVector(const QVector<T>& x): QVector<T>(x) {}
  T __getitem__(int i) {
    if (i<0)
      i+=this->size();
    if (i>=0 && i<this->size())
      return (*this)[i];
    else
      throw std::out_of_range("index out of bounds");
  }
  size_t __len__() const {return this->size();}
  bool __contains__(const T& x) const {
    return this->count(x);
  }
};

namespace QtGui
{
  class QTransform: public ::QTransform
  {
  public:
    enum TransformationType {
                             TxNone      = ::QTransform::TxNone,
                             TxTranslate = ::QTransform::TxTranslate,
                             TxScale     = ::QTransform::TxScale,
                             TxRotate    = ::QTransform::TxRotate,
                             TxShear     = ::QTransform::TxShear,
                             TxProject   = ::QTransform::TxProject
    };

    //inline explicit QTransform(Qt::Initialization) : affine(Qt::Uninitialized) {}
    QTransform() {}
    QTransform(qreal h11, qreal h12, qreal h13,
               qreal h21, qreal h22, qreal h23,
               qreal h31, qreal h32, qreal h33):
      ::QTransform(h11,h12,h13,h21,h22,h23,h31,h32,h33) {}
    QTransform(qreal h11, qreal h12, qreal h13,
               qreal h21, qreal h22, qreal h23,
               qreal h31, qreal h32):
      QTransform(h11,h12,h13,h21,h22,h23,h31,h32,1.0) {}
    QTransform(qreal h11, qreal h12, qreal h21,
               qreal h22, qreal dx, qreal dy):
      ::QTransform(h11,h12,h21,h22,dx,dy) {}
    //explicit QTransform(const QMatrix &mtx);
    QTransform(const ::QTransform& x): ::QTransform(x) {}

    bool isAffine() const {return ::QTransform::isAffine();}
    bool isIdentity() const {return ::QTransform::isIdentity();}
    bool isInvertible() const {return ::QTransform::isInvertible();}
    bool isScaling() const {return ::QTransform::isScaling();}
    bool isRotating() const {return ::QTransform::isRotating();}
    bool isTranslating() const {return ::QTransform::isTranslating();}

    TransformationType type() const {return TransformationType(::QTransform::type());}

    qreal determinant() const {return ::QTransform::determinant();}
    qreal det() const {return ::QTransform::det();}

    qreal m11() const {return ::QTransform::m11();}
    qreal m12() const {return ::QTransform::m12();}
    qreal m13() const {return ::QTransform::m13();}
    qreal m21() const {return ::QTransform::m21();}
    qreal m22() const {return ::QTransform::m22();}
    qreal m23() const {return ::QTransform::m23();}
    qreal m31() const {return ::QTransform::m23();}
    qreal m32() const {return ::QTransform::m32();}
    qreal m33() const {return ::QTransform::m33();}
    qreal dx() const {return ::QTransform::dx();}
    qreal dy() const {return ::QTransform::dy();}

    void setMatrix(qreal m11, qreal m12, qreal m13,
                   qreal m21, qreal m22, qreal m23,
                   qreal m31, qreal m32, qreal m33)
    {::QTransform::setMatrix(m11,m12,m13,m21,m22,m23,m31,m32,m33);}

    QtGui::QTransform inverted() const {return ::QTransform::inverted();}
    QtGui::QTransform adjoint() const {return ::QTransform::adjoint();}
    QtGui::QTransform transposed() const {return ::QTransform::transposed();}

    QtGui::QTransform &translate(qreal dx, qreal dy) {::QTransform::translate(dx,dy); return *this;}
    QtGui::QTransform &scale(qreal sx, qreal sy) {::QTransform::scale(sx,sy); return *this;}
    QtGui::QTransform &shear(qreal sh, qreal sv) {::QTransform::shear(sh,sv); return *this;}
    QtGui::QTransform &rotate(qreal a, QtEnums::Axis axis) {
      ::QTransform::rotate(a,Qt::Axis(axis)); return *this;}
    QtGui::QTransform &rotate(qreal a) {return rotate(a,QtEnums::ZAxis);}
    QtGui::QTransform &rotateRadians(qreal a, QtEnums::Axis axis) {
      ::QTransform::rotateRadians(a,Qt::Axis(axis)); return *this;}
    QtGui::QTransform &rotateRadians(qreal a) {return rotateRadians(a,QtEnums::ZAxis);}


    bool __eq__(const QtGui::QTransform& x) const {return ::QTransform::operator==(x);}
    bool __ne__(const QtGui::QTransform& x) const {return ::QTransform::operator!=(x);}

    //    static bool squareToQuad(const QPolygonF &square, QTransform &result);
    //    static bool quadToSquare(const QPolygonF &quad, QTransform &result);
    //    static bool quadToQuad(const QPolygonF &one,
    //                           const QPolygonF &two,
    //                           QTransform &result);

    void reset() {::QTransform::reset();}
    //    QPoint       map(const QPoint &p) const;
    //    QPointF      map(const QPointF &p) const;
    //    QLine        map(const QLine &l) const;
    //    QLineF       map(const QLineF &l) const;
    //    QPolygonF    map(const QPolygonF &a) const;
    //    QPolygon     map(const QPolygon &a) const;
    //    QRegion      map(const QRegion &r) const;
    //    QPainterPath map(const QPainterPath &p) const;
    //    QPolygon     mapToPolygon(const QRect &r) const;
    //    QRect mapRect(const QRect &) const;
    //    QRectF mapRect(const QRectF &) const;
    //    void map(int x, int y, int *tx, int *ty) const;
    //    void map(qreal x, qreal y, qreal *tx, qreal *ty) const;

    //    const QMatrix &toAffine() const;

    static QtGui::QTransform fromTranslate(qreal dx, qreal dy) {return ::QTransform::fromTranslate(dx,dy);}
    static QtGui::QTransform fromScale(qreal dx, qreal dy) {return ::QTransform::fromScale(dx,dy);}
  };
}

struct QtNamespace: public QtEnums
{
  static const QColor white;
  static const QColor black;
  static const QColor red;
  static const QColor darkRed;
  static const QColor green;
  static const QColor darkGreen;
  static const QColor blue;
  static const QColor darkBlue;
  static const QColor cyan;
  static const QColor darkCyan;
  static const QColor magenta;
  static const QColor darkMagenta;
  static const QColor yellow;
  static const QColor darkYellow;
  static const QColor gray;
  static const QColor darkGray;
  static const QColor lightGray;
  static const QColor transparent;
  static const QColor color0;
  static const QColor color1;


  
  struct QBrush: public ::QBrush
  {
    QBrush() {}
    QBrush(QtEnums::BrushStyle bs): ::QBrush(Qt::BrushStyle(bs)) {}
    QBrush(const QColor &color, QtEnums::BrushStyle bs):
      ::QBrush(color, Qt::BrushStyle(bs)) {}
    QBrush(const QColor &color): QBrush(color,QtEnums::SolidPattern) {}
    //    QBrush(QtEnums::GlobalColor color, QtEnums::BrushStyle bs=Qt::SolidPattern);

    //    QBrush(const QColor &color, const QPixmap &pixmap);
    //    QBrush(QtEnums::GlobalColor color, const QPixmap &pixmap);
    //QBrush(const QPixmap &pixmap);
    //QBrush(const QImage &image);

    QBrush(const ::QBrush &brush): ::QBrush(brush) {}

    //QBrush(const QGradient &gradient);

    QtEnums::BrushStyle style() const {return QtEnums::BrushStyle(::QBrush::style());}
    void setStyle(QtEnums::BrushStyle s) {::QBrush::setStyle(Qt::BrushStyle(s));}

    //    inline const QMatrix &matrix() const;
    //    void setMatrix(const QMatrix &mat);

    QtGui::QTransform transform() const {return ::QBrush::transform();}
    void setTransform(const QtGui::QTransform & t) {::QBrush::setTransform(t);}

    //    QPixmap texture() const;
    //    void setTexture(const QPixmap &pixmap);

    //    QImage textureImage() const;
    //    void setTextureImage(const QImage &image);

    QColor color() const {return ::QBrush::color();}
    void setColor(const QColor& c) {::QBrush::setColor(c);}
    //    inline void setColor(Qt::GlobalColor color);

    //    const QGradient *gradient() const;

    bool isOpaque() const {return ::QBrush::isOpaque();}

    bool isDetached() const {return ::QBrush::isDetached();}
  };

  struct QPen: public ::QPen
  {
    QPen() {}
    QPen(QtEnums::PenStyle x): ::QPen(x) {}
    QPen(const QColor &color): ::QPen(color) {}
    //    QPen(const QBrush &brush, qreal width, Qt::PenStyle s = Qt::SolidLine,
    //         Qt::PenCapStyle c = Qt::SquareCap, Qt::PenJoinStyle j = Qt::BevelJoin)
    
    QPen(const ::QPen &pen): ::QPen(pen) {}

    //    inline void swap(QPen &other) { qSwap(d, other.d); }

    QtEnums::PenStyle style() const {return QtEnums::PenStyle(::QPen::style());}
    void setStyle(QtEnums::PenStyle s) {::QPen::setStyle(Qt::PenStyle(s));}

    PyQtVector<qreal> dashPattern() const {return ::QPen::dashPattern();}
    void setDashPattern(const QVector<qreal> &pattern)
    {::QPen::setDashPattern(pattern);}
    void setDashPattern(const pyobject &pattern);

    qreal dashOffset() const {return ::QPen::dashOffset();}
    void setDashOffset(qreal doffset) {::QPen::setDashOffset(doffset);}

    qreal miterLimit() const {return ::QPen::miterLimit();}
    void setMiterLimit(qreal limit) {::QPen::setMiterLimit(limit);}

    qreal widthF() const {return ::QPen::widthF();}
    void setWidthF(qreal width) {::QPen::setWidthF(width);}

    int width() const {return ::QPen::width();}
    void setWidth(int width) {::QPen::setWidth(width);}

    QColor color() const {return ::QPen::color();}
    void setColor(const QColor &color) {::QPen::setColor(color);}

    QBrush brush() const {return ::QPen::brush();}
    void setBrush(const QBrush &brush) {::QPen::setBrush(brush);}

    bool isSolid() const {return ::QPen::isSolid();}

    QtEnums::PenCapStyle capStyle() const {return QtEnums::PenCapStyle(::QPen::capStyle());}
    void setCapStyle(QtEnums::PenCapStyle pcs) {::QPen::setCapStyle(Qt::PenCapStyle(pcs));}

    QtEnums::PenJoinStyle joinStyle() const {return QtEnums::PenJoinStyle(::QPen::joinStyle());}
    void setJoinStyle(QtEnums::PenJoinStyle pcs) {::QPen::setJoinStyle(Qt::PenJoinStyle(pcs));}

    bool isCosmetic() const {return ::QPen::isCosmetic();}
    void setCosmetic(bool cosmetic) {return setCosmetic(cosmetic);}

    bool isDetached() {return ::QPen::isDetached();}

  };


};



// shims around Qt classes to allow automatic exposure to python
namespace QtCore
{
  enum QDMonthNameType {DateFormat = ::QDate::DateFormat,
                        StandaloneFormat=::QDate::StandaloneFormat};
  struct QDate: public ::QDate
  {
    QDate() {}
    QDate(int y, int m, int d): ::QDate(y,m,d) {}
    QDate(const ::QDate& x): ::QDate(x) {}

    bool isNull() const {return ::QDate::isNull();}
    bool isValid() const {return ::QDate::isValid();}

    int year() const {return ::QDate::year();}
    int month() const {return ::QDate::month();}
    int day() const {return ::QDate::day();}
    int dayOfWeek() const {return ::QDate::dayOfWeek();}
    int dayOfYear() const {return ::QDate::dayOfYear();}
    int daysInMonth() const {return ::QDate::daysInMonth();}
    int daysInYear() const {return ::QDate::daysInYear();}
    int weekNumber() const {return ::QDate::weekNumber();}
    int weekNumber(int& yearNum) const {return ::QDate::weekNumber(&yearNum);}

    static std::string shortMonthName(int month)
    {return ::QDate::shortMonthName(month).toStdString();}
    static std::string shortMonthName(int month, QDMonthNameType type)
    {return ::QDate::shortMonthName(month,::QDate::MonthNameType(int(type))).toStdString();}
    static std::string shortDayName(int weekday)
    {return ::QDate::shortDayName(weekday).toStdString();}
    static std::string shortDayName(int weekday, QDMonthNameType type)
    {return ::QDate::shortDayName(weekday,::QDate::MonthNameType(int(type))).toStdString();}
    static std::string longMonthName(int month)
    {return ::QDate::longMonthName(month).toStdString();}
    static std::string longMonthName(int month, QDMonthNameType type)
    {return ::QDate::longMonthName(month,::QDate::MonthNameType(int(type))).toStdString();}
    static std::string longDayName(int weekday)
    {return ::QDate::longDayName(weekday).toStdString();}
    static std::string longDayName(int weekday, QDMonthNameType type)
    {return ::QDate::longDayName(weekday,::QDate::MonthNameType(int(type))).toStdString();}
    std::string toString() const
    {return ::QDate::toString().toStdString();}
    std::string toString(QtEnums::DateFormat f) const
    {return ::QDate::toString(Qt::DateFormat(int(f))).toStdString();}
    std::string toString(const std::string& format) const
    {return ::QDate::toString(format.c_str()).toStdString();}

    bool setDate(int year, int month, int day)
    {return ::QDate::setDate(year,month,day);}

    void getDate(int& year, int& month, int& day)
    {::QDate::getDate(&year,&month,&day);}
    
    QtCore::QDate addDays(int days) const {return ::QDate::addDays(days);}
    QtCore::QDate addMonths(int months) const {return ::QDate::addMonths(months);}
    QtCore::QDate addYears(int years) const {return ::QDate::addYears(years);}
    int daysTo(const QtCore::QDate& x) const {return ::QDate::daysTo(x);}

    static QtCore::QDate currentDate() {return ::QDate::currentDate();}
    static QtCore::QDate fromString(const std::string& s) 
    {return ::QDate::fromString(s.c_str());}
    static QtCore::QDate fromString(const std::string& s, QtDateFormat f) 
    {return ::QDate::fromString(s.c_str(), f);}
    static QtCore::QDate fromString(const std::string& s, const std::string& format)
    {return ::QDate::fromString(s.c_str(), format.c_str());}
    static bool isValid(int y, int m, int d) {return ::QDate::isValid(y,m,d);}
    static bool isLeapYear(int year) {return ::QDate::isLeapYear(year);}
  };
  
  struct QTime: public ::QTime
  {
    QTime() {}
    QTime(const ::QTime& x): ::QTime(x) {}
    QTime(int h, int m): ::QTime(h,m) {}
    QTime(int h, int m, int s): ::QTime(h,m,s) {}
    QTime(int h, int m, int s, int ms): ::QTime(h,m,s,ms) {}

    bool isNull() const {return ::QTime::isNull();}
    bool isValid() const {return ::QTime::isValid();}

    int hour() const {return ::QTime::hour();}
    int minute() const{return ::QTime::minute();}
    int second() const{return ::QTime::second();}
    int msec() const{return ::QTime::msec();}
    std::string toString() const{return ::QTime::toString().toStdString();}
    std::string toString(QtEnums::DateFormat f) const
    {return ::QTime::toString(Qt::DateFormat(int(f))).toStdString();}
    std::string toString(const std::string& f) const
    {return ::QTime::toString(f.c_str()).toStdString();}
    bool setHMS(int h, int m, int s){return ::QTime::setHMS(h,m,s);}
    bool setHMS(int h, int m, int s, int ms){return ::QTime::setHMS(h,m,s,ms);}

    QtCore::QTime addSecs(int secs) const {return ::QTime::addSecs(secs);}
    int secsTo(const QtCore::QTime& t) const {return ::QTime::secsTo(t);}
    QtCore::QTime addMSecs(int ms) const {return ::QTime::addMSecs(ms);}
    int msecsTo(const QtCore::QTime& t) const {return ::QTime::msecsTo(t);}

    static QtCore::QTime currentTime() {return ::QTime::currentTime();}
    static QtCore::QTime fromString(const std::string &s) {return ::QTime::fromString(s.c_str());}
    static QtCore::QTime fromString(const std::string &s, QtDateFormat f)
    {return ::QTime::fromString(s.c_str(),f);}
    static QtCore::QTime fromString(const std::string &s, const std::string &f) {
      return ::QTime::fromString(s.c_str(),f.c_str());}
    static bool isValid(int h, int m, int s) {return ::QTime::isValid(h,m,s);}
    static bool isValid(int h, int m, int s, int ms){return ::QTime::isValid(h,m,s,ms);}

    void start() {::QTime::start();}
    int restart() {return ::QTime::restart();}
    int elapsed() const {return ::QTime::elapsed();}
  };

  struct QDateTime: public ::QDateTime
  {
    QDateTime() {}
    explicit QDateTime(const ::QDate& x): ::QDateTime(x) {}
    QDateTime(const ::QDateTime& x): ::QDateTime(x) {}
    QDateTime(const ::QDate& d, const ::QTime& t): ::QDateTime(d,t) {}
    QDateTime(const ::QDate& d, const ::QTime& t, QtTimeSpec s): ::QDateTime(d,t,s) {}
    QDateTime(int y, int m, int d): ::QDateTime(QDate(y,m,d)) {}
    QDateTime(int y, int m, int d, int h, int mi): ::QDateTime(::QDate(y,m,d),::QTime(h,mi)) {}
    QDateTime(int y, int m, int d, int h, int mi, int s):
      ::QDateTime(::QDate(y,m,d),::QTime(h,mi,s)) {}
    QDateTime(int y, int m, int d, int h, int mi, int s, int ms):
      ::QDateTime(::QDate(y,m,d),::QTime(h,mi,s,ms)) {}

    bool isNull() const {return ::QDateTime::isNull();}
    bool isValid() const {return ::QDateTime::isValid();}

    QtCore::QDate date() const {return ::QDateTime::date();}
    QtCore::QTime time() const {return ::QDateTime::time();}
    QtTimeSpec timeSpec() const {return ::QDateTime::timeSpec();}
    qint64 toMSecsSinceEpoch() const {return ::QDateTime::toMSecsSinceEpoch();}
    uint toTime_t() const {return ::QDateTime::toTime_t();}
    void setDate(const QtCore::QDate &date) {::QDateTime::setDate(date);}
    void setTime(const QtCore::QTime &time) {::QDateTime::setTime(time);}
    void setTimeSpec(QtTimeSpec spec) {::QDateTime::setTimeSpec(spec);}
    void setMSecsSinceEpoch(qint64 msecs) {::QDateTime::setMSecsSinceEpoch(msecs);}
    void setTime_t(uint secsSince1Jan1970UTC) {::QDateTime::setTime_t(secsSince1Jan1970UTC);}
    std::string toString() const{return ::QDateTime::toString().toStdString();}
    std::string toString(QtEnums::DateFormat f) const
    {return ::QDateTime::toString(Qt::DateFormat(int(f))).toStdString();}
    std::string toString(const std::string &format) const
    {return ::QDateTime::toString(format.c_str()).toStdString();}
    QtCore::QDateTime addDays(int days) const {return ::QDateTime::addDays(days);}
    QtCore::QDateTime addMonths(int months) const {return ::QDateTime::addMonths(months);}
    QtCore::QDateTime addYears(int years) const {return ::QDateTime::addYears(years);}
    QtCore::QDateTime addSecs(int secs) const {return ::QDateTime::addSecs(secs);}
    QtCore::QDateTime addMSecs(qint64 msecs) const {return ::QDateTime::addMSecs(msecs);}
    QtCore::QDateTime toTimeSpec(QtTimeSpec spec) const {return ::QDateTime::toTimeSpec(spec);}
    QtCore::QDateTime toLocalTime() const {return ::QDateTime::toLocalTime();}
    QtCore::QDateTime toUTC() const {return ::QDateTime::toUTC();}
    int daysTo(const QtCore::QDateTime& x) const {return ::QDateTime::daysTo(x);}
    int secsTo(const QtCore::QDateTime& x) const {return ::QDateTime::secsTo(x);}
    qint64 msecsTo(const QtCore::QDateTime& x) const {return ::QDateTime::msecsTo(x);}

    void setUtcOffset(int seconds) {::QDateTime::setUtcOffset(seconds);}
    int utcOffset() const {return ::QDateTime::utcOffset();}

    static QtCore::QDateTime currentDateTime() {return ::QDateTime::currentDateTime();}
    static QtCore::QDateTime currentDateTimeUtc() {return ::QDateTime::currentDateTimeUtc();}
    static QtCore::QDateTime fromString(const std::string& s) {return ::QDateTime::fromString(s.c_str());}
    static QtCore::QDateTime fromString(const std::string& s, QtDateFormat f)
    {return ::QDateTime::fromString(s.c_str(),f);}
    static QtCore::QDateTime fromString(const std::string& s, const std::string& f)
    {return ::QDateTime::fromString(s.c_str(),f.c_str());}
    static QtCore::QDateTime fromTime_t(uint secsSince1Jan1970UTC)
    {return ::QDateTime::fromTime_t(secsSince1Jan1970UTC);}
    static QtCore::QDateTime fromMSecsSinceEpoch(qint64 msecs)
    {return ::QDateTime::fromMSecsSinceEpoch(msecs);}
    static qint64 currentMSecsSinceEpoch() {return ::QDateTime::currentMSecsSinceEpoch();}
  };

  struct QSize: ::QSize
  {
    QSize() {}
    QSize(int w, int h): ::QSize(w,h) {}
    QSize(const ::QSize& x): ::QSize(x) {}

    bool isNull() const {return ::QSize::isNull();}
    bool isEmpty() const {return ::QSize::isEmpty();}
    bool isValid() const {return ::QSize::isValid();}

    int width() const {return ::QSize::width();}
    int height() const {return ::QSize::height();}
    void setWidth(int w) {::QSize::setWidth(w);}
    void setHeight(int h) {::QSize::setHeight(h);}
    void transpose() {::QSize::transpose();}

    void scale(int w, int h, QtEnums::AspectRatioMode mode)
    {::QSize::scale(w,h,Qt::AspectRatioMode(mode));}
    void scale(const QtCore::QSize &s, QtEnums::AspectRatioMode mode)
    {::QSize::scale(s,Qt::AspectRatioMode(mode));}

    QtCore::QSize expandedTo(const QtCore::QSize & x) const 
    {return ::QSize::expandedTo(x);}
    QtCore::QSize boundedTo(const QtCore::QSize & x) const
    {return ::QSize::boundedTo(x);}

//    int &rwidth() {return ::QSize::rwidth();}
//    int &rheight() {return ::QSize::rheight();}
  };

};

namespace QtGui
{
  struct QFont: public ::QFont
    {
      enum StyleHint {
                      Helvetica=::QFont::Helvetica,
                      SansSerif =::QFont::SansSerif,
                      Times=::QFont::Times,
                      Serif = ::QFont::Serif,
                      Courier = ::QFont::Courier,
                      TypeWriter = ::QFont::TypeWriter,
                      OldEnglish = ::QFont::OldEnglish,
                      Decorative = ::QFont::Decorative,
                      System = ::QFont::System,
                      AnyStyle = ::QFont::AnyStyle,
                      Cursive = ::QFont::Cursive,
                      Monospace = ::QFont::Monospace,
                      Fantasy = ::QFont::Fantasy
      };

      enum StyleStrategy {
                          PreferDefault       = ::QFont::PreferDefault,
                          PreferBitmap        = ::QFont::PreferBitmap,
                          PreferDevice        = ::QFont::PreferDevice,
                          PreferOutline       = ::QFont::PreferOutline,
                          ForceOutline        = ::QFont::ForceOutline,
                          PreferMatch         = ::QFont::PreferMatch,
                          PreferQuality       = ::QFont::PreferQuality,
                          PreferAntialias     = ::QFont::PreferAntialias,
                          NoAntialias         = ::QFont::NoAntialias,
                          OpenGLCompatible    = ::QFont::OpenGLCompatible,
                          ForceIntegerMetrics = ::QFont::ForceIntegerMetrics,
                          NoFontMerging       = ::QFont::NoFontMerging
      };

      enum HintingPreference {
                              PreferDefaultHinting        = ::QFont::PreferDefaultHinting,
                              PreferNoHinting             = ::QFont::PreferNoHinting,
                              PreferVerticalHinting       = ::QFont::PreferVerticalHinting,
                              PreferFullHinting           = ::QFont::PreferFullHinting
      };

      enum Weight {
                   Light    = ::QFont::Light,
                   Normal   = ::QFont::Normal,
                   DemiBold = ::QFont::DemiBold,
                   Bold     = ::QFont::Bold,
                   Black    = ::QFont::Black
      };

      enum Style {
                  StyleNormal= ::QFont::StyleNormal,
                  StyleItalic= ::QFont::StyleItalic,
                  StyleOblique= ::QFont::StyleOblique
      };

      enum Stretch {
                    UltraCondensed = ::QFont::UltraCondensed,
                    ExtraCondensed = ::QFont::ExtraCondensed,
                    Condensed      = ::QFont::Condensed,
                    SemiCondensed  = ::QFont::SemiCondensed,
                    Unstretched    = ::QFont::Unstretched,
                    SemiExpanded   = ::QFont::SemiExpanded,
                    Expanded       = ::QFont::Expanded,
                    ExtraExpanded  = ::QFont::ExtraExpanded,
                    UltraExpanded  = ::QFont::UltraExpanded
      };

      enum Capitalization {
                           MixedCase  = ::QFont::MixedCase,
                           AllUppercase  = ::QFont::AllUppercase,
                           AllLowercase  = ::QFont::AllLowercase,
                           SmallCaps  = ::QFont::SmallCaps,
                           Capitalize  = ::QFont::Capitalize
      };

      enum SpacingType {
                        PercentageSpacing  = ::QFont::PercentageSpacing,
                        AbsoluteSpacing  = ::QFont::AbsoluteSpacing
      };

      enum ResolveProperties {
                              FamilyResolved              = ::QFont::FamilyResolved,
                              SizeResolved                = ::QFont::SizeResolved,
                              StyleHintResolved           = ::QFont::StyleHintResolved,
                              StyleStrategyResolved       = ::QFont::StyleStrategyResolved,
                              WeightResolved              = ::QFont::WeightResolved,
                              StyleResolved               = ::QFont::StyleResolved,
                              UnderlineResolved           = ::QFont::UnderlineResolved,
                              OverlineResolved            = ::QFont::OverlineResolved,
                              StrikeOutResolved           = ::QFont::StrikeOutResolved,
                              FixedPitchResolved          = ::QFont::FixedPitchResolved,
                              StretchResolved             = ::QFont::StretchResolved,
                              KerningResolved             = ::QFont::KerningResolved,
                              CapitalizationResolved      = ::QFont::CapitalizationResolved,
                              LetterSpacingResolved       = ::QFont::LetterSpacingResolved,
                              WordSpacingResolved         = ::QFont::WordSpacingResolved,
                              HintingPreferenceResolved   = ::QFont::HintingPreferenceResolved,
                              StyleNameResolved           = ::QFont::StyleNameResolved,
                              AllPropertiesResolved       = ::QFont::AllPropertiesResolved
      };

      QFont() {}
      QFont(const QString &family, int pointSize, Weight weight, bool italic):
        ::QFont(family,pointSize,weight,italic) {}

      QFont(const QString &family, int pointSize, Weight weight):
        QFont(family,pointSize,weight,false) {}
      QFont(const QString &family, int pointSize): ::QFont(family,pointSize,-1) {}
      QFont(const QString &family): QFont(family,-1) {}

      QFont(const ::QFont& x): ::QFont(x) {}
      
      //      QFont(const QFont &, QPaintDevice *pd);

      QString family() const {return ::QFont::family();}
      void setFamily(const QString& f) {::QFont::setFamily(f);}

      QString styleName() const {return ::QFont::styleName();}
      void setStyleName(const QString& s) {::QFont::setStyleName(s);}

      int pointSize() const {return ::QFont::pointSize();}
      void setPointSize(int p) {::QFont::setPointSize(p);}
      qreal pointSizeF() const {return ::QFont::pointSizeF();}
      void setPointSizeF(qreal p) {::QFont::setPointSizeF(p);}

      int pixelSize() const {return ::QFont::pixelSize();}
      void setPixelSize(int p) {::QFont::setPixelSize(p);}

      int weight() const {return ::QFont::weight();}
      void setWeight(int w) {::QFont::setWeight(w);}

      bool bold() const {return ::QFont::bold();}
      void setBold(bool b) {::QFont::setBold(b);}

      void setStyle(Style style) {::QFont::setStyle(::QFont::Style(style));}
      Style style() const {return Style(::QFont::style());}

      bool italic() const {return ::QFont::italic();}
      void setItalic(bool b) {::QFont::setItalic(b);}

      bool underline() const {return ::QFont::underline();}
      void setUnderline(bool x) {::QFont::setUnderline(x);}

      bool overline() const {return ::QFont::overline();}
      void setOverline(bool x) {::QFont::setOverline(x);}

      bool strikeOut() const {return ::QFont::strikeOut();}
      void setStrikeOut(bool x) {::QFont::setStrikeOut(x);}

      bool fixedPitch() const {return ::QFont::fixedPitch();}
      void setFixedPitch(bool x) {::QFont::setFixedPitch(x);}

      bool kerning() const {return ::QFont::kerning();}
      void setKerning(bool x) {::QFont::setKerning(x);}

      StyleHint styleHint() const {return StyleHint(::QFont::styleHint());}
      StyleStrategy styleStrategy() const {return StyleStrategy(::QFont::styleStrategy());}
      void setStyleHint(StyleHint sh, StyleStrategy ss) {
        ::QFont::setStyleHint(::QFont::StyleHint(sh),::QFont::StyleStrategy(ss));}
      void setStyleHint(StyleHint sh) {setStyleHint(sh,PreferDefault);}
      void setStyleStrategy(StyleStrategy s) {::QFont::setStyleStrategy(::QFont::StyleStrategy(s));}

      int stretch() const {return ::QFont::stretch();}
      void setStretch(int s) {::QFont::setStretch(s);}

      qreal letterSpacing() const {return ::QFont::letterSpacing();}
      SpacingType letterSpacingType() const
      {return SpacingType(::QFont::letterSpacingType());}
      void setLetterSpacing(SpacingType type, qreal spacing)
      {::QFont::setLetterSpacing(::QFont::SpacingType(type),spacing);}

      qreal wordSpacing() const {return ::QFont::wordSpacing();}
      void setWordSpacing(qreal spacing) {::QFont::setWordSpacing(spacing);}

      void setCapitalization(Capitalization c)
      {::QFont::setCapitalization(::QFont::Capitalization(c));}
      Capitalization capitalization() const
      {return Capitalization(::QFont::capitalization());}

      void setHintingPreference(HintingPreference hintingPreference)
      {::QFont::setHintingPreference(::QFont::HintingPreference(hintingPreference));}
      HintingPreference hintingPreference() const
      {return HintingPreference(::QFont::hintingPreference());}

      // is raw mode still needed?
      bool rawMode() const{return ::QFont::rawMode();}
      void setRawMode(bool x) {::QFont::setRawMode(x);}

      // dupicated from QFontInfo
      bool exactMatch() const {return ::QFont::exactMatch();}

      bool isCopyOf(const QtGui::QFont& f) const {return ::QFont::isCopyOf(f);}
  };
};





#endif
