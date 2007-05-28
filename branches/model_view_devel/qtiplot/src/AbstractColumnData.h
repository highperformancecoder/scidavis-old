/***************************************************************************
    File                 : AbstractColumnData.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Writing interface for column based data

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

#ifndef ABSTRACTCOLUMNDATA_H
#define ABSTRACTCOLUMNDATA_H

#include "AbstractDataSource.h"
class QString;

//! Writing interface for column based data
/**
  This is an abstract base class for column-based data, 
  i.e. mathematically a 1D vector or technically a 1D array or list.
  It only defines the writing interface and has no data members itself. 
  The reading interface is defined in AbstractDataSource and
  classes derived from it.<br>
  An object of a class derived from this is used as a column 
  in a table or as a data source for plots, filters and fit functions.
  In the latter case the filter/fit function must display the 
  data in a table that is read-only to the user or at least 
  does not support undo/redo. If an object of a class derived 
  from this is controlled by an object that supports undo, this 
  interface must only be avaiable to the commands (objects of a 
  class derived from QUndoCommand) issued from that object. 
  This is for example the case for Table.<br>
  This class defines all non-specific write functions. Classes derived from 
  this will store a vector with entries of one certain data type, 
  e.g. double, QString,  QDate. To determine the data type of a 
  class derived from this, use qobject_cast or QObject::inherits().<br>
  */
class AbstractColumnData : public QObject
{
	Q_OBJECT

public:
	//! Dtor
	virtual ~AbstractColumnData(){};

	//! Copy (if necessary convert) another vector of data
	/**
	 * \return true if copying was successful, otherwise false
	 * False means the column hast been filled with a
	 * standard value in some or all rows and some or
	 * all data could not be converted to the stored data type.
	 */
	virtual bool copy(const AbstractDataSource * other) = 0;
	//! Set a row value from a string
	virtual void setRowFromString(int row, const QString& string) = 0;
	//! Resize the data vector
	virtual void setNumRows(int new_size) = 0;
	//! Set the column label
	virtual void setLabel(const QString& label) = 0; 
	//! Set the column comment
	virtual void setComment(const QString& comment) = 0;
	//! Set the column plot designation
	/**
	 * Don't ever use AbstractDataSource::All here!
	 */
	virtual void setPlotDesignation(AbstractDataSource::PlotDesignation pd) = 0;
	//! Insert some empty (or initialized with zero) rows
	virtual void insertEmptyRows(int before, int count) = 0;
	//! Remove 'count' rows starting from row 'first'
	virtual void removeRows(int first, int count) = 0;
	//! This must be called before the column is replaced by another
	virtual void notifyReplacement(AbstractDataSource * replacement) = 0;

};

#endif
