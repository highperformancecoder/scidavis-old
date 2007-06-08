#include <iostream>
#include <iomanip>
using namespace std;

#include "DoubleColumnData.h"
#include "StatisticsFilter.h"
#include "DifferentiationFilter.h"
#include "DoubleTransposeFilter.h"
#include "Double2StringFilter.h"

//#include "TableModel.h"
#include "ReadOnlyTableModel.h"
#include <QApplication>
#include <QMainWindow>
#include <QTableView>
#include <QVBoxLayout>
#include <QTime>

Double2StringFilter double2string;

void dumpDataSource(AbstractDataSource *source)
{
	if (!source) {
		cout << "(empty data source)\n";
		return;
	}
	cout << setw(12) << source->label().toAscii().data() << " || ";
	if (source->inherits("AbstractDoubleDataSource")) {
		double2string.input(0, source);
		for(int i=0; i<source->numRows(); i++)
			cout << setw(8) << double2string.textAt(i).toAscii().data() << " | ";
	} else
		for(int i=0; i<source->numRows(); i++)
			cout << setw(8) << source->textAt(i).toAscii().data() << " | ";
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

int main(int argc, char **argv)
{
	QApplication a(argc,argv);
	double2string.setNumericFormat('d');
	double2string.setNumDigits(4);

	DoubleColumnData dc_one(7);
	for(int i=0;i<dc_one.size();i++)
		dc_one[i] = i;
	dc_one.setLabel("x");
	DoubleColumnData dc_two;
	dc_two << 2.3 << 45 << 1.1 << 9 << 12;
	dc_two.setLabel("y");

	CopyThroughFilter inputs;
	inputs.input(0, &dc_one);
	inputs.input(1, &dc_two);
	dumpFilterOutput("Input Data", &inputs);

	DifferentiationFilter diff_filter;
	if (!diff_filter.input(&inputs))
		qDebug("Connection inputs->diff_filter failed.");
	dumpFilterOutput("DifferentiationFilter", &diff_filter);

	StatisticsFilter stat_filter;
	if (!stat_filter.input(&inputs))
		qDebug("Connection inputs->stat_filter failed.");
	if (!stat_filter.input(2, diff_filter.output(0)))
		qDebug("Connection diff_filter[0] -> stat_filter failed.");
	if (!stat_filter.input(3, diff_filter.output(1)))
		qDebug("Connection diff_filter[1] -> stat_filter failed.");
	dumpFilterOutput("StatisticsFilter", &stat_filter);

	DoubleTransposeFilter transpose_filter(false);
	StatisticsFilter row_statistics;
	transpose_filter.input(&inputs);
	row_statistics.input(&transpose_filter);
	dumpFilterOutput("Transposed Data", &transpose_filter);
	dumpFilterOutput("Row Statistics", &row_statistics);

	QTime t;
	cout << "Computing statistics on a huge random data set... " << flush;
	t.start();
	DoubleColumnData dc_random(1000000);
	cout << "allocated (took " << t.elapsed() << "ms)... " << flush;
	t.restart();
	for (int i=0; i<dc_random.numRows(); i++)
		dc_random[i] = rand();
	cout << "filled (took " << t.elapsed() << "ms)... " << endl;
	StatisticsFilter huge_stat;
	t.restart();
	huge_stat.input(0, &dc_random);
	cout << "statistics computation took " << t.elapsed() << "ms." << endl;
	cout << endl;
	dumpFilterOutput("Huge Statistics Test", &huge_stat);


	QMainWindow mw;
	mw.resize(800,600);
	mw.setCentralWidget(new QWidget());
	QVBoxLayout layout(mw.centralWidget());
/*
	TableModel edit;
	QTableView editView;
	editView.setModel(&edit);
	layout.addWidget(&editView);
*/
	ReadOnlyTableModel table;
	if (!table.input(&stat_filter))
		qDebug("Connection stat_filter -> table failed.");
	QTableView view;
	view.setModel(&table);
	layout.addWidget(&view);

	mw.show();
	a.exec();

	return 0;
}
