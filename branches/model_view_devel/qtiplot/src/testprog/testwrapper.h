#include <QApplication>
#include <QMainWindow>
#include <QtDebug>
#include <QTime>
#include <QDate>
#include <QString>
#include <QUndoView>
#include <QUndoStack>
#include <QUndoCommand>
#include <QHBoxLayout>
#include <QObject>

#include "TableView.h"
#include "TableModel.h"
#include "tablecommands.h"
#include "TableItemDelegate.h"

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
	Q_OBJECT

public:
	~TableViewTestWrapper() {};

	QUndoStack * undo_stack;

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
		*dc1 << 1.1 << 2.2 << 3.3 << 4.4 << 5.5 << 6.6 << 7.7;
		*dc2 << 5.6 << 12 << 128.7 << 33 << 144;
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
		*dtc2 << QDateTime(QDate(),QTime()) << QDateTime(QDate(2000,1,1),QTime(0,0,0,0)) << QDateTime() << QDateTime() << QDateTime();
		dtc2->setInvalid(Interval<int>(3,4));
		dtc2->setMasked(1);
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

		undo_stack = model->undoStack();
		undoTest();
	}

private:
	void undoTest()
	{
		
	undo_stack->push(new TableSetColumnPlotDesignationCmd(d_model, 1, AbstractDataSource::X) );
	undo_stack->push(new TableSetColumnPlotDesignationCmd(d_model, 3, AbstractDataSource::X) );
	undo_stack->push(new TableSetColumnLabelCmd(d_model, 1, "col two") );
	undo_stack->push(new TableSetColumnLabelCmd(d_model, 4, "col five") );
	undo_stack->push(new TableSetColumnCommentCmd(d_model, 0, "a comment") );
	undo_stack->push(new TableShowCommentsCmd(d_model, true) );
	undo_stack->push(new TableClearColumnCmd(d_model, 3) );
	undo_stack->push(new TableClearColumnCmd(d_model, 1) );
	undo_stack->push(new TableSetColumnCommentCmd(d_model, 1, "I am\na long\nmultiline\ncomment") );
	undo_stack->push(new TableSetColumnCommentCmd(d_model, 2, "I am\nanother\nlong\nmultiline\ncomment") );
	undo_stack->push(new TableShowCommentsCmd(d_model, false) );
	undo_stack->push(new TableSetColumnCommentCmd(d_model, 3, "I am\nanother\nlong\nmultiline\ncomment") );
	undo_stack->push(new TableShowCommentsCmd(d_model, true) );
	}

};



