/***************************************************************************
    File                 : TableDataModel.cpp
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

#include "TableDataModel.h"
#include "DoubleColumnData.h"
#include "StringColumnData.h"
#include "DateColumnData.h"
#include "TimeColumnData.h"
#include <QString>
#include <QDate>
#include <QTime>


TableDataModel::TableDataModel( QObject * parent )
	: QAbstractItemModel( parent )
{
	d_column_count = 0;
	d_row_count = 0;

	// TODO: remove test code
	insertColumns(AbstractColumnData::Double, 0, 1, 0);
	insertColumns(AbstractColumnData::Double, 0, 1, 10);
	insertColumns(AbstractColumnData::Double, 0, 1, 15);
	double * ptr = static_cast< DoubleColumnData* >(columnPointer(0))->data();
	int end = static_cast< DoubleColumnData* >(columnPointer(0))->size();
	for(int i=0; i<end; i++)
		ptr[i] = i;

	appendRows(2);
	insertRows(0, 3);
	static_cast< DoubleColumnData* >(columnPointer(1))->resize(d_row_count);
	ptr = static_cast< DoubleColumnData* >(columnPointer(1))->data();
	end = d_row_count;
	for(int i=0; i<end; i++)
		ptr[i] = i*2;
	insertColumns(AbstractColumnData::String, columnCount(), 2, 10);
	insertColumns(AbstractColumnData::Date, columnCount(), 1, 10);
	insertColumns(AbstractColumnData::Date, columnCount(), 2, 10);
	insertColumns(AbstractColumnData::Time, columnCount(), 1, 10);
	// end of test code
}


TableDataModel::~TableDataModel()
{
	// delete the columns
	int end = d_columns.size();
	for(int i=0; i<end; i++)
		delete d_columns.at(i);
	d_columns.clear();
	
}

Qt::ItemFlags TableDataModel::flags(const QModelIndex & index ) const
{
	if (index.isValid())
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
	else
		return Qt::ItemIsEnabled;
}


QVariant TableDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
		return QVariant();

	int col = index.column();
	int row = index.row();

	if( (role == Qt::DisplayRole) || (role == Qt::EditRole) || (role == Qt::ToolTipRole) )
	{ 
		AbstractColumnData * col_ptr = columnPointer(col);
		if(col_ptr)
			return QVariant(col_ptr->cellString(row));
	}
	return QVariant();
}

QVariant TableDataModel::headerData(int section, Qt::Orientation orientation,
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

int TableDataModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return d_row_count;
}

int TableDataModel::columnCount(const QModelIndex & parent) const
{
	Q_UNUSED(parent)
	return d_column_count;
}

bool TableDataModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (!index.isValid())
		return false;

	int row = index.row();
	int column = index.column();
	
	if(role == Qt::EditRole)
	{  
		AbstractColumnData * col_ptr = columnPointer(column);
		if(col_ptr)
		{
			if(col_ptr->size() <= row)
				col_ptr->resize(row+1);
			col_ptr->setCellFromString(row, value.toString());
			emit dataChanged(index, index);
			return true;
		}
	}
	
	return false;
}

bool TableDataModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role )
{
	if( !( (role == Qt::ToolTipRole) ||
	       (role == Qt::DisplayRole || role == Qt::EditRole) ) )
		return false;
	
	if( section < 0 )
		return false;
		
	if (orientation == Qt::Horizontal)
	{
		if( section >= d_column_count )
			return false;
		d_horizontal_header_data.replace(section, value.toString());
		emit headerDataChanged(orientation, section, section);	
		return true;
	}
	else if (orientation == Qt::Vertical)
	{
		if( section >= d_row_count )
			return false;
		d_vertical_header_data.replace(section, value.toString());
		emit headerDataChanged(orientation, section, section);	
		return true;
	}
	else return false;
}

QModelIndex TableDataModel::index(int row, int column, const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return createIndex(row, column);
}

QModelIndex TableDataModel::parent(const QModelIndex & child) const
{
    return QModelIndex();
}


AbstractColumnData * TableDataModel::columnPointer(int index) const
{
	if( (index < 0) || (index >= d_column_count) )
		return 0;
	
	return d_columns[index];
}

void TableDataModel::insertColumns(int type, int col_index, int count, int rows)
{
	if(count < 1) 
		return;

	if(col_index < 0)
		col_index = 0;
	if(col_index > d_column_count)
		col_index = d_column_count;

	if(rows > d_row_count)
		appendRows(rows-d_row_count); // append rows to resize table

	beginInsertColumns(index(0, col_index, QModelIndex()), col_index, col_index+count-1);
	switch(type)
	{
		case AbstractColumnData::String:
			{ // the braces are needed to be able to declare variables inside the case block
				StringColumnData * new_col;
				for(int i=0; i<count; i++)
				{
					new_col = new StringColumnData();
					new_col->resize(rows);
					d_columns.insert(col_index, new_col);
					// TODO: implement this properly
					d_horizontal_header_data.insert(col_index, QString("String%1").arg(count) );
				}
			}
			break;

		case AbstractColumnData::Date:
			{ 
				DateColumnData * new_col;
				for(int i=0; i<count; i++)
				{
					new_col = new DateColumnData();
					new_col->resize(rows);
					d_columns.insert(col_index, new_col);
					// TODO: implement this properly
					d_horizontal_header_data.insert(col_index, QString("Date%1").arg(count) );
				}
			}
			break;

		case AbstractColumnData::Time:
			{ 
				TimeColumnData * new_col;
				for(int i=0; i<count; i++)
				{
					new_col = new TimeColumnData();
					new_col->resize(rows);
					d_columns.insert(col_index, new_col);
					// TODO: implement this properly
					d_horizontal_header_data.insert(col_index, QString("Time%1").arg(count) );
				}
			}
			break;

		default: // use AbstractColumnData::Double as default, although no invalid type should be used
			{
				DoubleColumnData * new_col;
				for(int i=0; i<count; i++)
				{
					new_col = new DoubleColumnData(rows);
					d_columns.insert(col_index, new_col);
					// TODO: implement this properly
					d_horizontal_header_data.insert(col_index, QString("Numeric%1").arg(count) );

				}
			}
	}
	emit headerDataChanged(Qt::Horizontal, col_index, col_index + count -1);	
	d_column_count += count;
	endInsertColumns();	
}

void TableDataModel::insertRows(int row_index, int count)
{
	if(count < 1) 
		return;

	if(row_index < 0)
		row_index = 0;

	if(row_index > d_row_count)
		appendRows(count);
	else
	{
		beginInsertRows(index(row_index, 0, QModelIndex()), row_index, row_index+count-1);
		AbstractColumnData * col_ptr;
		for(int col=0; col<d_column_count; col++)
		{
			col_ptr = columnPointer(col);

			if(col_ptr->size() <= row_index) // no need to append empty rows
				continue;

			switch(col_ptr->type())
			{
				case AbstractColumnData::Double:
					static_cast< DoubleColumnData* >(col_ptr)->insert(row_index, count, 0);
					break;

				case AbstractColumnData::String:
					{ // the braces are needed to be able to declare variables inside the case block
					StringColumnData * str_data_ptr = static_cast< StringColumnData* >(col_ptr);
					for(int i=0; i<count; i++)
						str_data_ptr->insert(row_index, QString());
					}
					break;

				case AbstractColumnData::Date:
					{
					DateColumnData * date_data_ptr = static_cast< DateColumnData* >(col_ptr);
					for(int i=0; i<count; i++)
						date_data_ptr->insert(row_index, QDate());
					}
					break;

				case AbstractColumnData::Time:
					{
					TimeColumnData * time_data_ptr = static_cast< TimeColumnData* >(col_ptr);
					for(int i=0; i<count; i++)
						time_data_ptr->insert(row_index, QTime());
					}
					break;
			}
		}
		d_row_count += count;
		updateVerticalHeader(row_index);
		endInsertRows();
	}

}

void TableDataModel::appendRows(int count)
{
	if(count < 1) 
		return;

	beginInsertRows(index(d_row_count, 0, QModelIndex()), d_row_count, d_row_count+count-1);
	d_row_count += count;
	updateVerticalHeader(d_row_count - count);
	endInsertRows();
}

void TableDataModel::updateVerticalHeader(int start_row)
{
	int current_size = d_vertical_header_data.size(), i;
	for(i=start_row; i<current_size; i++)
		d_vertical_header_data.replace(i, QString::number(i+1));
	for(; i<d_row_count; i++)
		d_vertical_header_data << QString::number(i+1);
	emit headerDataChanged(Qt::Vertical, start_row, d_row_count -1);	
}


