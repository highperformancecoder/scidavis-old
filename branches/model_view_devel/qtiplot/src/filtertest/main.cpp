#include <iostream>
#include <iomanip>
using namespace std;

#include "DoubleColumnData.h"
#include "StatisticsFilter.h"
#include "DifferentiationFilter.h"

void dumpDataSource(const AbstractDataSource *source)
{
	cout << setw(12) << source->label().toAscii().data() << " || ";
	for(int i=0; i<source->numRows(); i++)
		cout << setw(14) << source->textAt(i).toAscii().data() << " | ";
	cout << "\n";
}
void dumpFilterOutput(const char* header, const AbstractFilter *filter)
{
	cout << "==============================\n";
	cout << " " << header << endl;
	cout << "==============================\n";
	for(int i=0; i<filter->numOutputs(); i++)
		dumpDataSource(filter->output(i));
	cout << endl;
}

int main()
{
	DoubleColumnData dc_one(7);
	for(int i=0;i<dc_one.size();i++)
		dc_one[i] = i;
	dc_one.setLabel("x");
	dc_one.setNumericFormat('d');
	DoubleColumnData dc_two;
	dc_two << 2.3 << 45 << 1.1 << 9 << 12;
	dc_two.setLabel("y");
	dc_two.setNumericFormat('d');

	CopyThroughFilter inputs;
	inputs.input(0, &dc_one);
	inputs.input(1, &dc_two);
	dumpFilterOutput("Input Data", &inputs);

	DifferentiationFilter diff_filter;
	if (!diff_filter.input(0, &dc_one))
		qDebug("Connecting diff_filter's input 0 failed.");
	if (!diff_filter.input(1, &dc_two))
		qDebug("Connecting diff_filter's input 1 failed.");
	dumpFilterOutput("DifferentiationFilter", &diff_filter);

	StatisticsFilter stat_filter;
	if (!stat_filter.input(0, &dc_one))
		qDebug("Connecting stat_filter's input 0 failed.");
	if (!stat_filter.input(1, &dc_two))
		qDebug("Connecting stat_filter's input 1 failed.");
	stat_filter.input(2, diff_filter.output(0));
	stat_filter.input(3, diff_filter.output(1));
	dumpFilterOutput("StatisticsFilter", &stat_filter);

	return 0;
}
