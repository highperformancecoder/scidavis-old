/***************************************************************************
    File                 : AbstractColumnData.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Abstract base class for column data

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

//#include "Table.h"
#include <QString>

//! Abstract base class for column data
/**
  This is the abstract base class for all classes
  that are to store the data for a column in a table.
  Classes derived from this will store a vector with entries
  of one certain data type, e.g. double, QString, QDate.
  */
class AbstractColumnData
{
public:
	//! Destructor
	virtual ~AbstractColumnData(){};

	//! Emuneration of all possible data type in derived classes
	enum ColumnDataType{Double = 0, //!< store double values
		String = 1, //!< store QString values
		Date = 2, //!< store QDate values
		Time = 3 //!< store QTime values
		};

	//! Return the data type
	/**
	 * \sa ColumnDataType
	 */
	virtual AbstractColumnData::ColumnDataType type() const = 0;
	//! Copy (if necessary convert) another vector of data
	/**
	 * \return true if cloning was successful, otherwise false
	 */
	virtual bool clone(const AbstractColumnData& other) = 0;
	//! Return value number 'index' in its string representation
	virtual QString cellString(int index) const = 0;
	//! Set a cell value from a string
	virtual void setCellFromString(int index, const QString& string) = 0;
	//! Resize the data vector
	virtual void resize(int new_size) = 0;
	//! Return the data vector size
	virtual int size() const = 0;
	//! Set the column label
	void setLabel(const QString& label) { d_label = label; };
	//! Return the column label
	QString label() const { return d_label; };
	//! Set the column comment
	void setComment(const QString& comment) { d_comment = comment; };
	//! Return the column comment
	QString comment() const { return d_comment; };
	//! Set the column plot designation
	/**
	 * Don't ever use Table::All here!
	 */
//	void setPlotDesignation(Table::PlotDesignation pd) { d_plot_designation = pd; };
	//! Return the column plot designation
//	QString plotDesignation() const { return d_plot_designation; };

protected:
	//! The column label
	QString d_label;
	//! The column comment
	QString d_comment;
	//! The plot designation
//	Table::PlotDesignation d_plot_designation;

};

#endif
