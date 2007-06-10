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
#include <QObject>

DateTimeColumnData::DateTimeColumnData() 
{
}

DateTimeColumnData::DateTimeColumnData(const QList<QDateTime>& list)
{ 
	*(static_cast< QList<QDateTime>* >(this)) = list; 
}

bool DateTimeColumnData::copy(const AbstractDataSource * other)
{
	const AbstractDateTimeDataSource *other_as_date_time = qobject_cast<const AbstractDateTimeDataSource*>(other);
	if(!other_as_date_time) return false;

	int count = other_as_date_time->numRows();
	setNumRows(count);

	emit dataAboutToChange(this);
	for(int i=0; i<count; i++)
	{
		replace(i, other_as_date_time->dateTimeAt(i));
		setValid(i, other_as_date_time->isValid(i));
	}
	emit dataChanged(this);
	return true;
}

bool DateTimeColumnData::copy(const AbstractDataSource * source, int source_start, int dest_start, int num_rows)
{
	const AbstractDateTimeDataSource *source_as_date_time = qobject_cast<const AbstractDateTimeDataSource*>(source);
	if (!source_as_date_time) return false;

	if (dest_start + num_rows > numRows())
		setNumRows(dest_start + num_rows);

	emit dataAboutToChange(this);
	for(int i=0; i<num_rows; i++)
	{
		replace(dest_start+i, source_as_date_time->dateTimeAt(source_start+i));
		setValid(dest_start+i, source_as_date_time->isValid(source_start+i));
	}
	emit dataChanged(this);
	return true;
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

void DateTimeColumnData::setNumRows(int new_size)
{
	int old_size = numRows();
	int count = new_size - old_size;
	if(count == 0) return;

	if(count > 0)
	{
		emit rowsAboutToBeInserted(this, old_size, count);
		for(int i=0; i<count; i++)
			*this << QDateTime();
		d_validity.insertRows(old_size, count);
		d_selection.insertRows(old_size, count);
		d_masking.insertRows(old_size, count);
		d_formulas.insertRows(old_size, count);
		emit rowsInserted(this, old_size, count);
	}
	else // count < 0
	{
		emit rowsAboutToBeDeleted(this, new_size, -count);
		for(int i=0; i>count; i--)
			removeLast();
		d_validity.removeRows(new_size, -count);
		d_selection.removeRows(new_size, -count);
		d_masking.removeRows(new_size, -count);
		d_formulas.removeRows(new_size, -count);
		emit rowsDeleted(this, new_size, -count);
	}
}

void DateTimeColumnData::insertEmptyRows(int before, int count)
{
	emit rowsAboutToBeInserted(this, before, count);
	for(int i=0; i<count; i++)
		insert(before, QDateTime());
	d_validity.insertRows(before, count);
	d_selection.insertRows(before, count);
	d_masking.insertRows(before, count);
	d_formulas.insertRows(before, count);
	emit rowsInserted(this, before, count);
}

void DateTimeColumnData::removeRows(int first, int count)
{
	emit rowsAboutToBeDeleted(this, first, count);
	for(int i=0; i<count; i++)
		removeAt(first);
	d_validity.removeRows(first, count);
	d_selection.removeRows(first, count);
	d_masking.removeRows(first, count);
	d_formulas.removeRows(first, count);
	emit rowsDeleted(this, first, count);
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

void DateTimeColumnData::clear()
{
	removeRows(0, size());
}

