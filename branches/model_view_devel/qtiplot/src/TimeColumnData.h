/***************************************************************************
    File                 : TimeColumnData.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Data source that stores a list of QTimes (write functions)

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
#include "AbstractTimeDataSource.h"
#include <QList>
#include <QTime>
class AbstractDoubleDataSource;
class AbstractStringDataSource;
class AbstractDateDataSource;

//! Data source that stores a list of QTimes (implementation)
/**
  * This class stores a list of QTimes. It implements the
  * interfaces defined in AbstractColumnData, AbstractDataSource,
  * and AbstractTimeDataSource as well as the data type specific
  * writing functions. The stored data can also be accessed by
  * the functions of QList\<QTime\>.
  * \sa AbstractColumnData
  * \sa AbstractDataSource
  * \sa AbstractTimeDataSource
  */
class TimeColumnData : public AbstractColumnData, public AbstractTimeDataSource, public QList<QTime>
{
	Q_OBJECT

public:
	//! Dtor
	virtual ~TimeColumnData(){};
	//! Ctor
	TimeColumnData();
	//! Ctor
	explicit TimeColumnData(const QList<QTime>& list);

	//! \name Data writing functions
	//@{
	//! Copy (if necessary convert) another vector of data
	/**
	 * StringColumnData: QString to QTime conversion using setRowFromString()
	 * DateColumnData: will result in a list of zero times 
	 * DoubleColumnData: converted from the digits behind the dot as 
	 * the fraction of a day, e.g. 0.5 and 1.5 will give 43200000 ms
	 * \return true if copying was successful, otherwise false
	 * \sa format(), setFormat()
	 */
	virtual bool copy(const AbstractDataSource * other);
	//! Set a row value from a string
	/**
	 * This method is smarter than QTime::fromString()
	 * as it tries out several format strings until it 
	 * gets a valid time.
	 */
	virtual void setRowFromString(int row, const QString& string);
	//! Set the format String
	/**
	 * The default format string is "hh:mm:ss.zzz".
	 * \sa QTime::toString()
	 */
	virtual void setFormat(const QString& format);
	//! Resize the date list
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
	//! Return the column label
	virtual QString label() const;
	//! Return the column comment
	virtual QString comment() const;
	//! Return the column plot designation
	virtual AbstractDataSource::PlotDesignation plotDesignation() const;
	//! Return the value in row 'row' in its string representation
	virtual QString rowString(int row) const;
	//! Return the value in row 'row' as a double
	/**
	 * This returns the fraction of a full day.
	 */ 
	virtual double rowValue(int row) const;
	//! Return the format string
	/**
	 * The default format string is "hh:mm:ss.zzz".
	 * \sa QTime::toString()
	 */
	virtual QString format() const;
	//! Return the list size
	virtual int numRows() const;
	//! Return the date in row 'row'
	virtual QTime timeAt(int row) const;
	//@}

protected:
	//! The column label
	QString d_label;
	//! The column comment
	QString d_comment;
	//! The plot designation
	AbstractDataSource::PlotDesignation d_plot_designation;
	//! Format string for QTime::toString()
	QString d_format;

	//! Convert and copy a double column data source
	bool copyDoubleDataSource(const AbstractDoubleDataSource * other);
	//! Convert and copy a string column data source
	bool copyStringDataSource(const AbstractStringDataSource * other);
	//! Convert and copy a QDate column data source
	bool copyDateDataSource(const AbstractDateDataSource * other);
	//! Copy another QTime column data source
	bool copyTimeDataSource(const AbstractTimeDataSource * other);

};

#endif

