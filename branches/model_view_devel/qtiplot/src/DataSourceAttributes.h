/***************************************************************************
    File                 : DataSourceAttributes.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Base class for data sources supporting interval-based attributes
                           
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

#ifndef DATASOURCEATTRIBUTES_H
#define DATASOURCEATTRIBUTES_H

#include "IntervalAttribute.h"
#include <QObject>
class AbstractDataSource;

//! Base class for data sources supporting interval-based attributes
/**
  This class implements data type independent column information
  based on intervals. This includes validity, selection, masking as
  well as a formula string that can be associated with a range of rows.
  */

class DataSourceAttributes : public QObject
{
	Q_OBJECT

public:
	// \name Interval based information handling (reading functions)
	//@{
	//! Return whether a certain row contains a valid value
	bool isValid(int row) const;
	//! Return whether a certain interval of rows contains only valid values
	bool isValid(Interval i) const;
	//! Return whether a certain row is selected
	bool isSelected(int row) const;
	//! Return all selected intervals 
	QList<Interval> selectedIntervals() const;
	//! Return the formula associated with row 'row'
	QString formula(int row) const;
	//! Return whether a certain row is masked
	bool isMasked(int row) const;
	//! Return whether a certain interval of rows rows is fully masked
	bool isMasked(Interval i) const;
	//@}
	
	// \name Interval based information handling (writing functions)
	//@{
	//! Clear all validity information
	void clearValidity();
	//! Clear all selection information
	void clearSelections();
	//! Clear all masking information
	void clearMasks();
	//! Clear all formulas
	void clearFormulas();
	//! Set an interval valid or invalid
	/**
	 * \param i the interval
	 * \param valid true: set valid, false: set invalid
	 */ 
	void setValid(Interval i, bool valid = true);
	//! Select of deselect a certain interval
	/**
	 * \param i the interval
	 * \param select true: select, false: deselect
	 */ 
	void setSelected(Interval i, bool select = true);
	//! Set an interval masked
	/**
	 * \param i the interval
	 * \param mask true: mask, false: unmask
	 */ 
	void setMasked(Interval i, bool mask = true);
	//! Set a forumla string for an interval of rows
	void setFormula(Interval i, QString formula);
	//@}

public slots:
	//! Insert rows
	/**
	 *	This slot must be connected to the rowsInserted()
	 *	signal of the corresponding data source.
	 *
	 *	\param source this will be ignored
	 *	\param before the row to insert before
	 *	\param count the number of rows to be inserted
	 */
	void insertRows(AbstractDataSource * source, int before, int count); 
	//! Delete rows
	/**
	 *	This slot must be connected to the rowsDeleted()
	 *	signal of the corresponding data source.
	 *
	 *	\param source this will be ignored
	 *	\param first the first row to be deleted
	 *	\param count the number of rows to be deleted
	 */
	void deleteRows(AbstractDataSource * source, int first, int count); 


private:
	IntervalAttribute<bool> d_validity;
	IntervalAttribute<bool> d_selection;
	IntervalAttribute<bool> d_masking;
	IntervalAttribute<QString> d_formulas;
};

#endif
