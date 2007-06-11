#include <QApplication>
#include <QMainWindow>
#include <QtDebug>
#include <QTime>
#include <QDate>
#include <QString>

#include "TableView.h"
#include "TableModel.h"

#include "StringColumnData.h"
#include "DoubleColumnData.h"
#include "DateTimeColumnData.h"
#include "AbstractFilter.h"
#include "CopyThroughFilter.h"
#include "Double2StringFilter.h"
#include "String2DoubleFilter.h"
#include "DateTime2StringFilter.h"
#include "String2DateTimeFilter.h"

class TableViewTestWrapper : public TableView
{
	
public:
	TableViewTestWrapper(QWidget * parent, TableModel * model )
		: TableView(parent, model )
	{

		// test code
		StringColumnData *sc1 = new StringColumnData();
		StringColumnData *sc2 = new StringColumnData();
		sc1->setPlotDesignation(AbstractDataSource::X);
		sc2->setPlotDesignation(AbstractDataSource::Y);
		sc1->setLabel("col1");
		sc2->setLabel("col2");
		*sc1 << "one" << "two" << "three";
		*sc2 << "1" << "2" << "3";
		sc2->setInvalid(Interval<int>(1,1));
		sc1->setMasked(1);
		QList<AbstractColumnData *> list;
		list << sc1 << sc2;
		model->appendColumns(list);
		model->setInputFilter(0, new CopyThroughFilter());
		model->setInputFilter(1, new CopyThroughFilter());
		model->setOutputFilter(0, new CopyThroughFilter());
		model->setOutputFilter(1, new CopyThroughFilter());

		DoubleColumnData *dc1 = new DoubleColumnData();
		DoubleColumnData *dc2 = new DoubleColumnData();
		dc1->setPlotDesignation(AbstractDataSource::X);
		dc2->setPlotDesignation(AbstractDataSource::Y);
		dc1->setLabel("dcol1");
		dc2->setLabel("dcol2");
		*dc1 << 1.1 << 2.2 << 3.3 << 4.4 << 5.5;
		*dc2 << 5.6 << 12 << 128.7;
		dc2->setInvalid(Interval<int>(3,4));
		dc1->setMasked(2);
		list.clear();
		list << dc1 << dc2;
		model->appendColumns(list);
		model->setInputFilter(2, new String2DoubleFilter());
		model->setInputFilter(3, new String2DoubleFilter());
		model->setOutputFilter(2, new Double2StringFilter());
		model->setOutputFilter(3, new Double2StringFilter());

		DateTimeColumnData *dtc1 = new DateTimeColumnData();
		DateTimeColumnData *dtc2 = new DateTimeColumnData();
		dtc1->setPlotDesignation(AbstractDataSource::X);
		dtc2->setPlotDesignation(AbstractDataSource::Y);
		dtc1->setLabel("dtcol1");
		dtc2->setLabel("dtcol2");
		*dtc1 << QDateTime(QDate(2007,6,11),QTime(22,0,0,0));
		*dtc2 << QDateTime(QDate(),QTime()) << QDateTime(QDate(2000,1,1),QTime(0,0,0,0));
		dtc2->setInvalid(Interval<int>(5,6));
		dtc1->setMasked(3);
		list.clear();
		list << dtc1 << dtc2;
		model->appendColumns(list);
		model->setInputFilter(4, new String2DateTimeFilter());
		model->setInputFilter(5, new String2DateTimeFilter());
		model->setOutputFilter(4, new DateTime2StringFilter());
		model->setOutputFilter(5, new DateTime2StringFilter());

		DateTimeColumnData *dtc3 = new DateTimeColumnData();
		DateTimeColumnData *dtc4 = new DateTimeColumnData();
		dtc3->setPlotDesignation(AbstractDataSource::Y);
		dtc4->setPlotDesignation(AbstractDataSource::Y);
		dtc3->setLabel("day col");
		dtc4->setLabel("month col");
		*dtc3 << QDateTime(QDate(2007,6,11),QTime(22,0,0,0)) << QDateTime(QDate(0,12,1),QTime(12,0,0,0));
		*dtc4 << QDateTime(QDate(2007,6,11),QTime(22,0,0,0)) << QDateTime(QDate(0,12,1),QTime(12,0,0,0));
		list.clear();
		list << dtc3 << dtc4;
		model->appendColumns(list);
		model->setInputFilter(6, new String2DateTimeFilter()); //TODO: use String2DayOfWeekFilter
		model->setInputFilter(7, new String2DateTimeFilter());  //TODO: use String2MonthFilter
		DateTime2StringFilter * temp;
		temp = new DateTime2StringFilter();
		temp->setFormat("dddd");
		model->setOutputFilter(6, temp);
		temp = new DateTime2StringFilter();
		temp->setFormat("MMMM");
		model->setOutputFilter(7, temp); 
		// end of test code

	}
};


int main(int argc, char **argv)
{
	QApplication a(argc,argv);


	QMainWindow mw;
	mw.resize(800,600);

	TableViewTestWrapper tw(&mw, new TableModel(&mw));
	tw.resize(600,400);
	mw.show();

	a.exec();

	return 0;
}
