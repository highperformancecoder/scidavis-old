#include <cppunit/extensions/HelperMacros.h>
#include "DoubleColumnData.h"
#include "StringColumnData.h"
#include "DateTimeColumnData.h"

#include "String2DoubleFilter.h"
#include "Double2StringFilter.h"
#include "DateTime2DoubleFilter.h"
#include "DateTime2StringFilter.h"
#include "Double2DateTimeFilter.h"
#include "String2DateTimeFilter.h"

class ConversionFilterTest : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE(ConversionFilterTest);
		CPPUNIT_TEST(testString2Double);
		CPPUNIT_TEST(testDouble2String);
		CPPUNIT_TEST(testDateTime2Double);
		CPPUNIT_TEST(testDateTime2String);
		CPPUNIT_TEST(testDouble2DateTime);
		CPPUNIT_TEST(testString2DateTime);
		CPPUNIT_TEST(testIdentities);
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
				<< QDateTime(QDate(2007,6,5), QTime(12,28));
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
			CPPUNIT_ASSERT(ds->valueAt(3) == 12);
			CPPUNIT_ASSERT(ds->valueAt(0) == 4.5);
			CPPUNIT_ASSERT(ds->valueAt(1) == 0);
			QLocale::setDefault(QLocale("de_DE"));
			CPPUNIT_ASSERT(ds->valueAt(0) == 0);
			CPPUNIT_ASSERT(ds->valueAt(1) == 2.3);
		}
		void testDouble2String() {
			QLocale::setDefault(QLocale::c());
			Double2StringFilter f;
			CPPUNIT_ASSERT(f.input(0, dc) == true);
			AbstractStringDataSource *ds = qobject_cast<AbstractStringDataSource*>(f.output(0));
			CPPUNIT_ASSERT(ds != 0);
			CPPUNIT_ASSERT(ds->textAt(0) == "3.400000e+00");
			f.setNumericFormat('d');
			CPPUNIT_ASSERT(ds->textAt(1) == "18.000000");
			f.setNumDigits(2);
			CPPUNIT_ASSERT(ds->textAt(2) == "21.00");
		}
		void testDateTime2Double() {
			DateTime2DoubleFilter f;
			CPPUNIT_ASSERT(f.input(0, dtc) == true);
			AbstractDoubleDataSource *ds = qobject_cast<AbstractDoubleDataSource*>(f.output(0));
			CPPUNIT_ASSERT(ds != 0);
			CPPUNIT_ASSERT(ds->valueAt(0) == 320.5 + (6.0+3.0/60.0)/24.0);
			CPPUNIT_ASSERT(int(ds->valueAt(2)) == 2454257);
		}
		void testDateTime2String() {
			DateTime2StringFilter f;
			CPPUNIT_ASSERT(f.input(0, dtc) == true);
			AbstractStringDataSource *ds = qobject_cast<AbstractStringDataSource*>(f.output(0));
			CPPUNIT_ASSERT(ds != 0);
			CPPUNIT_ASSERT(ds->textAt(1) == "1890-12-30 10:23:00.000");
		}
		void testDouble2DateTime() {
			Double2DateTimeFilter f;
			CPPUNIT_ASSERT(f.input(0, dc) == true);
			AbstractDateTimeDataSource *ds = qobject_cast<AbstractDateTimeDataSource*>(f.output(0));
			CPPUNIT_ASSERT(ds != 0);
			CPPUNIT_ASSERT(ds->dateAt(0) == QDate::fromJulianDay(3));
			CPPUNIT_ASSERT(ds->timeAt(0) == QTime(21,36));
			CPPUNIT_ASSERT(ds->dateTimeAt(1) == QDateTime(QDate::fromJulianDay(18), QTime(0,0)));
		}
		void testString2DateTime() {
			String2DateTimeFilter f;
			CPPUNIT_ASSERT(f.input(0, sc) == true);
			AbstractDateTimeDataSource *ds = qobject_cast<AbstractDateTimeDataSource*>(f.output(0));
			CPPUNIT_ASSERT(ds != 0);
			CPPUNIT_ASSERT(ds->dateAt(2) == QDate(2007,1,15));
			CPPUNIT_ASSERT(ds->timeAt(2) == QTime(0,0));
			CPPUNIT_ASSERT(ds->dateTimeAt(4) == QDateTime(QDate(1972,3,10), QTime(12,14,31,4)));
			CPPUNIT_ASSERT(ds->dateAt(1) == QDate(1900,1,1));
		}
		void testIdentities() {
			QLocale::setDefault(QLocale::c());

			Double2StringFilter f1;
			String2DoubleFilter f2;
			CPPUNIT_ASSERT(f1.input(0, dc) == true);
			CPPUNIT_ASSERT(f2.input(&f1) == true);
			AbstractDoubleDataSource *ds = qobject_cast<AbstractDoubleDataSource*>(f2.output(0));
			CPPUNIT_ASSERT(ds != 0);
			for (int i=0; i<ds->numRows(); i++)
				CPPUNIT_ASSERT(ds->valueAt(i) == dc->valueAt(i));

			DateTime2StringFilter f3;
			String2DateTimeFilter f4;
			CPPUNIT_ASSERT(f3.input(0, dtc) == true);
			CPPUNIT_ASSERT(f4.input(&f3) == true);
			AbstractDateTimeDataSource *ds2 = qobject_cast<AbstractDateTimeDataSource*>(f4.output(0));
			CPPUNIT_ASSERT(ds2 != 0);
			for (int i=0; i<ds2->numRows(); i++)
				CPPUNIT_ASSERT(ds2->dateTimeAt(i) == dtc->dateTimeAt(i));

			DateTime2DoubleFilter f5;
			Double2DateTimeFilter f6;
			CPPUNIT_ASSERT(f5.input(0, dtc) == true);
			CPPUNIT_ASSERT(f6.input(&f5) == true);
			AbstractDateTimeDataSource *ds3 = qobject_cast<AbstractDateTimeDataSource*>(f6.output(0));
			CPPUNIT_ASSERT(ds3 != 0);
			for (int i=0; i<ds3->numRows(); i++)
				CPPUNIT_ASSERT(ds3->dateTimeAt(i) == dtc->dateTimeAt(i));
		}
};

