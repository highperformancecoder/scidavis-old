#include <QApplication>
#include <QMainWindow>
#include <QtDebug>
#include <QTime>
#include <QDate>
#include <QString>
#include <QTableView>

#include "DoubleColumnData.h"
#include "StringColumnData.h"
#include "DateTimeColumnData.h"
#include "ReadOnlyTableModel.h"

#include "AbstractFilter.h"
#include "CopyThroughFilter.h"

#define DOUT(col) {for(int i=0;i<(col).numRows();i++)qDebug() << (col).textAt(i) << " ";qDebug() << "\n";}

int main(int argc, char **argv)
{
	int i;
	QApplication a(argc,argv);

	DoubleColumnData col1(3), col2(5);
	for(i=0;i<col1.numRows();i++)
		col1[i] = i;
	for(i=0;i<col2.numRows();i++)
		col2[i] = 10*i;

	CopyThroughFilter filter;
	filter.input(0, &col1);
	filter.input(1, &col2);

	ReadOnlyTableModel model;
	for(i=0;i<filter.numOutputs();i++)
		model.input(i, filter.output(i));

	QMainWindow mw;
	mw.resize(800,600);

	QTableView tw(&mw);
	tw.setModel(&model);
	tw.resize(600,400);
	mw.show();

	a.exec();

	return 0;
}
