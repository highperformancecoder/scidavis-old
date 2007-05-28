/***************************************************************************
    File                 : DateColumnData.cpp
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Data source that stores a list of QDates (implementation)

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
#include "AbstractDoubleDataSource.h"
#include "AbstractStringDataSource.h"
#include "AbstractTimeDataSource.h"
#include <QObject>

DateColumnData::DateColumnData() 
	: d_format("yyyy-MM-dd")
{
}

DateColumnData::DateColumnData(const QList<QDate>& list)
	: d_format("yyyy-MM-dd") 
{ 
	*(static_cast< QList<QDate>* >(this)) = list; 
}

bool DateColumnData::copy(const AbstractDataSource * other)
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
	
int DateColumnData::numRows() const 
{ 
	return size(); 
}

QString DateColumnData::label() const
{ 
	return d_label;
}

QString DateColumnData::comment() const
{ 
	return d_comment;
}

AbstractDataSource::PlotDesignation DateColumnData::plotDesignation() const
{ 
	return d_plot_designation;
}

void DateColumnData::setLabel(const QString& label) 
{ 
	emit descriptionAboutToChange(this);
	d_label = label; 
	emit descriptionChanged(this);
}

void DateColumnData::setComment(const QString& comment) 
{ 
	emit descriptionAboutToChange(this);
	d_comment = comment; 
	emit descriptionChanged(this);
}

void DateColumnData::setPlotDesignation(AbstractDataSource::PlotDesignation pd) 
{ 
	emit plotDesignationAboutToChange(this);
	d_plot_designation = pd; 
	emit plotDesignationChanged(this);
}

void DateColumnData::notifyReplacement(AbstractDataSource * replacement)
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

void DateColumnData::setRowFromString(int row, const QString& string)
{
	emit dataAboutToChange(this);
	QDate result = QDate::fromString(string, d_format);
	// try other format strings
	int i=0;
	while( !result.isValid() && i<NUM_DATE_FMT_STRINGS )
		result = QDate::fromString(string, common_date_format_strings[i++]);
	
	(*(static_cast< QList<QDate>* >(this)))[row] = result; 
	emit dataChanged(this);
}

void DateColumnData::setNumRows(int new_size)
{
	int count = new_size - numRows();
	if(count == 0) return;

	emit dataAboutToChange(this);
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
	emit dataChanged(this);
}

void DateColumnData::insertEmptyRows(int before, int count)
{
	emit dataAboutToChange(this);
	for(int i=0; i<count; i++)
		insert(before, QDate());
	emit dataChanged(this);
}

void DateColumnData::removeRows(int first, int count)
{
	emit dataAboutToChange(this);
	for(int i=0; i<count; i++)
		removeAt(first);
	emit dataChanged(this);
}

void DateColumnData::setFormat(const QString& format) 
{ 
	emit specificDataAboutToChange(this);
	d_format = format; 
	emit specificDataChanged(this);
}

QString DateColumnData::rowString(int row) const 
{ 
	return at(row).toString(d_format); 
}

double DateColumnData::rowValue(int row) const 
{ 
	return double( -at(row).daysTo(QDate(1900,1,1)) ); 
}

QString DateColumnData::format() const 
{ 
	return d_format; 
}

QDate DateColumnData::dateAt(int row) const 
{ 
	return at(row); 
}

bool DateColumnData::copyDoubleDataSource(const AbstractDoubleDataSource * other)
{ 
	emit dataAboutToChange(this);
 	clear();

	int end = other->numRows();
	QDate ref_date = QDate(1900,1,1);
	for(int i=0; i<end; i++)
		*this << ref_date.addDays(int(other->rowValue(i)));
	emit dataChanged(this);
	return true;
}

bool DateColumnData::copyDateDataSource(const AbstractDateDataSource * other)
{ 
	emit dataAboutToChange(this);
 	clear();

	int end = other->numRows();
	for(int i=0; i<end; i++)
		*this << other->dateAt(i);
	d_format = other->format();
	emit dataChanged(this);
	return true;
}

bool DateColumnData::copyTimeDataSource(const AbstractTimeDataSource * other)
{  
	emit dataAboutToChange(this);
 	clear();

	int end = other->numRows();
	for(int i=0; i<end; i++)
		*this << QDate(1900,1,1);
	// converting QTime to QDate does not make much sense
	// therefore this will result in a list of 1900/1/1
	// and will return false since the column information is lost
	emit dataChanged(this);
	return false;
}

bool DateColumnData::copyStringDataSource(const AbstractStringDataSource * other)
{ 
	emit dataAboutToChange(this);
 	clear();

	int end = other->numRows(),j;
	QDate result;
	for(int i=0; i<end; i++)
	{
		result = QDate::fromString(other->rowString(i), d_format);
		// try other format strings
		j=0;
		while( !result.isValid() && j<NUM_DATE_FMT_STRINGS )
			result = QDate::fromString(other->rowString(i), common_date_format_strings[j++]);
	
		*this << result;
	}
	emit dataChanged(this);
	return true;
}

