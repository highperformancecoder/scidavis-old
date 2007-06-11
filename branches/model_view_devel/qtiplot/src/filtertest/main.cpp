#include <iostream>
#include <iomanip>
using namespace std;

#include "DoubleColumnData.h"
#include "StringColumnData.h"
#include "DateTimeColumnData.h"

#include "StatisticsFilter.h"
#include "DifferentiationFilter.h"
#include "DoubleTransposeFilter.h"
#include "Double2StringFilter.h"
#include "String2DoubleFilter.h"
#include "CopyThroughFilter.h"
#include "String2DateTimeFilter.h"
#include "DateTime2StringFilter.h"
#include "Double2DateTimeFilter.h"

#include "TableModel.h"
#include "ReadOnlyTableModel.h"

#include <QApplication>
#include <QMainWindow>
#include <QTableView>
#include <QVBoxLayout>
#include <QTime>

Double2StringFilter double2string('d',4);
DateTime2StringFilter date_time2string;

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
			cout << setw(8) << static_cast<AbstractStringDataSource*>(double2string.output(0))->textAt(i).toAscii().data() << " | ";
	} else if (source->inherits("AbstractStringDataSource"))
		for(int i=0; i<source->numRows(); i++)
			cout << setw(8) << static_cast<AbstractStringDataSource*>(source)->textAt(i).toAscii().data() << " | ";
	else if (source->inherits("AbstractDateTimeDataSource")) {
		date_time2string.input(0, source);
		for(int i=0; i<source->numRows(); i++)
			cout << setw(15) << static_cast<AbstractStringDataSource*>(date_time2string.output(0))->textAt(i).toAscii().data() << " | ";
	} else
		cout << "(invalid data type)";
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

	DoubleColumnData *dc_one = new DoubleColumnData(7);
	for(int i=0;i<dc_one->size();i++)
		(*dc_one)[i] = i;
	dc_one->setLabel("x");
	dc_one->setValid(Interval<int>(0,6));
	DoubleColumnData *dc_two = new DoubleColumnData();
	*dc_two << 2.3 << 45 << 1.1 << 9 << 12;
	dc_two->setLabel("y");
	dc_two->setValid(Interval<int>(0,4));

	CopyThroughFilter inputs;
	inputs.input(0, dc_one);
	inputs.input(1, dc_two);
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
		dc_random.replace(i, rand());
	dc_random.setValid(Interval<int>(0,dc_random.numRows()-1));
	cout << "filled (took " << t.elapsed() << "ms)... " << endl;
	StatisticsFilter huge_stat;
	t.restart();
	huge_stat.input(0, &dc_random);
	cout << "statistics computation took " << t.elapsed() << "ms." << endl;
	cout << endl;
	dumpFilterOutput("Huge Statistics Test", &huge_stat);

	StringColumnData *sc = new StringColumnData();
	*sc << "4.5" << "2,3" << "2007-01-15" << "12" << "10/03/72 12:14:31:004" << "2004-01-22 01:02:03.456";
	sc->setLabel("test strings");
	sc->setValid(Interval<int>(0, sc->numRows() - 1));
	String2DateTimeFilter f;
	f.input(0, sc);
	dumpFilterOutput("String -> DateTime", &f);

	DoubleColumnData *dc = new DoubleColumnData();
	*dc << 3.4 << 18 << 21;
	dc->setValid(Interval<int>(0, dc->numRows() - 1));
	Double2DateTimeFilter f2;
	f2.input(0, dc);
	dumpFilterOutput("Double -> DateTime", &f2);

	DateTimeColumnData *dtc = new DateTimeColumnData();
	*dtc << QDateTime(QDate::fromJulianDay(321), QTime(6,3))
		<< QDateTime(QDate(1890,12,30), QTime(10,23))
		<< QDateTime(QDate(2007,6,5), QTime(12,28));
	dtc->setValid(Interval<int>(0,2));
	dtc->setLabel("DateTime data");
	DateTime2StringFilter f3;
	String2DateTimeFilter f4;
	f3.input(0, dtc); f4.input(&f3);
	dumpDataSource(dtc);
	dumpDataSource(f3.output(0));
	dumpDataSource(f4.output(0));

	QMainWindow mw;
	mw.resize(800,600);
	mw.setCentralWidget(new QWidget());
	QVBoxLayout layout(mw.centralWidget());

	TableModel edit;
	QTableView editView;
	editView.setModel(&edit);
	layout.addWidget(&editView);
	QList<AbstractColumnData *> data_list;
	data_list << dc_one << dc_two;
	edit.appendColumns(data_list);
	// TODO: should be handled by TableModel (?)
	edit.setOutputFilter(0, new Double2StringFilter());
	edit.setOutputFilter(1, new Double2StringFilter());
	edit.setInputFilter(0, new String2DoubleFilter());
	edit.setInputFilter(1, new String2DoubleFilter());

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
