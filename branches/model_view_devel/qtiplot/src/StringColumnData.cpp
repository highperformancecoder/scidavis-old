/***************************************************************************
    File                 : StringColumnData.cpp
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Stores a vector of strings (for a table column)

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

#include "StringColumnData.h"
#include "DoubleColumnData.h"
#include "DateColumnData.h"
#include "TimeColumnData.h"

StringColumnData::StringColumnData()
{
}

StringColumnData::StringColumnData(const QStringList& list)
{
	*(static_cast< QStringList* >(this)) = list;
}

StringColumnData::~StringColumnData()
{
}

int StringColumnData::type() const
{
	return AbstractColumnData::String;
}

bool StringColumnData::clone(const AbstractColumnData& other)
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
	
bool StringColumnData::cloneDoubleColumnData(const DoubleColumnData& other)
{ 
 	clear();

	int end = other.size();
	for(int i=0; i<end; i++)	
		*this << other.cellString(i);
	return true;
}

bool StringColumnData::cloneStringColumnData(const StringColumnData& other)
{ 
	*(static_cast< QStringList* >(this)) = static_cast< const QStringList& >(other);
	return true;
}

bool StringColumnData::cloneDateColumnData(const DateColumnData& other)
{ 
 	clear();

	int end = other.size();
	for(int i=0; i<end; i++)	
		*this << other.cellString(i);
	return true;
}

bool StringColumnData::cloneTimeColumnData(const TimeColumnData& other)
{ 
 	clear();

	int end = other.size();
	for(int i=0; i<end; i++)	
		*this << other.cellString(i);
	return true;
}

QString StringColumnData::cellString(int index) const
{
	if(index < size())
		return at(index);
	else
		return QString();
}

void StringColumnData::setCellFromString(int index, const QString& string)
{
	(*(static_cast< QStringList* >(this)))[index] = string;
}

void StringColumnData::resize(int new_size)
{
	int old_size = size();
	int count = new_size - old_size;
	if(count == 0) return;

	if(count > 0)
	{
		for(int i=0; i<count; i++)
			*this << QString();
	}
	else // count < 0
	{
		for(int i=0; i>count; i--)
			removeLast();
		
	}
}

int StringColumnData::size() const
{
	return QStringList::size();
}
