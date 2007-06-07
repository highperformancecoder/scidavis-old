/***************************************************************************
    File                 : DataSourceAttributes.cpp
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

#include "DataSourceAttributes.h"

bool DataSourceAttributes::isValid(int row) const 
{ 
	return d_validity.isSet(row);
}

bool DataSourceAttributes::isValid(Interval i) const
{ 
	return d_validity.isSet(i);
}

bool DataSourceAttributes::isSelected(int row) const 
{ 
	return d_selection.isSet(row);
}

QList<Interval> DataSourceAttributes::selectedIntervals() const 
{ 
	return d_selection.intervals(); 
}

QString DataSourceAttributes::formula(int row) const 
{ 
	return d_formulas.value(row);
}

bool DataSourceAttributes::isMasked(int row) const
{ 
	return d_masking.isSet(row);
}

bool DataSourceAttributes::isMasked(Interval i) const
{ 
	return d_masking.isSet(i);
}

void DataSourceAttributes::clearValidity() 
{ 
	d_validity.clear(); 
}

void DataSourceAttributes::clearSelections() 
{ 
	d_selection.clear(); 
}

void DataSourceAttributes::clearMasks() 
{ 
	d_masking.clear(); 
}

void DataSourceAttributes::clearFormulas() 
{ 
	d_formulas.clear(); 
}

void DataSourceAttributes::setValid(Interval i, bool valid)
{ 
	d_validity.setValue(i, valid);
}

void DataSourceAttributes::setSelected(Interval i, bool select)
{ 
	d_selection.setValue(i, select);
}

void DataSourceAttributes::setMasked(Interval i, bool mask)
{ 
	d_selection.setValue(i, mask);
}

void DataSourceAttributes::setFormula(Interval i, QString formula)
{
	d_formulas.setValue(i, formula);
}

void DataSourceAttributes::insertRows(AbstractDataSource * source, int before, int count)
{
	Q_UNUSED(source);
	d_validity.insertRows(before, count);
	d_selection.insertRows(before, count);
	d_masking.insertRows(before, count);
	d_formulas.insertRows(before, count);
}

void DataSourceAttributes::deleteRows(AbstractDataSource * source, int first, int count)
{
	Q_UNUSED(source);
	d_validity.deleteRows(first, count);
	d_selection.deleteRows(first, count);
	d_masking.deleteRows(first, count);
	d_formulas.deleteRows(first, count);
}


