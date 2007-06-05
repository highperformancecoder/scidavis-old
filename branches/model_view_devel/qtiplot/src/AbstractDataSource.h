/***************************************************************************
    File                 : AbstractDataSource.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Reading interface for column based data

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

#ifndef ABSTRACTDATASOURCE_H
#define ABSTRACTDATASOURCE_H

#include <QObject>
#include <QtAlgorithms>
#include <QList>
class QString;
#include "Interval.h"


//! Reading interface for column-based data
/**
  This is the abstract base class for column-based data, 
  i.e. mathematically a 1D vector or technically a 1D array or list.
  It only defines the reading interface and has no data members itself. 
  The writing interface is defined in AbstractColumnData and
  classes derived from it.

  An object of a class derived from this is used as a column 
  in a table or as a data source for plots, filters and fit functions.
  This class defines all non-specific read functions and signals that
  indicate a data change. Classes derived from this will store a 
  vector with entries of one certain data type, e.g. double, QString, 
  QDateTime. To determine the data type of a class derived from this, 
  use qobject_cast or QObject::inherits().

  Any class implementing writing functions must emit the according 
  signals. In some cases it will be necessary for a class using 
  data sources to connect QObject::destroyed() also, to react 
  to a column's deletion, e.g. a filter's reaction to a 
  table deletion.

  This class also defines the reading interface to additional data type 
  independent information based on intervals. This includes validity, 
  selection, masking as well as a formula string that can be associated 
  with a range of rows. In the standard implementation all rows are
  valid, none are selected, none are masked and all formulas are empty.
  */
class AbstractDataSource : public QObject
{
	Q_OBJECT

public:
	//! Destructor
	virtual ~AbstractDataSource(){};

	//! Types of plot designations
	enum PlotDesignation{
	noDesignation = 0, //!< no plot designation
	X = 1,  //!< x values
	Y = 2, //!< y values 
	Z = 3, //!< z values
	xErr = 4, //!< x errors
	yErr = 5 //!< y errors
	};

	//! Return the value in row 'row' in its string representation
	virtual QString textAt(int row) const = 0;
	//! Return the value in row 'row' as a floating point number
	/**
	 * If the value cannot be converted to a meaningful
	 * value, this will return 0.0.
	 */
	virtual double valueAt(int row) const = 0;
	//! Return the data vector size
	/**
	 * \sa AbstractColumnData::setNumRows()
	 */ 
	virtual int numRows() const = 0;
	//! Return the column label
	virtual QString label() const = 0;
	//! Return the column comment
	virtual QString comment() const { return QString(); }
	//! Return the column plot designation
	virtual AbstractDataSource::PlotDesignation plotDesignation() const = 0;
	
	//! Return whether a certain row contains a valid value
	virtual bool isRowValid(int row) const { return ( row >= 0 && row < numRows() ); }
	//! Return whether a certain interval of rows contains only valid values
	virtual bool isValid(Interval i) const { return Interval(0, numRows()-1).contains(i); }
	//! Return whether a certain row is selected
	virtual bool isSelected(int row) const { return false; }
	//! Return all selected intervals 
	virtual QList<Interval> selectedIntervals() const { return QList<Interval>(); }
	//! Return the formula associated with row 'row'
	virtual QString formula(int row) const { return QString(); }
	//! Return whether a certain row is masked
	virtual bool isRowMasked(int row) const { return false; }
	//! Return whether a certain interval of rows rows is fully masked
	virtual bool isMasked(Interval i) const { return false; }

signals: 
	//! Column label and/or comment will be changed
	/**
	 * 'source' is always the this pointer of the column that
	 * emitted this signal. This way it's easier to use
	 * one handler for lots of columns.
	 */
	void descriptionAboutToChange(AbstractDataSource * source); 
	//! Column label and/or comment changed
	/**
	 * 'source' is always the this pointer of the column that
	 * emitted this signal. This way it's easier to use
	 * one handler for lots of columns.
	 */
	void descriptionChanged(AbstractDataSource * source); 
	//! Column plot designation will be changed
	/**
	 * 'source' is always the this pointer of the column that
	 * emitted this signal. This way it's easier to use
	 * one handler for lots of columns.
	 */
	void plotDesignationAboutToChange(AbstractDataSource * source); 
	//! Column plot designation changed
	/**
	 * 'source' is always the this pointer of the column that
	 * emitted this signal. This way it's easier to use
	 * one handler for lots of columns.
	 */
	void plotDesignationChanged(AbstractDataSource * source); 
	//! Data of the column will be changed
	/**
	 * This can also mean that the column will be resized.
	 * 'source' is always the this pointer of the column that
	 * emitted this signal. This way it's easier to use
	 * one handler for lots of columns.
	 */
	void dataAboutToChange(AbstractDataSource * source); 
	//! Data of the column has changed
	/**
	 * This can also mean that the column was resized.
	 * 'source' is always the this pointer of the column that
	 * emitted this signal. This way it's easier to use
	 * one handler for lots of columns.
	 */
	void dataChanged(AbstractDataSource * source); 
	//! The column will be replaced
	/**
	 * This is used then a column is replaced by another
	 * column, possibly of another type. This is
	 * necessary because changing a column's type
	 * cannot be done without changing the pointer
	 * to the data source.
	 *
	 * \param new_col Pointer to the column this one is to be replaced with.
	 *
	 * \param source is always a pointer to the column that
	 * emitted this signal. This way it's easier to use
	 * one handler for lots of columns.
	 */
	void aboutToBeReplaced(AbstractDataSource * source, AbstractDataSource * new_col); 

};

#endif
