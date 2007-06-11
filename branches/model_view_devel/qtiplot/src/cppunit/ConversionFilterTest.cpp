#include <cppunit/extensions/HelperMacros.h>
#include "assertion_traits.h"

#include "DoubleColumnData.h"
#include "StringColumnData.h"
#include "DateTimeColumnData.h"

#include "String2DoubleFilter.h"
#include "Double2StringFilter.h"
#include "DateTime2DoubleFilter.h"
#include "DateTime2StringFilter.h"
#include "Double2DateTimeFilter.h"
#include "String2DateTimeFilter.h"
#include "Double2MonthFilter.h"
#include "Double2DayOfWeekFilter.h"
#include "Month2DoubleFilter.h"
#include "DayOfWeek2DoubleFilter.h"

class ConversionFilterTest : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE(ConversionFilterTest);
		CPPUNIT_TEST(testString2Double);
		CPPUNIT_TEST(testDouble2String);
		CPPUNIT_TEST(testDateTime2Double);
		CPPUNIT_TEST(testDateTime2String);
		CPPUNIT_TEST(testDouble2DateTime);
		CPPUNIT_TEST(testString2DateTime);
		CPPUNIT_TEST(testDoubleStringDouble);
		CPPUNIT_TEST(testDTStringDT);
		CPPUNIT_TEST(testDTDoubleDT);
		/*
		CPPUNIT_TEST(testDoubleMonthDouble);
		CPPUNIT_TEST(testMonthDoubleMonth);
		CPPUNIT_TEST(testDoubleDayDouble);
		CPPUNIT_TEST(testDayDoubleDay);
		*/
		CPPUNIT_TEST_SUITE_END();

	public:
		void setUp() {
			dc = new DoubleColumnData();
			*dc << 3.4 << 18 << 21;
			dc->setValid(Interval<int>(0,2));
			sc = new StringColumnData();
			*sc << "4.5" << "2,3" << "2007-01-15" << "12" << "10/03/72 12:14:31:004";
			sc->setValid(Interval<int>(0,4));
			dtc = new DateTimeColumnData();
			*dtc << QDateTime(QDate::fromJulianDay(321), QTime(6,3))
				<< QDateTime(QDate(1890,12,30), QTime(10,23))
				<< QDateTime(QDate(2007,6,5), QTime(12,28))
				<< QDateTime(QDate(3,4,5), QTime(6,7))
				<< QDateTime(QDate(12,5,1), QTime(0,0));
			dtc->setValid(Interval<int>(0,2));
		}
		void tearDown() {
			delete dc;
			delete sc;
			delete dtc;
		}

	private:
		DoubleColumnData *dc;
		StringColumnData *sc;
		DateTimeColumnData *dtc;

		void testString2Double() {
			QLocale::setDefault(QLocale::c());
			String2DoubleFilter f;
			CPPUNIT_ASSERT(f.input(0, sc) == true);
			AbstractDoubleDataSource *ds = qobject_cast<AbstractDoubleDataSource*>(f.output(0));
			CPPUNIT_ASSERT(ds != 0);
			CPPUNIT_ASSERT_EQUAL(12.0, ds->valueAt(3));
			CPPUNIT_ASSERT_EQUAL(4.5, ds->valueAt(0));
			CPPUNIT_ASSERT_EQUAL(0.0, ds->valueAt(1));
			QLocale::setDefault(QLocale("de_DE"));
			CPPUNIT_ASSERT_EQUAL(0.0, ds->valueAt(0));
			CPPUNIT_ASSERT_EQUAL(2.3, ds->valueAt(1));
		}
		void testDouble2String() {
			QLocale::setDefault(QLocale::c());
			Double2StringFilter f;
			CPPUNIT_ASSERT(f.input(0, dc) == true);
			AbstractStringDataSource *ds = qobject_cast<AbstractStringDataSource*>(f.output(0));
			CPPUNIT_ASSERT(ds != 0);
			CPPUNIT_ASSERT_EQUAL(QString("3.400000e+00"), ds->textAt(0));
			f.setNumericFormat('d');
			CPPUNIT_ASSERT_EQUAL(QString("18.000000"), ds->textAt(1));
			f.setNumDigits(2);
			CPPUNIT_ASSERT_EQUAL(QString("21.00"), ds->textAt(2));
		}
		void testDateTime2Double() {
			DateTime2DoubleFilter f;
			CPPUNIT_ASSERT(f.input(0, dtc) == true);
			AbstractDoubleDataSource *ds = qobject_cast<AbstractDoubleDataSource*>(f.output(0));
			CPPUNIT_ASSERT(ds != 0);
			CPPUNIT_ASSERT_EQUAL((320.5 + (6.0+3.0/60.0)/24.0), ds->valueAt(0));
			CPPUNIT_ASSERT_EQUAL(2454257, int(ds->valueAt(2)));
		}
		void testDateTime2String() {
			DateTime2StringFilter f;
			CPPUNIT_ASSERT(f.input(0, dtc) == true);
			AbstractStringDataSource *ds = qobject_cast<AbstractStringDataSource*>(f.output(0));
			CPPUNIT_ASSERT(ds != 0);
			CPPUNIT_ASSERT_EQUAL(QString("1890-12-30 10:23:00.000"), ds->textAt(1));
		}
		void testDouble2DateTime() {
			Double2DateTimeFilter f;
			CPPUNIT_ASSERT(f.input(0, dc) == true);
			AbstractDateTimeDataSource *ds = qobject_cast<AbstractDateTimeDataSource*>(f.output(0));
			CPPUNIT_ASSERT(ds != 0);
			CPPUNIT_ASSERT_EQUAL(QDateTime(QDate::fromJulianDay(3), QTime(21,36)), ds->dateTimeAt(0));
			CPPUNIT_ASSERT_EQUAL(QDateTime(QDate::fromJulianDay(18), QTime(12,0)), ds->dateTimeAt(1));
		}
		void testString2DateTime() {
			String2DateTimeFilter f;
			CPPUNIT_ASSERT(f.input(0, sc) == true);
			AbstractDateTimeDataSource *ds = qobject_cast<AbstractDateTimeDataSource*>(f.output(0));
			CPPUNIT_ASSERT(ds != 0);
			CPPUNIT_ASSERT_EQUAL(QDateTime(QDate(2007,1,15), QTime(0,0)), ds->dateTimeAt(2));
			CPPUNIT_ASSERT_EQUAL(QDateTime(QDate(1972,3,10), QTime(12,14,31,4)), ds->dateTimeAt(4));
			CPPUNIT_ASSERT_EQUAL(QDate(1900,1,1), ds->dateAt(1));
		}
		void testDoubleStringDouble() {
			QLocale::setDefault(QLocale::c());
			Double2StringFilter f1;
			String2DoubleFilter f2;
			CPPUNIT_ASSERT(f1.input(0, dc) == true);
			CPPUNIT_ASSERT(f2.input(&f1) == true);
			AbstractDoubleDataSource *ds = qobject_cast<AbstractDoubleDataSource*>(f2.output(0));
			CPPUNIT_ASSERT(ds != 0);
			for (int i=0; i<ds->numRows(); i++)
				CPPUNIT_ASSERT_EQUAL(dc->valueAt(i), ds->valueAt(i));
		}
		void testDTStringDT() {
			QLocale::setDefault(QLocale::c());
			DateTime2StringFilter f1;
			String2DateTimeFilter f2;
			CPPUNIT_ASSERT(f1.input(0, dtc) == true);
			CPPUNIT_ASSERT(f2.input(&f1) == true);
			AbstractDateTimeDataSource *ds = qobject_cast<AbstractDateTimeDataSource*>(f2.output(0));
			CPPUNIT_ASSERT(ds != 0);
			// the following fails due to Qt missing support for negative or non-4-digit years
			// http://trolltech.com/developer/task-tracker/index_html?method=entry&id=157964
			/*
			for (int i=0; i<ds->numRows(); i++)
				CPPUNIT_ASSERT_EQUAL(dtc->dateTimeAt(i), ds->dateTimeAt(i));
			*/
			// these should work, though:
			CPPUNIT_ASSERT_EQUAL(dtc->dateTimeAt(1), ds->dateTimeAt(1));
			CPPUNIT_ASSERT_EQUAL(dtc->dateTimeAt(2), ds->dateTimeAt(2));
		}
		void testDTDoubleDT() {
			DateTime2DoubleFilter f1;
			Double2DateTimeFilter f2;
			CPPUNIT_ASSERT(f1.input(0, dtc) == true);
			CPPUNIT_ASSERT(f2.input(&f1) == true);
			AbstractDateTimeDataSource *ds = qobject_cast<AbstractDateTimeDataSource*>(f2.output(0));
			CPPUNIT_ASSERT(ds != 0);
			for (int i=0; i<ds->numRows(); i++)
				CPPUNIT_ASSERT_EQUAL(dtc->dateTimeAt(i), ds->dateTimeAt(i));
		}
		void testDoubleMonthDouble() {
			Double2MonthFilter f1;
			Month2DoubleFilter f2;
			CPPUNIT_ASSERT(f1.input(0, dc) == true);
			CPPUNIT_ASSERT(f2.input(&f1) == true);
			AbstractDoubleDataSource *ds = qobject_cast<AbstractDoubleDataSource*>(f2.output(0));
			CPPUNIT_ASSERT(ds != 0);
			for (int i=0; i<ds->numRows(); i++)
				CPPUNIT_ASSERT_EQUAL(dc->valueAt(i), ds->valueAt(i));
		}
		void testMonthDoubleMonth() {
			Month2DoubleFilter f1;
			Double2MonthFilter f2;
			CPPUNIT_ASSERT(f1.input(0, dtc) == true);
			CPPUNIT_ASSERT(f2.input(&f1) == true);
			AbstractDateTimeDataSource *ds = qobject_cast<AbstractDateTimeDataSource*>(f2.output(0));
			CPPUNIT_ASSERT(ds != 0);
			for (int i=0; i<ds->numRows(); i++)
				CPPUNIT_ASSERT_EQUAL(dtc->dateTimeAt(i), ds->dateTimeAt(i));
		}
		void testDoubleDayDouble() {
			Double2DayOfWeekFilter f1;
			DayOfWeek2DoubleFilter f2;
			CPPUNIT_ASSERT(f1.input(0, dc) == true);
			CPPUNIT_ASSERT(f2.input(&f1) == true);
			AbstractDoubleDataSource *ds = qobject_cast<AbstractDoubleDataSource*>(f2.output(0));
			CPPUNIT_ASSERT(ds != 0);
			for (int i=0; i<ds->numRows(); i++)
				CPPUNIT_ASSERT_EQUAL(dc->valueAt(i), ds->valueAt(i));
		}
		void testDayDoubleDay() {
			DayOfWeek2DoubleFilter f1;
			Double2DayOfWeekFilter f2;
			CPPUNIT_ASSERT(f1.input(0, dtc) == true);
			CPPUNIT_ASSERT(f2.input(&f1) == true);
			AbstractDateTimeDataSource *ds = qobject_cast<AbstractDateTimeDataSource*>(f2.output(0));
			CPPUNIT_ASSERT(ds != 0);
			for (int i=0; i<ds->numRows(); i++)
				CPPUNIT_ASSERT_EQUAL(dtc->dateTimeAt(i), ds->dateTimeAt(i));
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION( ConversionFilterTest );

