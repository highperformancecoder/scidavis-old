/***************************************************************************
    File                 : StringColumnData.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Stores a vector of strings (for a table column)

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
class DoubleColumnData;
class DateColumnData;
class TimeColumnData;
#include <QStringList>

//! Stores a vector of strings (for a table column)
/**
  * This class stores a list of QStrings. All functions from QStringList
  * can be used for convenient and fast access of the data. 
  * For consistency with other column data classes indvidual strings
  * can also be read by using StringColumnData::cellString().
  * All other types of column data can be converted to a
  * column of the according string representation
  * by StringColumnData::clone().
  * \sa AbstractColumnData
  */
class StringColumnData : public AbstractColumnData, public QStringList
{
public:
	//! Dtor
	virtual ~StringColumnData();

	//! Ctor
	explicit StringColumnData();
	//! Ctor
	explicit StringColumnData(const QStringList& list);

	//! Return the data type
	/**
	 * \sa AbstractColumnData::ColumnDataType
	 */
	virtual int type() const;
	//! Clone (if necessary convert) another vector of data
	/**
	 * \return true if cloning was successful, otherwise false
	 */
	virtual bool clone(const AbstractColumnData& other);
	//! Return string number 'index'
	virtual QString cellString(int index) const;
	//! Set a cell value from a string
	virtual void setCellFromString(int index, const QString& string);
	//! Resize the data vector
	virtual void resize(int new_size);
	//! Return the data vector size
	virtual int size() const;

private:
	//! Convert and clone a double column data vector
	bool cloneDoubleColumnData(const DoubleColumnData& other);
	//! Clone another string column data vector
	bool cloneStringColumnData(const StringColumnData& other);
	//! Convert and clone a QDate column data vector
	bool cloneDateColumnData(const DateColumnData& other);
	//! Convert and clone a QTime column data vector
	bool cloneTimeColumnData(const TimeColumnData& other);
};

#endif
