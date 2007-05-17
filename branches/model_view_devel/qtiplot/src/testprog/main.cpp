#include <QApplication>
#include <QMainWindow>
#include <QtDebug>
#include <QTime>
#include <QDate>
#include <QString>

#include "DoubleColumnData.h"
#include "StringColumnData.h"
#include "DateColumnData.h"
#include "TimeColumnData.h"
#include "TableView.h"
#include "TableDataModel.h"

#define DOUT(col) {for(int i=0;i<(col).size();i++)qDebug() << (col).cellString(i) << " ";qDebug() << "\n";}

int main(int argc, char **argv)
{
	QApplication a(argc,argv);

	// column data classes test code
	/*
	DoubleColumnData dc_const(4);
	for(int i=0;i<dc_const.size();i++)
		dc_const[i] = i;
	DoubleColumnData dc_var;

	QStringList dummy1, dummy2;
	dummy1 << "nice" << "wheather" << "today";
	dummy2 << "1.5" << "100.25" << "1000.1";
	StringColumnData sc_const(dummy2);

	StringColumnData sc_var(dummy1);
	qDebug() << "dummy1 output:\n";
	DOUT(sc_var);

	TimeColumnData tc_const,tc_var;
	tc_const << QTime(1,2,3,4) << QTime() << QTime(2,4,6,8);

	DateColumnData dtc_const, dtc_var;
	dtc_const << QDate(1800,1,2) << QDate() << QDate(2000,2,4);

	qDebug() << "The following 3 cols should be equal:\n";
	DOUT(tc_const);
	sc_var.clone(tc_const);
	DOUT(sc_var);
	tc_var.clone(sc_var);
	DOUT(tc_var);

	qDebug() << "The following 3 cols should be equal:\n";
	DOUT(dtc_const);
	sc_var.clone(dtc_const);
	DOUT(sc_var);
	dtc_var.clone(sc_var);
	DOUT(dtc_var);

	qDebug() << "The following 3 cols should be equal:\n";
	DOUT(sc_const);
	dc_var.clone(sc_const);
	DOUT(dc_var);
	sc_var.clone(dc_var);
	DOUT(sc_var);

	qDebug() << "The following 2 cols should be equal:\n";
	dc_var.clone(dc_const);
	DOUT(dc_const);
	DOUT(dc_var);


	qDebug() << "date to double:\n";
	DOUT(dtc_const);
	dc_var.clone(dtc_const);
	DOUT(dc_var);
	dtc_var.clone(dc_var);
	DOUT(dtc_var);

	qDebug() << "time to double:\n";
	DOUT(tc_const);
	dc_var.clone(tc_const);
	DOUT(dc_var);
	tc_var.clone(dc_var);
	DOUT(tc_var);


	qDebug() << "huge vector:";
	QTime t;
	t.start();

	DoubleColumnData dc_huge(1000000);
	for(int i=0;i<dc_huge.size();i++)
		dc_huge[i] = i;
	dc_var.clone(dc_huge);
	qDebug("Time elapsed, create and copy: %d ms", t.elapsed());
	t.restart();
	sc_var.clone(dc_huge);
	qDebug("Time elapsed, convert to string: %d ms", t.elapsed());
*/
	// end of column data classes test code

	QMainWindow mw;
	mw.resize(800,600);

	TableView tw(&mw, new TableDataModel(&mw));
	tw.resize(600,400);
	mw.show();

	a.exec();

	return 0;
}
