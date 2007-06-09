#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/CompilerOutputter.h>
#include "IntervalTest.h"
#include "ColumnDataTest.h"

int main()
{
	CppUnit::TestResult result;
	CppUnit::TestResultCollector collector;
	result.addListener(&collector);
	CppUnit::TextUi::TestRunner runner;

	runner.addTest(IntervalTest::suite());
	runner.addTest(ColumnDataTest::suite());

	runner.run(result);
	CppUnit::CompilerOutputter out(&collector, CppUnit::stdCOut());
	out.write();
	return collector.wasSuccessful() ? 0 : 1;
}

