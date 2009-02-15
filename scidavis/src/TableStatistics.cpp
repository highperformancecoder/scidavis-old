/***************************************************************************
	File                 : TableStatistics.cpp
	Project              : SciDAVis
--------------------------------------------------------------------
	Copyright            : (C) 2006 by Knut Franke
	Email (use @ for *)  : knut.franke*gmx.de
	Description          : Table subclass that displays statistics on
	                       columns or rows of another table

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/
#include "TableStatistics.h"
#include "table/TableModel.h"
#include "table/TableView.h"
#include "table/future_Table.h"
#include "table/TableDoubleHeaderView.h"
#include "core/column/Column.h"
#include "core/datatypes/Double2StringFilter.h"

#include <QList>
#include <QMenu>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_statistics.h>

TableStatistics::TableStatistics(ScriptingEnv *env, QWidget *parent, Table *base, Type t, QList<int> targets)
	: Table(env, 1, 1, "", parent, ""),
	d_base(base), d_type(t), d_targets(targets)
{
#ifdef LEGACY_CODE_0_2_x
	static_cast<TableModel *>(d_view_widget->model())->setReadOnly(true);
	d_hide_button->hide();
	d_control_tabs->hide();

	d_future_table->action_cut_selection->setEnabled(false);
	d_future_table->action_paste_into_selection->setEnabled(false);
	d_future_table->action_set_formula->setEnabled(false);
	d_future_table->action_clear_selection->setEnabled(false);
	d_future_table->action_recalculate->setEnabled(false);
	d_future_table->action_fill_row_numbers->setEnabled(false);
	d_future_table->action_fill_random->setEnabled(false);
	d_future_table->action_add_column->setEnabled(false);
	d_future_table->action_clear_table->setEnabled(false);
	d_future_table->action_sort_table->setEnabled(false);
	d_future_table->action_dimensions_dialog->setEnabled(false);
	d_future_table->action_insert_columns->setEnabled(false);
	d_future_table->action_remove_columns->setEnabled(false);
	d_future_table->action_clear_columns->setEnabled(false);
	d_future_table->action_add_columns->setEnabled(false);
	d_future_table->action_normalize_columns->setEnabled(false);
	d_future_table->action_normalize_selection->setEnabled(false);
	d_future_table->action_sort_columns->setEnabled(false);
	d_future_table->action_statistics_columns->setEnabled(false);
	d_future_table->action_type_format->setEnabled(false);
	d_future_table->action_edit_description->setEnabled(false);
	d_future_table->action_insert_rows->setEnabled(false);
	d_future_table->action_remove_rows->setEnabled(false);
	d_future_table->action_clear_rows->setEnabled(false);
	d_future_table->action_add_rows->setEnabled(false);
	d_future_table->action_statistics_rows->setEnabled(false);
	d_future_table->action_toggle_tabbar->setEnabled(false);

#endif
	setCaptionPolicy(MyWidget::Both);
	if (d_type == TableStatistics::StatRow)
	{
		setName(QString(d_base->name())+"-"+tr("RowStats"));
		setWindowLabel(tr("Row Statistics of %1").arg(base->name()));
		d_future_table->setRowCount(d_targets.size());
		d_future_table->setColumnCount(9);
		setColName(0, tr("Row"));
		setColName(1, tr("Cols"));
		setColName(2, tr("Mean"));
		setColName(3, tr("StandardDev"));
		setColName(4, tr("Variance"));
		setColName(5, tr("Sum"));
		setColName(6, tr("Max"));
		setColName(7, tr("Min"));
		setColName(8, "N");

		for (int i=0; i < 1; i++)
			setColumnType(i, SciDAVis::Text);
		
		for (int i=1; i < 9; i++)
			setColumnType(i, SciDAVis::Numeric);

		Double2StringFilter *pFilter = qobject_cast<Double2StringFilter*>(column(1)->outputFilter());
		Q_ASSERT(pFilter != NULL);
		pFilter->setNumDigits(0);	
		pFilter->setNumericFormat('f');	
		pFilter = qobject_cast<Double2StringFilter*>(column(8)->outputFilter());
		Q_ASSERT(pFilter != NULL);
		pFilter->setNumDigits(0);	
		pFilter->setNumericFormat('f');	

		for (int i=0; i < d_targets.size(); i++)
			column(0)->setTextAt(i, QString::number(d_targets[i]+1));

		update(d_base, QString());
	}
	else if (d_type == TableStatistics::StatColumn)
	{
		setName(QString(d_base->name())+"-"+tr("ColStats"));
		setWindowLabel(tr("Column Statistics of %1").arg(base->name()));
		d_future_table->setRowCount(d_targets.size());
		d_future_table->setColumnCount(11);
		setColName(0, tr("Col"));
		setColName(1, tr("Rows"));
		setColName(2, tr("Mean"));
		setColName(3, tr("StandardDev"));
		setColName(4, tr("Variance"));
		setColName(5, tr("Sum"));
		setColName(6, tr("iMax"));
		setColName(7, tr("Max"));
		setColName(8, tr("iMin"));
		setColName(9, tr("Min"));
		setColName(10, "N");

		for (int i=0; i < 2; i++)
			setColumnType(i, SciDAVis::Text);
		
		for (int i=2; i < 11; i++)
			setColumnType(i, SciDAVis::Numeric);

		Double2StringFilter *pFilter = qobject_cast<Double2StringFilter*>(column(6)->outputFilter());
		Q_ASSERT(pFilter != NULL);
		pFilter->setNumDigits(0);	
		pFilter->setNumericFormat('f');	
		pFilter = qobject_cast<Double2StringFilter*>(column(8)->outputFilter());
		Q_ASSERT(pFilter != NULL);
		pFilter->setNumDigits(0);	
		pFilter->setNumericFormat('f');	
		pFilter = qobject_cast<Double2StringFilter*>(column(10)->outputFilter());
		Q_ASSERT(pFilter != NULL);
		pFilter->setNumDigits(0);	
		pFilter->setNumericFormat('f');	

		for (int i=0; i < d_targets.size(); i++)
		{
			column(0)->setTextAt(i, d_base->colLabel(d_targets[i]));
			update(d_base, d_base->colName(d_targets[i]));
		}
	}
	setColPlotDesignation(0, SciDAVis::X);
}

void TableStatistics::update(Table *t, const QString& colName)
{
	if (t != d_base) return;

	int j;
	if (d_type == TableStatistics::StatRow)
		for (int r=0; r < d_targets.size(); r++)
		{
			int cols=d_base->numCols();
			int i = d_targets[r];
			int m = 0;
			for (j = 0; j < cols; j++)
				if (d_base->column(j)->rowCount() > i && d_base->column(j)->columnMode() == SciDAVis::Numeric)
					m++;

			if (cols == 0)
			{//clear row statistics
				for (j = 1; j<9; j++)
					column(j)->clear();
			}

			if (m > 0)
			{
				double *dat = new double[m];
				gsl_vector *y = gsl_vector_alloc (m);
				int temp = 0;
				for (j = 0; j<cols; j++)
				{
					if (d_base->column(j)->rowCount() > i && d_base->column(j)->columnMode() == SciDAVis::Numeric)
					{					
						double val = d_base->column(j)->valueAt(i);
						gsl_vector_set (y, temp, val);
						dat[temp] = val;
						temp++;
					}
				}
				double mean = gsl_stats_mean (dat, 1, m);
				double min, max;
				gsl_vector_minmax (y, &min, &max);

				column(1)->setValueAt(r, d_base->numCols());
				column(2)->setValueAt(r, mean);
				column(3)->setValueAt(r, gsl_stats_sd(dat, 1, m));
				column(4)->setValueAt(r, gsl_stats_variance(dat, 1, m));
				column(5)->setValueAt(r, mean*m);
				column(6)->setValueAt(r, max);
				column(7)->setValueAt(r, min);
				column(8)->setValueAt(r, m);

				gsl_vector_free (y);
				delete[] dat;
			}
		}
	else if (d_type == TableStatistics::StatColumn)
		for (int c=0; c < d_targets.size(); c++)
			if (colName == QString(d_base->name())+"_"+d_base->colLabel(d_targets[c]))
			{
				int i = d_base->colIndex(colName);
				Column *col = d_base->column(i);
				
				if (col->columnMode() != SciDAVis::Numeric) return;

				int rows = col->rowCount();
				if (rows == 0)
				{
					col->clear();
					return;
				}
				double *dat = new double[rows];
				gsl_vector *y = gsl_vector_alloc(rows);

				int min_index = 0, max_index = 0;
				double val = col->valueAt(0);
				gsl_vector_set (y, 0, val);
				dat[0] = val;
				double min = val, max = val;
				for (j = 1; j<rows; j++)
				{
					val = col->valueAt(j);
					gsl_vector_set (y, j, val);
					dat[j] = val;
					if (val < min)
					{
						min = val;
						min_index = j;
					}
					if (val > max)
					{
						max = val;
						max_index = j;
					}
				}
				double mean=gsl_stats_mean (dat, 1, rows);

				column(1)->setTextAt(c, "[1:"+QString::number(rows)+"]");
				column(2)->setValueAt(c, mean);
				column(3)->setValueAt(c, gsl_stats_sd(dat, 1, rows));
				column(4)->setValueAt(c, gsl_stats_variance(dat, 1, rows));
				column(5)->setValueAt(c, mean*rows);
				column(6)->setValueAt(c, max_index + 1);
				column(7)->setValueAt(c, max);
				column(8)->setValueAt(c, min_index + 1);
				column(9)->setValueAt(c, min);
				column(10)->setValueAt(c, rows);

				gsl_vector_free (y);
				delete[] dat;
			}

	for (int i=0; i<numCols(); i++)
		emit modifiedData(this, Table::colName(i));
}

void TableStatistics::renameCol(const QString &from, const QString &to)
{
	if (d_type == TableStatistics::StatRow) return;
	for (int c=0; c < d_targets.size(); c++)
		if (from == QString(d_base->name())+"_"+text(c, 0))
		{
			column(0)->setTextAt(c, to.section('_', 1, 1));
			return;
		}
}

void TableStatistics::removeCol(const QString &col)
{
	if (d_type == TableStatistics::StatRow)
	{
		update(d_base, col);
		return;
	}
	for (int c=0; c < d_targets.size(); c++)
		if (col == QString(d_base->name())+"_"+text(c, 0))
		{
			d_targets.remove(d_targets.at(c));
			d_future_table->removeRows(c,1);
			return;
		}
}

QString TableStatistics::saveToString(const QString &geometry)
{
	QString s = "<TableStatistics>\n";
	s += QString(name())+"\t";
	s += QString(d_base->name())+"\t";
	s += QString(d_type == StatRow ? "row" : "col") + "\t";
	s += birthDate()+"\n";
	s += "Targets";
	for (QList<int>::iterator i=d_targets.begin(); i!=d_targets.end(); ++i)
		s += "\t" + QString::number(*i);
	s += "\n";
	s += geometry;
	s += saveHeader();
	s += saveColumnWidths();
	s += saveCommands();
	s += saveColumnTypes();
	s += saveComments();
	s += "WindowLabel\t" + windowLabel() + "\t" + QString::number(captionPolicy()) + "\n";
	return s + "</TableStatistics>\n";
}

#ifdef LEGACY_CODE_0_2_x
bool TableStatistics::eventFilter(QObject * watched, QEvent * event)
{
	QHeaderView * v_header = d_view_widget->verticalHeader();

	if (event->type() == QEvent::ContextMenu) 
	{
		QContextMenuEvent *cm_event = static_cast<QContextMenuEvent *>(event);
		QPoint global_pos = cm_event->globalPos();
		if(watched == v_header)	
		{
			// no enabled actions for rows
		}
		else if(watched == d_horizontal_header)
		{
			QMenu context_menu;

			if(d_future_table->d_plot_menu)
			{
				context_menu.addMenu(d_future_table->d_plot_menu);
				context_menu.addSeparator();
			}

			QMenu * submenu = new QMenu(tr("S&et Column(s) As"));
			submenu->addAction(d_future_table->action_set_as_x);
			submenu->addAction(d_future_table->action_set_as_y);
			submenu->addAction(d_future_table->action_set_as_z);
			submenu->addSeparator();
			submenu->addAction(d_future_table->action_set_as_xerr);
			submenu->addAction(d_future_table->action_set_as_yerr);
			submenu->addSeparator();
			submenu->addAction(d_future_table->action_set_as_none);
			context_menu.addMenu(submenu);
			context_menu.addSeparator();

			connect(&context_menu, SIGNAL(aboutToShow()), d_future_table, SLOT(adjustActionNames()));
			context_menu.addAction(d_future_table->action_toggle_comments);

			context_menu.exec(global_pos);
		}
		else if(watched == d_view_widget)
		{
			QMenu context_menu;

			if(d_future_table->d_plot_menu)
			{
				context_menu.addMenu(d_future_table->d_plot_menu);
				context_menu.addSeparator();
			}

			context_menu.addAction(d_future_table->action_copy_selection);
			context_menu.addSeparator();
			context_menu.addSeparator();

			connect(&context_menu, SIGNAL(aboutToShow()), d_future_table, SLOT(adjustActionNames()));
			context_menu.addAction(d_future_table->action_toggle_comments);
			context_menu.addSeparator();
			context_menu.addAction(d_future_table->action_select_all);
			context_menu.addSeparator();
			context_menu.addAction(d_future_table->action_go_to_cell);

			context_menu.exec(global_pos);
		}
		else
			return TableView::eventFilter(watched, event);

		return true;
	} 
	else 
		return TableView::eventFilter(watched, event);
}
#endif

