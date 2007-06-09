#include <cppunit/extensions/HelperMacros.h>
#include "DoubleColumnData.h"
#include "StringColumnData.h"
#include "DateTimeColumnData.h"
#include "Interval.h"

class ColumnDataTest : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE(ColumnDataTest);
		CPPUNIT_TEST(testDouble);
		CPPUNIT_TEST(testString);
		CPPUNIT_TEST(testIsValid);
		CPPUNIT_TEST(testIntervals);
		CPPUNIT_TEST_SUITE_END();
	public:
		void setUp() {
			dc1 = new DoubleColumnData(100);
			dc2 = new DoubleColumnData(100);
		}
		void tearDown() {
			delete dc1;
			delete dc2;
		}
	private:
		DoubleColumnData *dc1, *dc2;
		void testDouble() {
			DoubleColumnData dc_const(4);
			for(int i=0;i<dc_const.size();i++)
				dc_const[i] = i;
			DoubleColumnData dc_var;

			dc_var.copy(&dc_const);
			CPPUNIT_ASSERT(dc_const.numRows() == dc_var.numRows());
			for (int i=0; i<dc_var.size();i++)
				CPPUNIT_ASSERT(dc_var.valueAt(i) == dc_const.valueAt(i));
		}
		void testString() {
			QStringList dummy1, dummy2;
			dummy1 << "nice" << "wheather" << "today";
			dummy2 << "1.5" << "100.25" << "1000.1";
			StringColumnData sc_const(dummy2);
			StringColumnData sc_var(dummy1);

			CPPUNIT_ASSERT(sc_var.numRows() == dummy1.size());
			for (int i=0; i<sc_var.numRows(); i++)
				CPPUNIT_ASSERT(sc_var.textAt(i) == dummy1[i]);
			//TODO: change textAt to valueAt after removing textAt in AbstractDataSource.h and moving valueAt to *DataSource
		}
		void testIsValid() {
			CPPUNIT_ASSERT(dc1->isValid(10) == false);
			CPPUNIT_ASSERT(dc1->isValid(Interval<int>(10,10)) == false);
			dc1->setValid(Interval<int>(10,19), true);
			CPPUNIT_ASSERT(dc1->isValid(10) == true);
			dc1->removeRows(10,5);
			dc1->insertEmptyRows(5,5);
			dc1->setValid(Interval<int>(10,20));
			CPPUNIT_ASSERT(dc1->isValid(Interval<int>(12,13)) == true);
			dc1->setValid(Interval<int>(12,13), false);
			CPPUNIT_ASSERT(dc1->isValid(Interval<int>(12,13)) == false);
		}
		void testIntervals() {
			dc2->setSelected(Interval<int>(1,20));
			dc2->setFormula(Interval<int>(10,20), "foo bar");
			dc2->setFormula(Interval<int>(1,9), "bar baz");
			CPPUNIT_ASSERT(dc2->selectedIntervals() == QList< Interval<int> >() << Interval<int>(1,20));
			dc2->removeRows(10,5);
			CPPUNIT_ASSERT(dc2->selectedIntervals() == QList< Interval<int> >() << Interval<int>(1,15));
			dc2->setSelected(Interval<int>(20,30));
			CPPUNIT_ASSERT(dc2->selectedIntervals().size() == 2);
			CPPUNIT_ASSERT(dc2->selectedIntervals().contains(Interval<int>(1,15)));
			CPPUNIT_ASSERT(dc2->selectedIntervals().contains(Interval<int>(20,30)));
			dc2->removeRows(15,5);
			CPPUNIT_ASSERT(dc2->selectedIntervals() == QList< Interval<int> >() << Interval<int>(1,25));
			dc2->insertEmptyRows(5,5);
			CPPUNIT_ASSERT(dc2->selectedIntervals().size() == 2);
			CPPUNIT_ASSERT(dc2->selectedIntervals().contains(Interval<int>(1,4)));
			CPPUNIT_ASSERT(dc2->selectedIntervals().contains(Interval<int>(10,30)));
			CPPUNIT_ASSERT(dc2->formula(1) == "bar baz");
			CPPUNIT_ASSERT(dc2->formula(4) == "bar baz");
			CPPUNIT_ASSERT(dc2->formula(5) == "");
			CPPUNIT_ASSERT(dc2->formula(10) == "bar baz");
			CPPUNIT_ASSERT(dc2->formula(14) == "bar baz");
			CPPUNIT_ASSERT(dc2->formula(15) == "foo bar");
			CPPUNIT_ASSERT(dc2->formula(17) == "foo bar");
			CPPUNIT_ASSERT(dc2->formula(19) == "foo bar");
			CPPUNIT_ASSERT(dc2->formula(20) == "");
			CPPUNIT_ASSERT(dc2->formula(30) == "");
		}
};

