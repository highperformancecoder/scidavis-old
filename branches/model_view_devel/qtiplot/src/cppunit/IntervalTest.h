#include <cppunit/extensions/HelperMacros.h>
#include "Interval.h"

class IntervalTest : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE(IntervalTest);
		CPPUNIT_TEST(testEquality);
		CPPUNIT_TEST(testSplit);
		CPPUNIT_TEST(testTouches);
		CPPUNIT_TEST(testIntersects);
		CPPUNIT_TEST(testMerge);
		CPPUNIT_TEST(testCopy);
		CPPUNIT_TEST_SUITE_END();
	public:
		void setUp() {
			int_4_6 = new Interval<int>(4,6);
			int_4_4 = new Interval<int>(4,4);
			int_5_6 = new Interval<int>(5,6);
			int_7_19 = new Interval<int>(7,19);
			int_10_14 = new Interval<int>(10,14);
			int_4_19 = new Interval<int>(4,19);
		}
		void tearDown() {
			delete int_4_6;
			delete int_4_4;
			delete int_5_6;
			delete int_7_19;
			delete int_10_14;
			delete int_4_19;
		}
	private:
		Interval<int> *int_4_6, *int_4_4, *int_5_6, *int_7_19, *int_10_14, *int_4_19;
		void testEquality() {
			CPPUNIT_ASSERT(*int_4_6 == *int_4_6);
			CPPUNIT_ASSERT(!(*int_4_6 == *int_4_4));
		}
		void testSplit() {
			CPPUNIT_ASSERT(Interval<int>::split(*int_4_6, 5) == QList< Interval<int> >() << *int_4_4 << *int_5_6);
			CPPUNIT_ASSERT(Interval<int>::split(*int_4_6, 4) == QList< Interval<int> >() << *int_4_6);
			CPPUNIT_ASSERT(Interval<int>::split(*int_4_6, 7) == QList< Interval<int> >() << *int_4_6);
		}
		void testTouches() {
			CPPUNIT_ASSERT(int_4_6->touches(*int_7_19) == true);
		}
		void testIntersects() {
			CPPUNIT_ASSERT(int_7_19->intersects(*int_10_14) == true);
			CPPUNIT_ASSERT(int_4_6->intersects(*int_4_4) == true);
		}
		void testMerge() {
			CPPUNIT_ASSERT(Interval<int>::merge(*int_4_6, *int_7_19) == *int_4_19);
			CPPUNIT_ASSERT(Interval<int>::merge(*int_4_19, *int_10_14) == *int_4_19);
			CPPUNIT_ASSERT(Interval<int>::merge(*int_4_6, Interval<int>(6,19)) == *int_4_19);
			CPPUNIT_ASSERT(Interval<int>::merge(*int_4_6, *int_10_14) == *int_4_6);
		}
		void testCopy() {
			CPPUNIT_ASSERT(Interval<int>(*int_7_19) == *int_7_19);
			Interval<int> tmp;
			tmp = *int_10_14;
			CPPUNIT_ASSERT(tmp == *int_10_14);
		}
};

