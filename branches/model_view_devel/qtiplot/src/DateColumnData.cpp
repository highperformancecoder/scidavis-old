/***************************************************************************
    File                 : DateColumnData.cpp
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Stores a vector of QDates (for a table column)

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

#include "DateColumnData.h"
#include "DoubleColumnData.h"
#include "StringColumnData.h"
#include "TimeColumnData.h"
#include <QDate>
#include <QTime>

DateColumnData::DateColumnData()
{
	d_format = "yyyy-MM-dd";
}

DateColumnData::DateColumnData(const QList<QDate>& list)
{
	d_format = "yyyy-MM-dd";
	*(static_cast< QList<QDate>* >(this)) = list;
}

DateColumnData::~DateColumnData()
{
}

int DateColumnData::type() const
{
	return AbstractColumnData::Date;
}

bool DateColumnData::clone(const AbstractColumnData& other)
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
	
bool DateColumnData::cloneDoubleColumnData(const DoubleColumnData& other)
{ 
 	clear();

	int end = other.size();
	QDate ref_date = QDate(1900,1,1);
	for(int i=0; i<end; i++)
		*this << ref_date.addDays(int(other.at(i)));
	return true;
}

bool DateColumnData::cloneDateColumnData(const DateColumnData& other)
{ 
	*(static_cast< QList<QDate>* >(this)) = static_cast< const QList<QDate>& >(other);
	return true;
}

bool DateColumnData::cloneTimeColumnData(const TimeColumnData& other)
{  
 	clear();

	int end = other.size();
	for(int i=0; i<end; i++)
		*this << QDate(1900,1,1);
	// converting QTime to QDate does not make much sense
	// therefore this will result in a list of 1900/1/1
	// and will return false since the column information is lost
	return false;
}

bool DateColumnData::cloneStringColumnData(const StringColumnData& other)
{ 
 	clear();

	int end = other.size();
	for(int i=0; i<end; i++)	
		*this << QDate::fromString(other.at(i), d_format);
	return true;
}

QString DateColumnData::cellString(int index) const
{
	if(index < size())
		return at(index).toString(d_format);
	else
		return QString();
}

void DateColumnData::setCellFromString(int index, const QString& string)
{
	(*(static_cast< QList<QDate>* >(this)))[index] = QDate::fromString(string, d_format);
}

void DateColumnData::resize(int new_size)
{
	int old_size = size();
	int count = new_size - old_size;
	if(count == 0) return;

	if(count > 0)
	{
		for(int i=0; i<count; i++)
			*this << QDate();
	}
	else // count < 0
	{
		for(int i=0; i>count; i--)
			removeLast();
		
	}
}

int DateColumnData::size() const
{
	return QList<QDate>::size();
}

