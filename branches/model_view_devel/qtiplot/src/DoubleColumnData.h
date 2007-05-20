/***************************************************************************
    File                 : DoubleColumnData.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Stores a vector of doubles (for a table column)

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

#ifndef DOUBLECOLUMNDATA_H
#define DOUBLECOLUMNDATA_H

#include "AbstractColumnData.h"
class StringColumnData;
class DateColumnData;
class TimeColumnData;
#include <QVector>

//! Stores a vector of doubles (for a table column)
/**
  * This class stores a one-dimensional array of double precision
  * values. All functions from QVector can be used for convenient
  * and fast access of the data. The class also stores the format
  * code as well as the number of displayed decimal digits for
  * string output. Single cells can be output as a string using
  * DoubleColumnData::cellString() and the whole column can be
  * converted to strings by StringColumnData::clone().
  * \sa AbstractColumnData
  */
class DoubleColumnData : public AbstractColumnData, public QVector<double>
{
public:
	//! Ctor
	explicit DoubleColumnData(int size = 0);
	//! Dtor
	virtual ~DoubleColumnData();

	//! Return the data type
	/**
	 * \sa AbstractColumnData::ColumnDataType
	 */
	virtual AbstractColumnData::ColumnDataType type() const;
	//! Copy (if necessary convert) another vector of data
	/**
	 * StringColumnData: normal string to double conversion
	 * DateColumnData: converted into the number of days relative to 1900/1/1
	 * TimeColumnData: converted to the fraction of a day 
	 * \return true if cloning was successful, otherwise false
	 */
	virtual bool clone(const AbstractColumnData& other);
	//! Set format character as in QString::number
	void setNumericFormat(char format) { d_numeric_format = format; };
	//! Set number of displayed digits
	void setDisplayedDigits(int digits) { d_displayed_digits = digits; };
	//! Return format character as in QString::number
	char numericFormat() const { return d_numeric_format; };
	//! Return the number of displayed digits
	int displayedDigits() const { return d_displayed_digits; };
	//! Return value number 'index' as a string
	virtual QString cellString(int index) const;
	//! Set a cell value from a string
	virtual void setCellFromString(int index, const QString& string);
	//! Resize the data vector
	virtual void resize(int new_size);
	//! Return the data vector size
	virtual int size() const;

private:
	//! Copy another double column data vector
	bool cloneDoubleColumnData(const DoubleColumnData& other);
	//! Convert and copy a QString column data vector
	bool cloneStringColumnData(const StringColumnData& other);
	//! Convert and copy a QDate column data vector
	bool cloneDateColumnData(const DateColumnData& other);
	//! Convert and copy a QTime column data vector
	bool cloneTimeColumnData(const TimeColumnData& other);

	//! Format character as in QString::number 
	char d_numeric_format;
	//! Display digits or precision as in QString::number  
	int d_displayed_digits;
};

#endif
