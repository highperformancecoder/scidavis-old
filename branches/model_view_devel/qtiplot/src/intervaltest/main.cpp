#include <QApplication>
#include <QMainWindow>
#include <QtDebug>
#include <QTime>
#include <QDate>
#include <QList>
#include <QString>
#include <QTableView>

#include "DoubleColumnData.h"
#include "StringColumnData.h"
#include "DateTimeColumnData.h"
#include "ReadOnlyTableModel.h"

#include "IntervalAttribute.h"
#include "AbstractFilter.h"

void dumpIntervalList(const QString& expected, const QList< Interval<int> >& list)
{
	qDebug() << "===============";
	qDebug() << "expected: " << expected;
	QList< Interval<int> > temp_list = list;
	for(int i=0;i<temp_list.size();i++)
		qDebug() << temp_list.at(i).toString();
}

int main(int argc, char **argv)
{
	Interval<int> temp;
	DoubleColumnData ba(100);
	DoubleColumnData sa(100);

	DoubleColumnData col1(100);
	qDebug() << "expected: false: " << col1.isValid(10);
	qDebug() << "expected: false: " << col1.isValid(Interval<int>(10,10));
	col1.setValid(Interval<int>(10,19), true);
	qDebug() << "expected: true: " << col1.isValid(10);

	dumpIntervalList("[4,4] [5,6]", Interval<int>::split(Interval<int>(4,6),5));
	dumpIntervalList("[4,6]", Interval<int>::split(Interval<int>(4,6),4));
	dumpIntervalList("[4,6]", Interval<int>::split(Interval<int>(4,6),2));
	dumpIntervalList("[4,6]", Interval<int>::split(Interval<int>(4,6),7));

	
	qDebug() << "expected: true: " << Interval<int>(10,19).touches(Interval<int>(0,9));
	qDebug() << "expected: true: " << Interval<int>(9,19).intersects(Interval<int>(0,9));

	qDebug() << "expected: 3x [0,19]: ";
	temp = Interval<int>::merge(Interval<int>(0,9), Interval<int>(10,19));
	qDebug() << temp.toString();
	qDebug() << Interval<int>::merge(Interval<int>(0,10), Interval<int>(10,19)).toString();
	qDebug() << Interval<int>(Interval<int>::merge(Interval<int>(0,9), Interval<int>(10,19))).toString();

	temp = Interval<int>::merge(Interval<int>(0,10), Interval<int>(10,19));
	qDebug() << "expected: [0,19]: ";
	qDebug() << temp.toString();
	temp = Interval<int>::merge(Interval<int>(0,7), Interval<int>(10,19));
	qDebug() << "expected: [0,7]: ";
	qDebug() << temp.toString();

	DoubleColumnData col2(100);
	col2.setSelected(Interval<int>(1, 20));
	col2.setFormula(Interval<int>(10, 20),"foo bar");
	col2.setFormula(Interval<int>(1, 9),"bar foo");
	dumpIntervalList("[1,20]", col2.selectedIntervals());

	col2.removeRows(10,5);
	dumpIntervalList("[1,15]", col2.selectedIntervals());

	col2.setSelected(Interval<int>(20, 30));
	dumpIntervalList("[1,15] [20,30]", col2.selectedIntervals());

	col2.removeRows(15,5);
	dumpIntervalList("[1,25]", col2.selectedIntervals());

	col2.insertEmptyRows(5,5);
	dumpIntervalList("[1,4] [10,30]", col2.selectedIntervals());

	qDebug() << "----start of formulas---";
	for(int i=1;i<=30;i++)
		qDebug() << col2.formula(i);
	qDebug() << "----end of formulas------";

	sa.setFormula(Interval<int>(0,10),"foo");
	qDebug() << "expected: foo: " << sa.formula(5);
	ba.setValid(Interval<int>(10,20));
	qDebug() << "expected: true: " << ba.isValid(Interval<int>(12,13));
	sa.setFormula(Interval<int>(5,5),"bar");
	qDebug() << "expected: bar: " << sa.formula(5);
	ba.setValid(Interval<int>(12,13), false);
	qDebug() << "expected: false: " << ba.isValid(Interval<int>(12,13));

	qDebug() << "expected: hello :";
	col1.setFormula(Interval<int>(0,10), "hello");
	qDebug() << col1.formula(7);

	return 0;
}
