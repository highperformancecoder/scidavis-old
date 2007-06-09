/***************************************************************************
    File                 : DoubleColumnData.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Data source that stores a vector of doubles (implementation)

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
#include "AbstractDoubleDataSource.h"
#include <QVector>

//! Data source that stores a vector of doubles (implementation)
/**
  * This class stores a vector of double precision values. 
  * It implements the interfaces defined in AbstractColumnData, 
  * AbstractDataSource, and AbstractDoubleDataSource as well as the data type specific
  * writing functions. The stored data can also be accessed by
  * the functions of QVector\<double\>.
  * \sa AbstractColumnData
  * \sa AbstractDataSource
  * \sa AbstractDoubleDataSource
  */
class DoubleColumnData : public AbstractDoubleDataSource, public AbstractColumnData, public QVector<double>
{ 
	Q_OBJECT

public:
	//! Ctor
	explicit DoubleColumnData(int size = 0);
	//! Dtor
	virtual ~DoubleColumnData(){};

	//! \name Data writing functions
	//@{
	//! Copy (if necessary convert) another vector of data
	/**
	 * StringColumnData: normal string to double conversion
	 * DateColumnData: converted into the number of days relative to 1900/1/1
	 * TimeColumnData: converted to the fraction of a day 
	 * \return true if copying was successful, otherwise false
	 */
	virtual bool copy(const AbstractDataSource * other);
	//! Set format character as in QString::number
	virtual void setNumericFormat(char format);
	//! Set number of displayed digits
	virtual void setDisplayedDigits(int digits);
	//! Set a row value from a string
	virtual void setRowFromString(int row, const QString& string);
	//! Resize the data vector
	virtual void setNumRows(int new_size);
	//! Set the column label
	virtual void setLabel(const QString& label);
	//! Set the column comment
	virtual void setComment(const QString& comment);
	//! Set the column plot designation
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
	//! Return the vector size
	virtual int numRows() const;
	//! Return the value in row 'row' in its string representation
	virtual QString textAt(int row) const;
	//! Return the value in row 'row'
	virtual double valueAt(int row) const;
	//! Return the column label
	virtual QString label() const;
	//! Return the column comment
	virtual QString comment() const;
	//! Return the column plot designation
	virtual AbstractDataSource::PlotDesignation plotDesignation() const;
	//! Return format character as in QString::number
	virtual char numericFormat() const;
	//! Return the number of displayed digits
	virtual int displayedDigits() const;
	//! Return a read-only array pointer for fast data access
	/**
	 * The pointer remains valid as long as the vector is 
	 * not resized. When it is resized it will emit
	 * a AbstractDataSource::dataChanged() signal.
	 */ 
	virtual const double * constDataPointer() const;
	//@}

	//! \name IntervalAttribute related reading functions
	//@{
	//! Return whether a certain row contains a valid value 	 
	virtual bool isValid(int row) const { return d_validity.isSet(row); } 	 
	//! Return whether a certain interval of rows contains only valid values 	 
	virtual bool isValid(Interval<int> i) const { return d_validity.isSet(i); } 	 
	//! Return all intervals of valid rows
	virtual QList< Interval<int> > validIntervals() const { return d_validity.intervals(); } 	 
	//! Return whether a certain row is selected 	 
	virtual bool isSelected(int row) const { return d_selection.isSet(row); } 	 
	//! Return whether a certain interval of rows is fully selected
	virtual bool isSelected(Interval<int> i) const { return d_selection.isSet(i); } 	 
	//! Return all selected intervals 	 
	virtual QList< Interval<int> > selectedIntervals() const { return d_selection.intervals(); } 	 
	//! Return whether a certain row is masked 	 
	virtual bool isMasked(int row) const { return d_masking.isSet(row); } 	 
	//! Return whether a certain interval of rows rows is fully masked 	 
	virtual bool isMasked(Interval<int> i) const { return d_masking.isSet(i); }
	//! Return all intervals of masked rows
	virtual QList< Interval<int> > maskedIntervals() const { return d_masking.intervals(); } 	 
	//@}
	
	//! \name IntervalAttribute related writing functions
	//@{
	//! Clear all validity information
	virtual void clearValidity()
	{
		emit validityAboutToChange(this);	
		d_validity.clear();
		emit validityChanged(this);	
	}
	//! Clear all selection information
	virtual void clearSelections()
	{
		emit selectionAboutToChange(this);	
		d_selection.clear();
		emit selectionChanged(this);	
	}
	//! Clear all masking information
	virtual void clearMasks()
	{
		emit maskingAboutToChange(this);	
		d_masking.clear();
		emit maskingChanged(this);	
	}
	//! Set an interval valid or invalid
	/**
	 * \param i the interval
	 * \param valid true: set valid, false: set invalid
	 */ 
	virtual void setValid(Interval<int> i, bool valid = true)
	{
		emit validityAboutToChange(this);	
		d_validity.setValue(i, valid);
		emit validityChanged(this);	
	}
	//! Select of deselect a certain interval
	/**
	 * \param i the interval
	 * \param select true: select, false: deselect
	 */ 
	virtual void setSelected(Interval<int> i, bool select = true)
	{
		emit selectionAboutToChange(this);	
		d_selection.setValue(i, select);
		emit selectionChanged(this);	
	}
	//! Set an interval masked
	/**
	 * \param i the interval
	 * \param mask true: mask, false: unmask
	 */ 
	virtual void setMasked(Interval<int> i, bool mask = true)
	{
		emit maskingAboutToChange(this);	
		d_masking.setValue(i, mask);
		emit maskingChanged(this);	
	}
	//@}
	
protected:
	IntervalAttribute<bool> d_validity;
	IntervalAttribute<bool> d_selection;
	IntervalAttribute<bool> d_masking;
	//! Format character as in QString::number 
	char d_numeric_format;
	//! Display digits or precision as in QString::number  
	/**
	 * This has nothing to do with the internal precision. 
	 * Internally, everything is always handled using the full
	 * precision of the data type 'double'.
	 */
	int d_displayed_digits;
	//! The column label
	QString d_label;
	//! The column comment
	QString d_comment;
	//! The plot designation
	AbstractDataSource::PlotDesignation d_plot_designation;

};

#endif
