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
#include <QItemSelectionModel>
#include <QLocale>
#include <QMenu>
#include <QContextMenuEvent>

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
			*sc1 << "one" << "two" << "three" << "four" << "five" << "six" << "seven";
			*sc2 << "1" << "2" << "3" << "4" << "5";
			sc2->setInvalid(Interval<int>(1,2));
			sc1->setMasked(Interval<int>(2,4));
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
			*dc1 << 1.1 << 2.2 << 3.3 << 4.4 << 5.5 << 6.6 << 7.7 << 8.8 << 9.9 << 10.10;
			*dc2 << 5.6 << 12 << 128.7 << 33 << 144;
			dc2->setInvalid(Interval<int>(3,4));
			dc1->setMasked(Interval<int>(2,6));
			dc2->setMasked(1);
			dc1->setInvalid(Interval<int>(3,6));
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
		}

	public slots:
		void startTests()
		{
			undoTest();
			setValuesTest();
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
			undo_stack->push(new TableRemoveRowsCmd(d_model, 1, 3) );
			undo_stack->push(new TableRemoveColumnsCmd(d_model, 3, 2) );
			undo_stack->push(new TableInsertRowsCmd(d_model, 0, 2) );
			
			QList<AbstractColumnData *> cols;
			QList<AbstractFilter *> in_filters;
			QList<AbstractFilter *> out_filters;
			int max=1;
			cols << new DoubleColumnData();
			cols.at(0)->setLabel(QString::number(max++));
			in_filters << new String2DoubleFilter();
			out_filters << new Double2StringFilter();
			for(int i=0; i<d_model->rowCount(); i++)
				static_cast<DoubleColumnData*>(cols.at(0))->append(i);

			cols << new StringColumnData();
			cols.at(1)->setLabel(QString::number(max++));
			in_filters << new CopyThroughFilter();
			out_filters << new CopyThroughFilter();

			undo_stack->push(new TableAppendColumnsCmd(d_model, cols, in_filters, out_filters) );

			cols.clear();
			in_filters.clear();
			out_filters.clear();
			for(int i=0; i <2; i++)
			{
				cols << new DoubleColumnData();
				cols.at(i)->setLabel(QString::number(max++));
				in_filters << new String2DoubleFilter();
				out_filters << new Double2StringFilter();
			}
			undo_stack->push(new TableInsertColumnsCmd(d_model, 0, cols, in_filters, out_filters) );
		}

	void setValuesTest()
	{
		QLocale::setDefault(QLocale::c());
		QStringList values;
		values << "1.5" << "2.8";
		QItemSelectionModel * sm = selectionModel();		
		sm->select(d_model->index(1, 8, QModelIndex()), QItemSelectionModel::Select);
		sm->select(d_model->index(2, 8, QModelIndex()), QItemSelectionModel::Select);
		sm->select(d_model->index(4, 8, QModelIndex()), QItemSelectionModel::Select);
		sm->select(d_model->index(5, 8, QModelIndex()), QItemSelectionModel::Select);
		sm->select(d_model->index(1, 9, QModelIndex()), QItemSelectionModel::Select);
		sm->select(d_model->index(2, 9, QModelIndex()), QItemSelectionModel::Select);
		undo_stack->push( new TableSetColumnValuesCmd(d_model, 8, values) );

		sm->select(d_model->index(0, 2, QModelIndex()), QItemSelectionModel::Select);
		sm->select(d_model->index(1, 2, QModelIndex()), QItemSelectionModel::Select);
		sm->select(d_model->index(4, 2, QModelIndex()), QItemSelectionModel::Select);
		sm->select(d_model->index(5, 2, QModelIndex()), QItemSelectionModel::Select);

		sm->select(d_model->index(0, 1, QModelIndex()), QItemSelectionModel::Select);
		sm->select(d_model->index(1, 1, QModelIndex()), QItemSelectionModel::Select);
		sm->select(d_model->index(4, 1, QModelIndex()), QItemSelectionModel::Select);
		sm->select(d_model->index(7, 1, QModelIndex()), QItemSelectionModel::Select);

		// row numbers
		int rows = d_model->rowCount();
		int columns = d_model->columnCount();
		QStringList data;

		for(int i=0; i<columns; i++)
		{
			data.clear();
			for(int j=0; j<rows; j++)
			{
				if(d_model->output(i)->isSelected(j))
					data << QString::number(j+1);
			}
			if(data.count() > 0)
			{
				QUndoCommand * temp = new TableSetColumnValuesCmd(d_model, i, data);
				temp->setText("set column values: row numbers");
				undo_stack->push( temp );
			}
		}

		// random values
		qsrand(QTime::currentTime().msec());

		for(int i=0; i<columns; i++)
		{
			data.clear();
			for(int j=0; j<rows; j++)
			{
				if(d_model->output(i)->isSelected(j))
					data << QString::number(double(qrand())/double(RAND_MAX));
			}
			if(data.count() > 0)
			{
				QUndoCommand * temp = new TableSetColumnValuesCmd(d_model, i, data);
				temp->setText("set column values: random values");
				undo_stack->push( temp );
			}
		}

	}

	protected:
		void contextMenuEvent ( QContextMenuEvent * e ) 
		{
			QMenu contextMenu(this);
			contextMenu.addAction("start tests", this, SLOT(startTests()));
			contextMenu.exec(e->globalPos());
		}
};



