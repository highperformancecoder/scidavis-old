#ifndef QT_SCIDAVIS_H
#define QT_SCIDAVIS_H
#include <qcolor.h>
#include <qprinter.h>
#include <QDateTime>
#include <QtEnums.h>

struct QtNamespace
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
};

// shims around Qt classes to allow automatic exposure to python
struct QtCore
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
    std::string toStringStrf(const std::string& format) const
    {return ::QDate::toString(format.c_str()).toStdString();}

    bool setDate(int year, int month, int day)
    {return ::QDate::setDate(year,month,day);}

    void getDate(int& year, int& month, int& day)
    {::QDate::getDate(&year,&month,&day);}
    
    QDate addDays(int days) const {return ::QDate::addDays(days);}
    QDate addMonths(int months) const {return ::QDate::addMonths(months);}
    QDate addYears(int years) const {return ::QDate::addYears(years);}
    int daysTo(const QDate& x) const {return ::QDate::daysTo(x);}

    static QDate currentDate() {return ::QDate::currentDate();}
    static QDate fromString(const std::string& s) 
    {return ::QDate::fromString(s.c_str());}
    static QDate fromString(const std::string& s, QtDateFormat f) 
    {return ::QDate::fromString(s.c_str(), f);}
    static QDate fromString(const std::string& s, const std::string& format)
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
    std::string toStringStrf(const std::string& f) const
    {return ::QTime::toString(f.c_str()).toStdString();}
    bool setHMS(int h, int m, int s){return ::QTime::setHMS(h,m,s);}
    bool setHMS(int h, int m, int s, int ms){return ::QTime::setHMS(h,m,s,ms);}

    QTime addSecs(int secs) const {return ::QTime::addSecs(secs);}
    int secsTo(const QTime& t) const {return ::QTime::secsTo(t);}
    QTime addMSecs(int ms) const {return ::QTime::addMSecs(ms);}
    int msecsTo(const QTime& t) const {return ::QTime::msecsTo(t);}

    static QTime currentTime() {return ::QTime::currentTime();}
    static QTime fromString(const std::string &s) {return ::QTime::fromString(s.c_str());}
    static QTime fromString(const std::string &s, QtDateFormat f)
    {return ::QTime::fromString(s.c_str(),f);}
    static QTime fromString(const std::string &s, const std::string &f) {
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

    QDate date() const {return ::QDateTime::date();}
    QTime time() const {return ::QDateTime::time();}
    QtTimeSpec timeSpec() const {return ::QDateTime::timeSpec();}
    qint64 toMSecsSinceEpoch() const {return ::QDateTime::toMSecsSinceEpoch();}
    uint toTime_t() const {return ::QDateTime::toTime_t();}
    void setDate(const QDate &date) {::QDateTime::setDate(date);}
    void setTime(const QTime &time) {::QDateTime::setTime(time);}
    void setTimeSpec(QtTimeSpec spec) {::QDateTime::setTimeSpec(spec);}
    void setMSecsSinceEpoch(qint64 msecs) {::QDateTime::setMSecsSinceEpoch(msecs);}
    void setTime_t(uint secsSince1Jan1970UTC) {::QDateTime::setTime_t(secsSince1Jan1970UTC);}
    std::string toString() const{return ::QDateTime::toString().toStdString();}
    std::string toString(QtEnums::DateFormat f) const
    {return ::QDateTime::toString(Qt::DateFormat(int(f))).toStdString();}
    std::string toStringStrf(const std::string &format) const
    {return ::QDateTime::toString(format.c_str()).toStdString();}
    QDateTime addDays(int days) const {return ::QDateTime::addDays(days);}
    QDateTime addMonths(int months) const {return ::QDateTime::addMonths(months);}
    QDateTime addYears(int years) const {return ::QDateTime::addYears(years);}
    QDateTime addSecs(int secs) const {return ::QDateTime::addSecs(secs);}
    QDateTime addMSecs(qint64 msecs) const {return ::QDateTime::addMSecs(msecs);}
    QDateTime toTimeSpec(QtTimeSpec spec) const {return ::QDateTime::toTimeSpec(spec);}
    QDateTime toLocalTime() const {return ::QDateTime::toLocalTime();}
    QDateTime toUTC() const {return ::QDateTime::toUTC();}
    int daysTo(const QDateTime& x) const {return ::QDateTime::daysTo(x);}
    int secsTo(const QDateTime& x) const {return ::QDateTime::secsTo(x);}
    qint64 msecsTo(const QDateTime& x) const {return ::QDateTime::msecsTo(x);}

    void setUtcOffset(int seconds) {::QDateTime::setUtcOffset(seconds);}
    int utcOffset() const {return ::QDateTime::utcOffset();}

    static QDateTime currentDateTime() {return ::QDateTime::currentDateTime();}
    static QDateTime currentDateTimeUtc() {return ::QDateTime::currentDateTimeUtc();}
    static QDateTime fromString(const std::string& s) {return ::QDateTime::fromString(s.c_str());}
    static QDateTime fromString(const std::string& s, QtDateFormat f)
    {return ::QDateTime::fromString(s.c_str(),f);}
    static QDateTime fromString(const std::string& s, const std::string& f)
    {return ::QDateTime::fromString(s.c_str(),f.c_str());}
    static QDateTime fromTime_t(uint secsSince1Jan1970UTC)
    {return ::QDateTime::fromTime_t(secsSince1Jan1970UTC);}
    static QDateTime fromMSecsSinceEpoch(qint64 msecs)
    {return ::QDateTime::fromMSecsSinceEpoch(msecs);}
    static qint64 currentMSecsSinceEpoch() {return ::QDateTime::currentMSecsSinceEpoch();}
  };
};


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




#endif
