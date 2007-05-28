/***************************************************************************
    File                 : StringColumnData.cpp
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Data source that stores a list of strings (implementation)

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

StringColumnData::StringColumnData()
{
}

StringColumnData::StringColumnData(const QStringList& list)
{ 
	*(static_cast< QStringList* >(this)) = list; 
}

bool StringColumnData::copy(const AbstractDataSource * other)
{
	emit dataAboutToChange(this);
	clear();
	int end = other->numRows();
	for(int i=0; i<end; i++)
		*this << other->rowString(i);
	emit dataChanged(this);
	return true;
}
	
int StringColumnData::numRows() const 
{ 
	return size(); 
}

QString StringColumnData::label() const
{ 
	return d_label;
}

QString StringColumnData::comment() const
{ 
	return d_comment;
}

AbstractDataSource::PlotDesignation StringColumnData::plotDesignation() const
{ 
	return d_plot_designation;
}

void StringColumnData::setLabel(const QString& label) 
{ 
	emit descriptionAboutToChange(this);
	d_label = label; 
	emit descriptionChanged(this);
}

void StringColumnData::setComment(const QString& comment) 
{ 
	emit descriptionAboutToChange(this);
	d_comment = comment; 
	emit descriptionChanged(this);
}

void StringColumnData::setPlotDesignation(AbstractDataSource::PlotDesignation pd) 
{ 
	emit plotDesignationAboutToChange(this);
	d_plot_designation = pd; 
	emit plotDesignationChanged(this);
}

void StringColumnData::notifyReplacement(AbstractDataSource * replacement)
{
	emit aboutToBeReplaced(this, replacement); 
}

void StringColumnData::setRowFromString(int row, const QString& string)
{
	emit dataAboutToChange(this);
    replace(row, string);
	emit dataChanged(this);
}

void StringColumnData::setNumRows(int new_size)
{
	int count = new_size - numRows();
	if(count == 0) return;

	emit dataAboutToChange(this);
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
	emit dataChanged(this);
}

void StringColumnData::insertEmptyRows(int before, int count)
{
	emit dataAboutToChange(this);
	for(int i=0; i<count; i++)
		insert(before, QString());
	emit dataChanged(this);
}

void StringColumnData::removeRows(int first, int count)
{
	emit dataAboutToChange(this);
	for(int i=0; i<count; i++)
		removeAt(first);
	emit dataChanged(this);
}

QString StringColumnData::rowString(int row) const
{ 
		return at(row);
}

double StringColumnData::rowValue(int row) const 
{ 
	return at(row).toDouble(); 
}


