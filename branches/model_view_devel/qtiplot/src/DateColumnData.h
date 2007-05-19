/***************************************************************************
    File                 : DateColumnData.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Stores a vector of QDates (for a table column)

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

#ifndef DATECOLUMNDATA_H
#define DATECOLUMNDATA_H

#include "AbstractColumnData.h"
class DoubleColumnData;
class StringColumnData;
class TimeColumnData;
#include <QList>
#include <QString>
class QDate;


//! Stores a vector of QDates (for a table column)
/**
  * This class stores a list of QDate. All functions from QList<QDate>
  * can be used for convenient and fast access of the data. 
  * The class also stores a format string that is
  * used by cellString() to output the string representation.
  * The whole column of times can be
  * converted to strings by StringColumnData::clone().
  * \sa AbstractColumnData
  */
class DateColumnData : public AbstractColumnData, public QList<QDate>
{
public:
	//! Dtor
	virtual ~DateColumnData();
	//! Ctor
	DateColumnData();
	//! Ctor
	explicit DateColumnData(const QList<QDate>& list);

	//! Return the data type
	/**
	 * \sa AbstractColumnData::ColumnDataType
	 */
	virtual int type() const;
	//! Clone (if necessary convert) another vector of data
	/**
	 * StringColumnData: QString to QDate conversion using the format string
	 * TimeColumnData: will result in a list of 1900/1/1 dates
	 * DoubleColumnData: converted from the digits before the dot as 
	 * as days relative to 1900/1/1, e.g. 5.2 will give 1900/1/6.
	 * \return true if cloning was successful, otherwise false
	 * \sa format(), setFormat()
	 */
	virtual bool clone(const AbstractColumnData& other);
	//! Return value number 'index' as a string
	virtual QString cellString(int index) const;
	//! Set a cell value from a string
	/**
	 * This method is smarter than QDate::fromString()
	 * and cloneStringColumnData() as it tries 
	 * out several format strings until it 
	 * gets a valid date.
	 */
	virtual void setCellFromString(int index, const QString& string);
	//! Set the format String
	/**
	 * \sa QDate::toString()
	 */
	void setFormat(const QString& format) { d_format = format; };
	//! Return the format string
	/**
	 * The default format string is "yyyy-MM-dd".
	 * \sa QDate::toString()
	 */
	QString format() const { return d_format; };
	//! Resize the data vector
	virtual void resize(int new_size);
	//! Return the data vector size
	virtual int size() const;

private:
	//! Convert and clone a double column data vector
	bool cloneDoubleColumnData(const DoubleColumnData& other);
	//! Convert and clone a string column data vector
	bool cloneStringColumnData(const StringColumnData& other);
	//! Clone another QDate column data vector
	bool cloneDateColumnData(const DateColumnData& other);
	//! Convert and clone a QTime column data vector
	bool cloneTimeColumnData(const TimeColumnData& other);

	QString d_format;

};

#endif
