#include <iostream>
#include <iomanip>
using namespace std;

#include "DoubleColumnData.h"
#include "StatisticsFilter.h"
#include "DifferentiationFilter.h"
#include "DoubleTransposeFilter.h"

//#include "TableModel.h"
//#include "ReadOnlyTableModel.h"
#include <QApplication>
#include <QMainWindow>
#include <QTableView>
#include <QVBoxLayout>

void dumpDataSource(const AbstractDataSource *source)
{
	if (!source) {
		cout << "(empty data source)\n";
		return;
	}
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

int main(int argc, char **argv)
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
	dumpFilterOutput("Row statistics", &row_statistics);

/*
	QApplication a(argc,argv);
	QMainWindow mw;
	mw.resize(800,600);
	mw.setCentralWidget(new QWidget());
	QVBoxLayout layout(mw.centralWidget());

	QTableView edit;
	edit.setModel(new TableModel(&mw));
	layout.addWidget(&edit);

	ReadOnlyTableModel table;
	if (!table.input(&stat_filter))
		qDebug("Connection stat_filter -> table failed.");
	QTableView view;
	view.setModel(&table);
	layout.addWidget(&view);

	mw.show();
	a.exec();
*/
	return 0;
}
