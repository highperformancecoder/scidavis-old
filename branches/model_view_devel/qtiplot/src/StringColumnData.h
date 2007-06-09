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
class StringColumnData : public AbstractStringDataSource, public AbstractColumnData, public QStringList
{
	Q_OBJECT

public:
	//! Dtor
	virtual ~StringColumnData(){};

	//! Ctor
	StringColumnData();
	//! Ctor
	StringColumnData(const QStringList& list);

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

	//! \name IntervalAttribute related reading functions
	//@{
	//! Return whether a certain row contains a valid value 	 
	virtual bool isValid(int row) const { return d_validity.isSet(row); } 	 
	//! Return whether a certain interval of rows contains only valid values 	 
	virtual bool isValid(Interval<> i) const { return d_validity.isSet(i); } 	 
	//! Return all intervals of valid rows
	virtual QList< Interval<> > validIntervals() const { return d_validity.intervals(); } 	 
	//! Return whether a certain row is selected 	 
	virtual bool isSelected(int row) const { return d_selection.isSet(row); } 	 
	//! Return whether a certain interval of rows is fully selected
	virtual bool isSelected(Interval<> i) const { return d_selection.isSet(i); } 	 
	//! Return all selected intervals 	 
	virtual QList< Interval<> > selectedIntervals() const { return d_selection.intervals(); } 	 
	//! Return whether a certain row is masked 	 
	virtual bool isMasked(int row) const { return d_masking.isSet(row); } 	 
	//! Return whether a certain interval of rows rows is fully masked 	 
	virtual bool isMasked(Interval<> i) const { return d_masking.isSet(i); }
	//! Return all intervals of masked rows
	virtual QList< Interval<> > maskedIntervals() const { return d_masking.intervals(); } 	 
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
	virtual void setValid(Interval<> i, bool valid = true)
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
	virtual void setSelected(Interval<> i, bool select = true)
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
	virtual void setMasked(Interval<> i, bool mask = true)
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
	//! The column label
	QString d_label;
	//! The column comment
	QString d_comment;
	//! The plot designation
	AbstractDataSource::PlotDesignation d_plot_designation;

};

#endif

