/***************************************************************************
    File                 : TimeColumnData.cpp
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Stores a vector of QTimes (for a table column)

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

#include "TimeColumnData.h"
#include "DoubleColumnData.h"
#include "DateColumnData.h"
#include "StringColumnData.h"
#include <QTime>
#include <QDate>
#include <QString>

TimeColumnData::TimeColumnData()
{
	d_format = "hh:mm:ss.zzz";
}

TimeColumnData::TimeColumnData(const QList<QTime>& list)
{
	d_format = "hh:mm:ss.zzz";
	*(static_cast< QList<QTime>* >(this)) = list;
}

TimeColumnData::~TimeColumnData()
{
}

AbstractColumnData::ColumnDataType TimeColumnData::type() const
{
	return AbstractColumnData::Time;
}

bool TimeColumnData::clone(const AbstractColumnData& other)
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
	
bool TimeColumnData::cloneDoubleColumnData(const DoubleColumnData& other)
{ 
 	clear();

	int end = other.size();
	double temp;
	int itemp;
	QTime ref_time = QTime(0,0,0,0);
	for(int i=0; i<end; i++)
	{
		temp = other.at(i);
		itemp = int(temp);
		temp -= double(itemp); // we only want the digits behind the dot
		temp *= 86400000.0; // convert from fraction of day to milliseconds
		*this << ref_time.addMSecs(int(temp));
	}
	return true;
}

bool TimeColumnData::cloneTimeColumnData(const TimeColumnData& other)
{ 
	*(static_cast< QList<QTime>* >(this)) = static_cast< const QList<QTime>& >(other);
	d_format = other.format();
	return true;
}

bool TimeColumnData::cloneDateColumnData(const DateColumnData& other)
{  
 	clear();

	int end = other.size();
	for(int i=0; i<end; i++)
		*this << QTime(0,0,0,0);
	// converting QDate to QTime does not make much sense
	// therefore this will result in a list of zero times
	// and will return false since the column information is lost
	return false;
}

bool TimeColumnData::cloneStringColumnData(const StringColumnData& other)
{ 
 	clear();

	int end = other.size();
	for(int i=0; i<end; i++)
	{
		*this << QTime();
		setCellFromString(i, other.at(i));
	}
	return true;
}

QString TimeColumnData::cellString(int index) const
{
	if(index < size())
		return at(index).toString(d_format);
	else
		return QString();
}

// Some format strings to try in setCellFromString()
#define NUM_TIME_FMT_STRINGS 9
static const char * common_time_format_strings[NUM_TIME_FMT_STRINGS] = {
			"h",
			"h ap",
			"h:mm",
			"h:mm ap",
			"h:mm:ss",
			"h:mm:ss.zzz",
			"h:mm:ss:zzz",
			"mm:ss.zzz",
			"hmmss",
};

void TimeColumnData::setCellFromString(int index, const QString& string)
{
	QTime result = QTime::fromString(string, d_format);
	// try other format strings
	int i=0;
	while( !result.isValid() && i<NUM_TIME_FMT_STRINGS )
		result = QTime::fromString(string, common_time_format_strings[i++]);
	
	(*(static_cast< QList<QTime>* >(this)))[index] = result;
}

void TimeColumnData::resize(int new_size)
{
	int old_size = size();
	int count = new_size - old_size;
	if(count == 0) return;

	if(count > 0)
	{
		for(int i=0; i<count; i++)
			*this << QTime();
	}
	else // count < 0
	{
		for(int i=0; i>count; i--)
			removeLast();
		
	}
}

int TimeColumnData::size() const
{
	return QList<QTime>::size();
}
