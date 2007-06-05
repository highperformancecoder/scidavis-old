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

#include "AbstractDataSource.h"
#include <QStringList>

//! Base class for data sources supporting interval-based attributes
/**
  This class implements data type independent additional information
  based on intervals. This includes validity, selection, masking as
  well as a formula string that can be associated with a range of rows.
  */

class DataSourceAttributes : public AbstractDataSource
{
public:
	// \name Interval based information handling (reading functions)
	//@{
	//! Return whether a certain row contains a valid value
	virtual bool isRowValid(int row) const;
	//! Return whether a certain interval of rows contains only valid values
	virtual bool isValid(Interval i) const;
	//! Return whether a certain row is selected
	virtual bool isSelected(int row) const;
	//! Return all selected intervals 
	virtual QList<Interval> selectedIntervals() const;
	//! Return the formula associated with row 'row'
	QString formula(int row) const;
	//! Return whether a certain row is masked
	virtual bool isRowMasked(int row) const;
	//! Return whether a certain interval of rows rows is fully masked
	virtual bool isMasked(Interval i) const;
	//@}
	
	// \name Interval based information handling (writing functions)
	//@{
	//! Clear all validity information
	virtual void clearValidity();
	//! Clear all selection information
	virtual void clearSelections();
	//! Clear all masking information
	virtual void clearMasks();
	//! Clear all formulas
	virtual void clearFormulas();
	//! Set an interval valid or invalid
	/**
	 * \param i the interval
	 * \param valid true: set valid, false: set invalid
	 */ 
	virtual void setValid(Interval i, bool valid = true);
	//! Select of deselect a certain interval
	/**
	 * \param i the interval
	 * \param select true: select, false: deselect
	 */ 
	virtual void setSelected(Interval i, bool select = true);
	//! Set an interval masked
	/**
	 * \param i the interval
	 * \param mask true: mask, false: unmask
	 */ 
	virtual void setMasked(Interval i, bool mask = true);
	//! Set a forumla string for an interval of rows
	virtual void setFormula(Interval i, QString formula);
	//@}

private:
	QList<Interval> d_valid_intervals;
	QList<Interval> d_selected_intervals;
	QList<Interval> d_formula_intervals;
	QStringList d_formulas;
	QList<Interval> d_masked_intervals;
	
};

#endif
