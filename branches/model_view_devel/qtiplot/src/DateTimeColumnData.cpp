/***************************************************************************
    File                 : DateTimeColumnData.cpp
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Data source that stores a list of QDateTimes (implementation)

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

#include "DateTimeColumnData.h"
#include "AbstractDoubleDataSource.h"
#include "AbstractStringDataSource.h"
#include <QObject>
#include <QStringList>

DateTimeColumnData::DateTimeColumnData() 
	: d_format("yyyy-MM-dd hh:mm:ss.zzz")
{
}

DateTimeColumnData::DateTimeColumnData(const QList<QDateTime>& list)
	: d_format("yyyy-MM-dd hh:mm:ss.zzz") 
{ 
	*(static_cast< QList<QDateTime>* >(this)) = list; 
}

bool DateTimeColumnData::copy(const AbstractDataSource * other)
{
	if(other->inherits("AbstractDoubleDataSource"))
			return copyDoubleDataSource(static_cast<const AbstractDoubleDataSource *>(other));
	if(other->inherits("AbstractStringDataSource"))
			return copyStringDataSource(static_cast<const AbstractStringDataSource *>(other));
	if(other->inherits("AbstractDateTimeDataSource"))
			return copyDateTimeDataSource(static_cast<const AbstractDateTimeDataSource *>(other));
	return false;
}
	
int DateTimeColumnData::numRows() const 
{ 
	return size(); 
}

QString DateTimeColumnData::label() const
{ 
	return d_label;
}

QString DateTimeColumnData::comment() const
{ 
	return d_comment;
}

AbstractDataSource::PlotDesignation DateTimeColumnData::plotDesignation() const
{ 
	return d_plot_designation;
}

void DateTimeColumnData::setLabel(const QString& label) 
{ 
	emit descriptionAboutToChange(this);
	d_label = label; 
	emit descriptionChanged(this);
}

void DateTimeColumnData::setComment(const QString& comment) 
{ 
	emit descriptionAboutToChange(this);
	d_comment = comment; 
	emit descriptionChanged(this);
}

void DateTimeColumnData::setPlotDesignation(AbstractDataSource::PlotDesignation pd) 
{ 
	emit plotDesignationAboutToChange(this);
	d_plot_designation = pd; 
	emit plotDesignationChanged(this);
}

void DateTimeColumnData::notifyReplacement(AbstractDataSource * replacement)
{
	emit aboutToBeReplaced(this, replacement); 
}

// Some format strings to try in setRowFromString()
#define NUM_DATE_FMT_STRINGS 11
static const char * common_date_format_strings[NUM_DATE_FMT_STRINGS] = {
	"yyyy-M-d", // ISO 8601 w/ and w/o leading zeros
	"yyyy/M/d", 
	"d/M/yyyy", // European style day/month order (this order seems to be used in more countries than the US style M/d/yyyy)
	"d/M/yy", 
	"d-M-yyyy",
	"d-M-yy", 
	"d.M.yyyy", // German style
	"d.M.yy",
	"M/yyyy",
	"d.M.", // German form w/o year
	"yyyyMMdd"
};

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


void DateTimeColumnData::setRowFromString(int row, const QString& string)
{
	emit dataAboutToChange(this);
	(*(static_cast< QList<QDateTime>* >(this)))[row] = dateTimeFromString(string, d_format);
	emit dataChanged(this);
}

void DateTimeColumnData::setNumRows(int new_size)
{
	int count = new_size - numRows();
	if(count == 0) return;

	emit dataAboutToChange(this);
	if(count > 0)
	{
		for(int i=0; i<count; i++)
			*this << QDateTime();
	}
	else // count < 0
	{
		for(int i=0; i>count; i--)
			removeLast();
	}
	emit dataChanged(this);
}

void DateTimeColumnData::insertEmptyRows(int before, int count)
{
	emit dataAboutToChange(this);
	for(int i=0; i<count; i++)
		insert(before, QDateTime());
	emit dataChanged(this);
}

void DateTimeColumnData::removeRows(int first, int count)
{
	emit dataAboutToChange(this);
	for(int i=0; i<count; i++)
		removeAt(first);
	emit dataChanged(this);
}

void DateTimeColumnData::setFormat(const QString& format) 
{ 
	emit formatAboutToChange(this);
	d_format = format; 
	emit formatChanged(this);
}

QString DateTimeColumnData::textAt(int row) const 
{ 
	QDateTime temp = value(row);
	if(!temp.date().isValid() && temp.time().isValid())
		temp.setDate(QDate(1900,1,1)); // see class documentation
	return temp.toString(d_format);
}

double DateTimeColumnData::valueAt(int row) const 
{ 
	return double(value(row).date().toJulianDay()) + double( -at(row).time().msecsTo(QTime(12,0,0,0))/86400000.0 );
}

QString DateTimeColumnData::format() const 
{ 
	return d_format; 
}

QDate DateTimeColumnData::dateAt(int row) const 
{ 
	return value(row).date(); 
}

QTime DateTimeColumnData::timeAt(int row) const 
{ 
	return value(row).time(); 
}
QDateTime DateTimeColumnData::dateTimeAt(int row) const 
{ 
	return value(row); 
}

bool DateTimeColumnData::copyDoubleDataSource(const AbstractDoubleDataSource * other)
{ 
	emit dataAboutToChange(this);
 	clear();

	int end = other->numRows();
	QDateTime temp;
	double val;
	for(int i=0; i<end; i++)
	{
		val = other->valueAt(i);
		temp.setDate( QDate::fromJulianDay(int(val + 0.5)) ); 
								// we only want the digits behind the dot and 
								// convert them from fraction of day to milliseconds
		temp.setTime( QTime(12,0,0,0).addMSecs( int(val - int(other->valueAt(i)) * 86400000.0) ) );
		*this << temp;
	}
		
	emit dataChanged(this);
	return true;
}

bool DateTimeColumnData::copyDateTimeDataSource(const AbstractDateTimeDataSource * other)
{ 
	emit dataAboutToChange(this);
 	clear();

	int end = other->numRows();
	for(int i=0; i<end; i++)
		*this << other->dateTimeAt(i);
	d_format = other->format();
	emit dataChanged(this);
	return true;
}

bool DateTimeColumnData::copyStringDataSource(const AbstractStringDataSource * other)
{ 
	emit dataAboutToChange(this);
 	clear();

	int end = other->numRows();
	for(int i=0; i<end; i++)
		*this << dateTimeFromString(other->textAt(i), d_format);
	emit dataChanged(this);
	return true;
}


QDateTime DateTimeColumnData::dateTimeFromString(const QString& string, const QString& format)
{
	QDateTime result = QDateTime::fromString(string, format);
	if(result.date().isValid() || result.time().isValid())
		return result;

	// try other format strings 
	// comma and space are valid separators between date and time
	QStringList strings = string.simplified().split(",", QString::SkipEmptyParts);
	if(strings.size() == 1) strings = strings.at(0).split(" ", QString::SkipEmptyParts);

	if(strings.size() < 1)
		return result;
	
	QDate date_result;
	QTime time_result;

	QString date_string = strings.at(0).trimmed();
	QString time_string;
	if(strings.size() > 1)
		time_string = strings.at(1).trimmed();
	else
		time_string = date_string;

	int i=0;
	// find a valid date
	while( !date_result.isValid() && i<NUM_DATE_FMT_STRINGS )
		date_result = QDate::fromString(date_string, common_date_format_strings[i++]);
	i=0;
	// find a valid time
	while( !time_result.isValid() && i<NUM_TIME_FMT_STRINGS )
		time_result = QTime::fromString(time_string, common_time_format_strings[i++]);

	if(!date_result.isValid())
		date_result.setDate(1900,1,1); // this is what QDateTime does e.g. for
										// QDateTime::fromString("00:00","hh:mm");
	return QDateTime(date_result, time_result);
}

