/***************************************************************************
    File                 : StringColumnData.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Data source that stores a list of strings (implementation)

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

#ifndef STRINGCOLUMNDATA_H
#define STRINGCOLUMNDATA_H

#include "AbstractColumnData.h"
#include "AbstractStringDataSource.h"
#include <QStringList>

//! Data source that stores a list of strings (implementation)
/**
  * This class stores a list of QStrings. 
  * It implements the interfaces defined in AbstractColumnData, 
  * AbstractDataSource, and AbstractStringDataSource as well as the data type specific
  * writing functions. The stored data can also be accessed by
  * the functions of QStringList.
  * \sa AbstractColumnData
  * \sa AbstractDataSource
  * \sa AbstractDoubleDataSource
  */
class StringColumnData : public AbstractColumnData, public AbstractStringDataSource, public QStringList
{
	Q_OBJECT

public:
	//! Dtor
	virtual ~StringColumnData(){};

	//! Ctor
	explicit StringColumnData();
	//! Ctor
	explicit StringColumnData(const QStringList& list);

	//! \name Data writing functions
	//@{
	//! Copy (if necessary convert) another vector of data
	virtual bool copy(const AbstractDataSource * other);
	//! Set a row value from a string
	virtual void setRowFromString(int row, const QString& string);
	//! Resize the string list
	virtual void setNumRows(int new_size);
	//! Set the column label
	virtual void setLabel(const QString& label);
	//! Set the column comment
	virtual void setComment(const QString& comment);
	//! Set the column plot designation
	/**
	 * Don't ever use AbstractDataSource::All here!
	 */
	virtual void setPlotDesignation(AbstractDataSource::PlotDesignation pd);
	//! Insert some empty rows
	virtual void insertEmptyRows(int before, int count);
	//! Remove 'count' rows starting from row 'first'
	virtual void removeRows(int first, int count);
	//! This must be called before the column is replaced by another
	virtual void notifyReplacement(AbstractDataSource * replacement);
	//@}

	//! \name Data reading functions
	//@{
	//! Return the list size
	virtual int numRows() const;
	//! Return the string in row 'row'
	virtual QString textAt(int row) const;
	//! Return the corresponding double value of row 'row'
	virtual double valueAt(int row) const;
	//! Return the column label
	virtual QString label() const;
	//! Return the column comment
	virtual QString comment() const;
	//! Return the column plot designation
	virtual AbstractDataSource::PlotDesignation plotDesignation() const;
	//@}

protected:
	//! The column label
	QString d_label;
	//! The column comment
	QString d_comment;
	//! The plot designation
	AbstractDataSource::PlotDesignation d_plot_designation;

};

#endif

