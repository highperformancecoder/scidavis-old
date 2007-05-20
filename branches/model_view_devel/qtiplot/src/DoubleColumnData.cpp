/***************************************************************************
    File                 : DoubleColumnData.cpp
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

#include "DoubleColumnData.h"
#include "StringColumnData.h"
#include "DateColumnData.h"
#include "TimeColumnData.h"
#include <QDate>
#include <QTime>

DoubleColumnData::DoubleColumnData(int size)
: QVector<double>(size)
{
	d_numeric_format = 'e';
	d_displayed_digits = 6;
}

DoubleColumnData::~DoubleColumnData()
{
}

AbstractColumnData::ColumnDataType DoubleColumnData::type() const
{
	return AbstractColumnData::Double;
}

bool DoubleColumnData::clone(const AbstractColumnData& other)
{
	switch(other.type())
	{
		case AbstractColumnData::Double:
			return cloneDoubleColumnData((DoubleColumnData &)other);

		case AbstractColumnData::String:
			return cloneStringColumnData((StringColumnData &)other);

		case AbstractColumnData::Date:
			return cloneDateColumnData((DateColumnData &)other);

		case AbstractColumnData::Time:
			return cloneTimeColumnData((TimeColumnData &)other);
	
		default:
			return false;
	}
}
	
bool DoubleColumnData::cloneDoubleColumnData(const DoubleColumnData& other)
{
	*(static_cast< QVector<double> * >(this)) = static_cast< QVector<double> >(other);
	d_numeric_format = other.numericFormat();
	d_displayed_digits = other.displayedDigits();
	return true;
}

bool DoubleColumnData::cloneStringColumnData(const StringColumnData& other)
{ 
	resize(other.size());
 	
	int end = size();
	bool ok, result = true;
	double * rawdata = data();
	for(int i=0; i<end; i++)  
	{
    	rawdata[i] = other[i].toDouble(&ok);
		if(!ok)
			result = false;
	}
	return result;
}

bool DoubleColumnData::cloneDateColumnData(const DateColumnData& other)
{ 
	resize(other.size());
 	
	int end = size();
	double * rawdata = data();
	QDate ref_date(1900, 1, 1);
	for(int i=0; i<end; i++)  
    	rawdata[i] = double(- other[i].daysTo(ref_date));
	return true;
}

bool DoubleColumnData::cloneTimeColumnData(const TimeColumnData& other)
{ 
	resize(other.size());
 	
	int end = size();
	double * rawdata = data();
	QTime ref_time(0,0,0,0);
	for(int i=0; i<end; i++)  
    	rawdata[i] = double(- other[i].msecsTo(ref_time))/86400000.0;
	return true;
}


QString DoubleColumnData::cellString(int index) const
{
	if(index < size())
		return QString::number(at(index), d_numeric_format, d_displayed_digits);
	else
		return QString();
}

void DoubleColumnData::setCellFromString(int index, const QString& string)
{
    data()[index] = string.toDouble();
}


void DoubleColumnData::resize(int new_size)
{
	QVector<double>::resize(new_size);
}


int DoubleColumnData::size() const
{
	return QVector<double>::size();
}

