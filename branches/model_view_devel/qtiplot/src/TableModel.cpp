/***************************************************************************
    File                 : TableModel.cpp
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Model storing data for a table

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

#include "TableModel.h"
#include "DoubleColumnData.h"
#include "StringColumnData.h"
#include "DateColumnData.h"
#include "TimeColumnData.h"
#include <QString>
#include <QDate>
#include <QTime>
#include <QBrush>


TableModel::TableModel( QObject * parent )
	: QAbstractItemModel( parent )
{
	d_column_count = 0;
	d_row_count = 0;
	d_show_comments = false;
}


TableModel::~TableModel()
{
	// delete the columns and filters
	int end = d_columns.size();
	for(int i=0; i<end; i++)
		if(d_columns.at(i))
			delete d_columns.at(i);
	int end = d_input_filters.size();
	for(int i=0; i<end; i++)
		if(d_input_filters.at(i))
			delete d_input_filters.at(i);
	int end = d_output_filters.size();
	for(int i=0; i<end; i++)
		if(d_output_filters.at(i))
			delete d_output_filters.at(i);
}

Qt::ItemFlags TableModel::flags(const QModelIndex & index ) const
{
	if (index.isValid())
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
	else
		return Qt::ItemIsEnabled;
}


QVariant TableModel::data(const QModelIndex &index, int role) const
{
	if( !index.isValid() )
		return QVariant();
	
	switch(role)
	{
		case Qt::DisplayRole:
		case Qt::EditRole:
		case Qt::ToolTipRole:

			AbstractColumnData * col_ptr = d_columns[col];
			if(!col_ptr->isValid(index.row()))
				return QVariant();
			
			AbstractFilter * out_fltr = outputFilter(col);
			out_fltr->input(0, col_ptr);
			return QVariant(static_cast<StringColumnData *>(out_fltr->output(0))->valueAt(index.row()));

		case Qt::BackgroundRole:
			// TODO: Make masked color customizable
			// masked cells are displayed as hatched
			if(d_columns[index.column()]->isMasked(index.row()))
				return QVariant(QBrush(QColor(0xff,0,0), Qt::DiagCrossPattern));
			else
				break;
	}

	return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation,
		int role) const
{
	if( !( (role == Qt::ToolTipRole) ||
	       (role == Qt::DisplayRole || role == Qt::EditRole) ) )
		return QVariant();
		
	if (orientation == Qt::Horizontal)
		return d_horizontal_header_data.at(section);
	else if (orientation == Qt::Vertical)
		return d_vertical_header_data.at(section);

	return QVariant();
}

int TableModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return d_row_count;
}

int TableModel::columnCount(const QModelIndex & parent) const
{
	Q_UNUSED(parent)
	return d_column_count;
}

bool TableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (!index.isValid())
		return false;

	int row = index.row();
	int column = index.column();
	
	if(role == Qt::EditRole)
	{  
			AbstractColumnData * col_ptr = d_columns[col];
			if(col_ptr->size() <= row)
				col_ptr->resize(row+1);

			AbstractFilter * in_fltr = inputFilter(col);
			StringColumnData sd;
			sd << value.toString();
			in_fltr->input(0, &sd);
			col_ptr->copy(in_fltr->output(0), 0, row, 1); 
			emit dataChanged(index, index);
			return true;
	}
	
	return false;
}

QModelIndex TableModel::index(int row, int column, const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return createIndex(row, column);
}

QModelIndex TableModel::parent(const QModelIndex & child) const
{
    return QModelIndex();
}


AbstractColumnData * TableModel::output(int port) const
{
	if( (col < 0) || (col >= d_column_count) )
		return 0;
	
	return d_columns[col];
}

void TableModel::setOutputFilter(int col, AbstractFilter * filter)
{
	d_output_filters[col] = filter;
}

AbstractFilter * TableModel::outputFilter(int col) const
{
	if( (col < 0) || (col >= d_column_count) )
		return 0;
	
	return d_output_filters[col];
}

void TableModel::setInputFilter(int col, AbstractFilter * filter)
{
	d_input_filters[col] = filter;
}

AbstractFilter * TableModel::inputFilter(int col) const
{
	if( (col < 0) || (col >= d_column_count) )
		return 0;
	
	return d_input_filters[col];
}


void TableModel::replaceColumn(int col, AbstractColumnData * new_col)
{
	if( (col < 0) || (col >= d_column_count) )
		return;
	
	d_columns[col] = new_col;
	emit dataChanged(index(0, col), index(d_row_count-1, col));
}

void TableModel::emitDataChanged(int top, int left, int bottom, int right)
{
	emit dataChanged(index(top, left), index(bottom, right));
}

void TableModel::insertColumns(QList<AbstractColumnData *> cols, int first, int count)
{
	if(count < 1) 
		return;

	if(first < 0)
		first = 0;
	if(first > d_column_count)
		first = d_column_count;

	int i, rows;
	for(i=0; i<count; i++)
	{
		rows = cols.at(i)->numRows();
		if(rows > d_row_count)
			appendRows(rows-d_row_count); // append rows to resize table
	}

	beginInsertColumns(QModelIndex(), first, first+count-1);
	for(int i=count-1; i>=0; i--)
	{
		d_columns.insert(first, cols.at(i));
		d_input_filters.insert(first, 0);
		d_output_filters.insert(first, 0);
	}
	d_column_count += count;
	updateHorizontalHeader(first, first+count-1);
	endInsertColumns();	
}

void TableModel::removeColumns(int first, int count)
{
	if(count < 1) 
		return;
	if(first >= d_column_count)
		return;
	if(first < 0)
		return;

	if(first+count > d_column_count)
		count = d_column_count - first;

	beginRemoveColumns(QModelIndex(), first, first+count-1);
	for(int i=count-1; i>=0; i--)
	{
		d_columns.removeAt(first);
		d_input_filters.removeAt(first);
		d_output_filters.removeAt(first);
	}
	d_column_count -= count;
	updateHorizontalHeader(first, d_column_count);
	endRemoveColumns();	
}

void TableModel::appendColumns(QList<AbstractColumnData *> cols)
{
	insertColumns(cols, d_column_count);
}

void TableModel::insertRows(int first, int count)
{
	if(count < 1) 
		return;

	if(first < 0)
		first = 0;

	if(first > d_row_count)
		appendRows(count);
	else
	{
		beginInsertRows(QModelIndex(), first, first+count-1);
		AbstractColumnData * col_ptr;
		for(int col=0; col<d_column_count; col++)
		{
			col_ptr = d_columns[col];
			if(col_ptr->size() <= first) // no need to append empty rows
				continue;
			col_ptr->insertEmptyRows(first, count);
		}
		d_row_count += count;
		updateVerticalHeader(first);
		endInsertRows();
	}

}

void TableModel::appendRows(int count)
{
	if(count < 1) 
		return;

	beginInsertRows(QModelIndex(), d_row_count, d_row_count+count-1);
	d_row_count += count;
	updateVerticalHeader(d_row_count - count);
	endInsertRows();
}

void TableModel::updateVerticalHeader(int start_row)
{
	int current_size = d_vertical_header_data.size(), i;
	for(i=start_row; i<current_size; i++)
		d_vertical_header_data.replace(i, QString::number(i+1));
	for(; i<d_row_count; i++)
		d_vertical_header_data << QString::number(i+1);
	emit headerDataChanged(Qt::Vertical, start_row, d_row_count -1);	
}

void TableModel::updateHorizontalHeader(int start_col, int end_col)
{
	if(numColsWithPD(Table::X)>1)
	{
		x_cols = 0;
		for (int i=0; i<d_column_count; i++)
		{
			if (columnPlotDesignation(i) == Table::X)
				composeColumnHeader(i, columnLabel(i)+"[X" + QString::number(++x_cols) +"]");
			else if (columnPlotDesignation(i) == Table::Y)
			{
				if(x_cols>0)
					composeColumnHeader(i, columnLabel(i)+"[Y"+ QString::number(x_cols) +"]");
				else
					composeColumnHeader(i, columnLabel(i)+"[Y]");
			}
			else if (columnPlotDesignation(i) == Table::Z)
			{
				if(x_cols>0)
					composeColumnHeader(i, columnLabel(i)+"[Z"+ QString::number(x_cols) +"]");
				else
					composeColumnHeader(i, columnLabel(i)+"[Z]");
			}
			else if (columnPlotDesignation(i) == Table::xErr)
			{
				if(x_cols>0)
					composeColumnHeader(i, columnLabel(i)+"[xEr"+ QString::number(x_cols) +"]");
				else
					composeColumnHeader(i, columnLabel(i)+"[xEr]");
			}
			else if (columnPlotDesignation(i) == Table::yErr)
			{
				if(x_cols>0)
					composeColumnHeader(i, columnLabel(i)+"[yEr"+ QString::number(x_cols) +"]");
				else
					composeColumnHeader(i, columnLabel(i)+"[yEr]");
			}
			else
				composeColumnHeader(i, columnLabel(i));
		}
	}
	else
	{
		for (int i=0; i<d_column_count; i++)
		{
			if (columnPlotDesignation(i) == Table::X)
				composeColumnHeader(i, columnLabel(i)+"[X]");
			else if(columnPlotDesignation(i) == Table::Y)
				composeColumnHeader(i, columnLabel(i)+"[Y]");
			else if(columnPlotDesignation(i) == Table::Z)
				composeColumnHeader(i, columnLabel(i)+"[Z]");
			else if(columnPlotDesignation(i) == Table::xErr)
				composeColumnHeader(i, columnLabel(i)+"[xEr]");
			else if(columnPlotDesignation(i) == Table::yErr)
				composeColumnHeader(i, columnLabel(i)+"[yEr]");
			else
				composeColumnHeader(i, columnLabel(i));
		}
	}
	emit headerDataChanged(Qt::Horizontal, start_col, end_col);	
}


void TableModel::composeColumnHeader(int col, const QString& label)
{
	QString s = label;
	if (d_show_comments)
	{
		int lines = 10; // TODO: this needs improvement
		s.remove("\n");
		s += "\n" + QString(lines, '_') + "\n" + columnComment(col);
	}
	
	d_horizontal_header_data.replace(col, s);
}

void TableModel::setColumnLabel(int column, const QString& label)
{
	d_columns[column]->setLabel(label);
	updateHorizontalHeader(column, column);
}
	
QString TableModel::columnLabel(int column) const
{
	return d_columns[column]->label();
}

void TableModel::setColumnComment(int column, const QString& comment)
{
	d_columns[column]->setComment(comment);
	updateHorizontalHeader(column, column);
}
	
QString TableModel::columnComment(int column) const
{
	return d_columns[column]->comment();
	
}

void TableModel::setColumnPlotDesignation(int column, Table::PlotDesignation pd)
{
	d_columns[column]->setPlotDesignation(pd);
	updateHorizontalHeader(column, column);
}
	
Table::PlotDesignation TableModel::columnPlotDesignation(int column) const
{
	return d_columns[column]->plotDesignation();
	
}

void TableModel::showComments(bool on)
{
	if (d_show_comments == on)
		return;

	d_show_comments = on;
	updateHorizontalHeader(0, d_column_count-1);
}

bool TableModel::areCommentsShown()
{
	return d_show_comments;
}

QString TableModel::columnHeader(int col)
{
	return headerData(int section, Qt::Horizontal).toString();
}

int TableModel::numColsWithPD(Table::PlotDesignation pd)
{
	if(pd == Table::All)
		return d_column_count;

	int count = 0;
	
	for (int i=0; i<d_column_count; i++)
		if(columnPlotDesignation(i) == pd)
			count++;
	
	return count;
}

