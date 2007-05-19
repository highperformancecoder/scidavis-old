/***************************************************************************
    File                 : TimeColumnData.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Stores a vector of QTimes (for a table column)

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

#ifndef TIMECOLUMNDATA_H
#define TIMECOLUMNDATA_H

#include "AbstractColumnData.h"
class DoubleColumnData;
class StringColumnData;
class DateColumnData;
#include <QList>
#include <QString>
class QTime;

//! Stores a vector of QTimes (for a table column)
/**
  * This class stores a list of QTimes. All functions from QList<QTime>
  * can be used for convenient and fast access of the data. 
  * The class also stores a format string that is
  * used by cellString() to output the string representation.
  * The whole column of times can be
  * converted to strings by StringColumnData::clone().
  * \sa AbstractColumnData
  */
class TimeColumnData : public AbstractColumnData, public QList<QTime>
{
public:
	//! Dtor
	virtual ~TimeColumnData();
	//! Ctor
	TimeColumnData();
	//! Ctor
	explicit TimeColumnData(const QList<QTime>& list);

	//! Return the data type
	/**
	 * \sa AbstractColumnData::ColumnDataType
	 */
	virtual int type() const;
	//! Clone (if necessary convert) another vector of data
	/**
	 * StringColumnData: QString to QTime conversion using the format string
	 * DateColumnData: will result in a list of zero times 
	 * DoubleColumnData: converted from the digits behind the dot as 
	 * the fraction of a day, e.g. 0.5 and 1.5 will give 43200000 ms
	 * \return true if cloning was successful, otherwise false
	 * \sa format(), setFormat()
	 */
	virtual bool clone(const AbstractColumnData& other);
	//! Return value number 'index' as a string
	virtual QString cellString(int index) const;
	//! Set a cell value from a string
	/**
	 * This method is smarter than QTime::fromString()
	 * and cloneStringColumnData() as it tries 
	 * out several format strings until it 
	 * gets a valid time.
	 */
	virtual void setCellFromString(int index, const QString& string);
	//! Set the format String
	/**
	 * \sa QTime::toString()
	 */
	void setFormat(const QString& format) { d_format = format; };
	//! Return the format string
	/**
	 * The default format string is "hh:mm:ss.zzz".
	 * \sa QTime::toString()
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
	//! Convert and clone a QDate column data vector
	bool cloneDateColumnData(const DateColumnData& other);
	//! Clone another QTime column data vector
	bool cloneTimeColumnData(const TimeColumnData& other);

	QString d_format;
};

#endif
