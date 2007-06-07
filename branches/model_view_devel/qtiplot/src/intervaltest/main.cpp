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

#include "DataSourceAttributes.h"
#include "IntervalAttribute.h"
#include "AbstractFilter.h"

#define DOUT(col) {for(int i=0;i<(col).numRows();i++)qDebug() << (col).textAt(i) << " ";qDebug() << "\n";}

int main(int argc, char **argv)
{
	int i;
	Interval temp;
	QList<Interval> temp_list;
	IntervalAttribute<bool> ba;
	IntervalAttribute<QString> sa;

	DataSourceAttributes col1;
	qDebug() << "expected: false: " << col1.isValid(10);
	qDebug() << "expected: false: " << col1.isValid(Interval(10,10));
	col1.setValid(Interval(10,19), true);
	qDebug() << "expected: true: " << col1.isValid(10);

	qDebug() << "===============";
	temp_list = Interval::split(Interval(4,6),5);
	for(i=0;i<temp_list.size();i++)
		temp_list.at(i).print();
	qDebug() << "===============";
	temp_list = Interval::split(Interval(4,6),4);
	for(i=0;i<temp_list.size();i++)
		temp_list.at(i).print();
	qDebug() << "===============";
	temp_list = Interval::split(Interval(4,6),7);
	for(i=0;i<temp_list.size();i++)
		temp_list.at(i).print();
	qDebug() << "===============";
	temp_list = Interval::split(Interval(4,6),2);
	for(i=0;i<temp_list.size();i++)
		temp_list.at(i).print();
	qDebug() << "===============";

	
	qDebug() << "expected: true: " << Interval(10,19).touches(Interval(0,9));
	qDebug() << "expected: true: " << Interval(9,19).intersects(Interval(0,9));

	qDebug() << "expected: 3x [0,19]: ";
	temp = Interval::merge(Interval(0,9), Interval(10,19));
	temp.print();
	Interval::merge(Interval(0,10), Interval(10,19)).print();
	Interval(Interval::merge(Interval(0,9), Interval(10,19))).print();

	temp = Interval::merge(Interval(0,10), Interval(10,19));
	qDebug() << "expected: [0,19]: ";
	temp.print();
	temp = Interval::merge(Interval(0,7), Interval(10,19));
	qDebug() << "expected: [0,7]: ";
	temp.print();

	qDebug() << "----------------";
	qDebug() << "expected: [0,9]: ";
	temp_list = Interval::subtract(Interval(0,9),Interval(10,19));
	for(i=0;i<temp_list.size();i++)
		temp_list.at(i).print();
	qDebug() << "----------------";
	qDebug() << "expected: []: ";
	temp_list = Interval::subtract(Interval(0,9),Interval(0,9));
	for(i=0;i<temp_list.size();i++)
		temp_list.at(i).print();
	qDebug() << "----------------";
	qDebug() << "expected: [0,4]: ";
	temp_list = Interval::subtract(Interval(0,9),Interval(5,9));
	for(i=0;i<temp_list.size();i++)
		temp_list.at(i).print();
	qDebug() << "----------------";
	qDebug() << "expected: [6,9]: ";
	temp_list = Interval::subtract(Interval(0,9),Interval(0,5));
	for(i=0;i<temp_list.size();i++)
		temp_list.at(i).print();
	qDebug() << "----------------";
	qDebug() << "expected: []: ";
	temp_list = Interval::subtract(Interval(0,9),Interval(0,20));
	for(i=0;i<temp_list.size();i++)
		temp_list.at(i).print();
	qDebug() << "----------------";
	qDebug() << "expected: [7,9]: ";
	temp_list = Interval::subtract(Interval(5,9),Interval(0,6));
	for(i=0;i<temp_list.size();i++)
		temp_list.at(i).print();
	qDebug() << "----------------";
	qDebug() << "expected: [0,9] [16,20]: ";
	temp_list = Interval::subtract(Interval(0,20),Interval(10,15));
	for(i=0;i<temp_list.size();i++)
		temp_list.at(i).print();
	qDebug() << "----------------";


	DataSourceAttributes col2;
	col2.setSelected(Interval(1, 20));
	col2.setFormula(Interval(10, 20),"foo bar");
	col2.setFormula(Interval(1, 9),"bar foo");
	col2.deleteRows(0, 10,5);
	col2.setSelected(Interval(20, 30));
	col2.deleteRows(0, 15,5);
	temp_list = col2.selectedIntervals();
	qDebug() << "selected intervals:";
	qDebug() << "expected: [1,25]:";
	for(i=0;i<temp_list.size();i++)
		temp_list.at(i).print();

	col2.insertRows(0, 5,5);
	temp_list = col2.selectedIntervals();
	qDebug() << "selected intervals:";
	qDebug() << "expected: [1,4] [10,30]:";
	for(i=0;i<temp_list.size();i++)
		temp_list.at(i).print();

	qDebug() << "----start of formulas---";
	for(i=1;i<=30;i++)
		qDebug() << col2.formula(i);
	qDebug() << "----end of formulas------";

	sa.setValue(Interval(0,10),"foo");
	qDebug() << "expected: foo: " << sa.value(5);
	ba.setValue(Interval(10,20));
	qDebug() << "expected: true: " << ba.isSet(Interval(12,13));
	sa.setValue(Interval(5,5),"bar");
	qDebug() << "expected: bar: " << sa.value(5);
	ba.setValue(Interval(12,13), false);
	qDebug() << "expected: false: " << ba.isSet(Interval(12,13));

	qDebug() << "expected: hello :";
	col1.setFormula(Interval(0,10), "hello");
	qDebug() << col1.formula(7);

	return 0;
}
