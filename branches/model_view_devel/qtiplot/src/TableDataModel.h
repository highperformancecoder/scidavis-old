/***************************************************************************
    File                 : TableDataModel.h
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

#ifndef TABLEDATAMODEL_H
#define TABLEDATAMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QStringList>
#include "AbstractColumnData.h"

#include <QtDebug>

//! Model storing data for a table
/**
  This model class provides the interface to column based data 
  (i.e. 1D vector based data like x-values and y-values for a plot).
  It implements the QAbstractItemModel interface, allowing it to be 
  used with Qt's standard views, and additionally provides methods for fast,
  direct access to the stored data.

  Each column saves its data in a class derived from AbstractColumnData
  and can be directly accessed by the pointer return by columnPointer().

  Important: For memory saving reasons the columns usually have less
  rows than the displayed number of rows and are not automatically
  resized when the number of rows in the table is increased. 
  */
class TableDataModel : public QAbstractItemModel
{
	Q_OBJECT

public:
    //! Constructor
    explicit TableDataModel( QObject * parent = 0 );
    //! Destructor
    ~TableDataModel();


	//! \name Overloaded functions from QAbstractItemModel
	//@{
	Qt::ItemFlags flags( const QModelIndex & index ) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, 
			Qt::Orientation orientation,int role) const;
	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex & parent) const;
	bool setData(const QModelIndex & index, const QVariant & value, int role);
	bool setHeaderData( int section, Qt::Orientation orientation, const QVariant & value, int role = Qt::EditRole );
	QModelIndex index(int row, int column, const QModelIndex &parent) const;
	QModelIndex parent(const QModelIndex & child) const;
	//@}

	//! \name Other functions
	//@{
	//! Return a pointer to column number 'index'
	/**
	 * \return returns a pointer to the column data or zero if the index is invalid
	 */
	AbstractColumnData * columnPointer(int index) const;
	//! Insert columns before column number 'index'
	/**
	 * If index is higher than (current number of columns -1),
	 * the columns will be appended.
	 * \param type see AbstractColumnData::ColumnDataType
	 * \param col_index index of the column to insert before
	 * \param count number of columns to insert
	 * \param rows number of initialized rows for the column
	 */
	void insertColumns(int type, int col_index, int count, int rows = 0);
	//! Insert rows before row number 'index'
	/**
	 * If index is higher than (current number of rows -1),
	 * the rows will be appended.
	 * \param row_index index of the row to insert before
	 * \param count number of rows to insert
	 */
	void insertRows(int row_index, int count = 1);
	//! Append rows to the table
	void appendRows(int count);
	//! Append columns to the table
	 /**
	 * \param type see AbstractColumnData::ColumnDataType
	 * \param count number of columns to insert
	 * \param rows number of initialized rows for the column
	 */
	void appendColumns(int type, int count, int rows = 0);
	//! Return the number of columns in the table
	int columnCount() { return d_column_count; }
	//! Return the number of rows in the table
	int rowCount() { return d_row_count; }
	//@}
	
	
private:
	//! The number of columns
	int d_column_count;
	//! The maximum number of rows of all columns
	int d_row_count;
	//! Vertical header data
	QStringList d_vertical_header_data;
	//! Horizontal header data
	QStringList d_horizontal_header_data;
	//! List of pointers to the column data vectors
	QList<AbstractColumnData *> d_columns;	
	
	//! Update the vertical header labels
	/**
	 * \param start_row first row that needs to be updated
	 */
	void updateVerticalHeader(int start_row);
}; 

#endif
