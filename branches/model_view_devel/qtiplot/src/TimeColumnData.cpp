/***************************************************************************
    File                 : TimeColumnData.cpp
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Data source that stores a list of QTimes (write functions)

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
#include "StringColumnData.h"
#include "DateColumnData.h"
#include <QObject>

TimeColumnData::TimeColumnData() 
	: d_format("hh:mm:ss.zzz")
{
}

TimeColumnData::TimeColumnData(const QList<QTime>& list)
	: d_format("hh:mm:ss.zzz") 
{ 
	*(static_cast< QList<QTime>* >(this)) = list; 
}


bool TimeColumnData::copy(const AbstractDataSource * other)
{
	if(other->inherits("AbstractDoubleDataSource"))
			return copyDoubleDataSource(static_cast<const AbstractDoubleDataSource *>(other));
	if(other->inherits("AbstractStringDataSource"))
			return copyStringDataSource(static_cast<const AbstractStringDataSource *>(other));
	if(other->inherits("AbstractDateDataSource"))
			return copyDateDataSource(static_cast<const AbstractDateDataSource *>(other));
	if(other->inherits("AbstractTimeDataSource"))
			return copyTimeDataSource(static_cast<const AbstractTimeDataSource *>(other));
	return false;
}
	
int TimeColumnData::numRows() const 
{ 
	return size(); 
}

QString TimeColumnData::label() const
{ 
	return d_label;
}

QString TimeColumnData::comment() const
{ 
	return d_comment;
}

AbstractDataSource::PlotDesignation TimeColumnData::plotDesignation() const
{ 
	return d_plot_designation;
}

void TimeColumnData::setLabel(const QString& label) 
{ 
	emit descriptionAboutToChange(this);
	d_label = label; 
	emit descriptionChanged(this);
}

void TimeColumnData::setComment(const QString& comment) 
{ 
	emit descriptionAboutToChange(this);
	d_comment = comment; 
	emit descriptionChanged(this);
}

void TimeColumnData::setPlotDesignation(AbstractDataSource::PlotDesignation pd) 
{ 
	emit plotDesignationAboutToChange(this);
	d_plot_designation = pd; 
	emit plotDesignationChanged(this);
}

void TimeColumnData::notifyReplacement(AbstractDataSource * replacement)
{
	emit aboutToBeReplaced(this, replacement); 
}

// Some format strings to try in setRowFromString()
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

void TimeColumnData::setRowFromString(int row, const QString& string)
{
	emit dataAboutToChange(this);
	QTime result = QTime::fromString(string, d_format);
	// try other format strings
	int i=0;
	while( !result.isValid() && i<NUM_TIME_FMT_STRINGS )
		result = QTime::fromString(string, common_time_format_strings[i++]);
	
	(*(static_cast< QList<QTime>* >(this)))[row] = result;
	emit dataChanged(this);
}


void TimeColumnData::setNumRows(int new_size)
{
	int count = new_size - numRows();
	if(count == 0) return;

	emit dataAboutToChange(this);
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
	emit dataChanged(this);
}

void TimeColumnData::insertEmptyRows(int before, int count)
{
	emit dataAboutToChange(this);
	for(int i=0; i<count; i++)
		insert(before, QTime());
	emit dataChanged(this);
}

void TimeColumnData::removeRows(int first, int count)
{
	emit dataAboutToChange(this);
	for(int i=0; i<count; i++)
		removeAt(first);
	emit dataChanged(this);
}

void TimeColumnData::setFormat(const QString& format) 
{ 
	emit specificDataAboutToChange(this);
	d_format = format; 
	emit specificDataChanged(this);
}

QString TimeColumnData::rowString(int row) const 
{ 
	return at(row).toString(d_format); 
}

double TimeColumnData::rowValue(int row) const 
{ 
	return double( -at(row).msecsTo(QTime(0,0,0,0))/86400000.0 );
}

QString TimeColumnData::format() const 
{ 
	return d_format; 
}

QTime TimeColumnData::timeAt(int row) const 
{ 
	return at(row); 
}

bool TimeColumnData::copyDoubleDataSource(const AbstractDoubleDataSource * other)
{ 
	emit dataAboutToChange(this);
 	clear();

	double temp;
	int itemp;
	QTime ref_time(0,0,0,0);
	int end = other->numRows();
	for(int i=0; i<end; i++)
	{
		temp = other->rowValue(i);
		itemp = int(temp);
		temp -= double(itemp); // we only want the digits behind the dot
		temp *= 86400000.0; // convert from fraction of day to milliseconds
		*this << ref_time.addMSecs(int(temp));
	}
	emit dataChanged(this);
	return true;
}

bool TimeColumnData::copyTimeDataSource(const AbstractTimeDataSource * other)
{ 
	emit dataAboutToChange(this);
 	clear();

	int end = other->numRows();
	for(int i=0; i<end; i++)
		*this << other->timeAt(i);
	d_format = other->format();
	emit dataChanged(this);
	return true;
}

bool TimeColumnData::copyDateDataSource(const AbstractDateDataSource * other)
{  
	emit dataAboutToChange(this);
 	clear();

	int end = other->numRows();
	for(int i=0; i<end; i++)
		*this << QTime(0,0,0,0);
	// converting QDate to QTime does not make much sense
	// therefore this will result in a list of zero times
	// and will return false since the column information is lost
	emit dataChanged(this);
	return false;
}

bool TimeColumnData::copyStringDataSource(const AbstractStringDataSource * other)
{ 
	emit dataAboutToChange(this);
 	clear();

	int end = other->numRows(),j;
	QTime result;
	for(int i=0; i<end; i++)
	{
		result = QTime::fromString(other->rowString(i), d_format);
		// try other format strings
		j=0;
		while( !result.isValid() && j<NUM_TIME_FMT_STRINGS )
			result = QTime::fromString(other->rowString(i), common_time_format_strings[j++]);
	
		*this << result;
	}
	emit dataChanged(this);
	return true;
}

