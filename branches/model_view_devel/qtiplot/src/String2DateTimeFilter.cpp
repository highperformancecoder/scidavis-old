/***************************************************************************
    File                 : String2DateTimeFilter.cpp
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
                           Knut Franke
    Email (use @ for *)  : thzs*gmx.net, knut.franke*gmx.de
    Description          : Conversion filter QString -> QDateTime.
                           
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
#include "String2DateTimeFilter.h"
#include <QStringList>

const char * String2DateTimeFilter::date_formats[] = {
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
	"yyyyMMdd",
	0
};

const char * String2DateTimeFilter::time_formats[] = {
	"h",
	"h ap",
	"h:mm",
	"h:mm ap",
	"h:mm:ss",
	"h:mm:ss.zzz",
	"h:mm:ss:zzz",
	"mm:ss.zzz",
	"hmmss",
	0
};

QDateTime String2DateTimeFilter::dateTimeAt(int row) const
{
	if (!d_inputs.value(0)) return QDateTime();
	QString string = static_cast<AbstractStringDataSource*>(d_inputs.at(0))->textAt(row);

	// first try the selected format string d_format
	QDateTime result = QDateTime::fromString(string, d_format);
	if(result.date().isValid() || result.time().isValid())
		return result;

	// fallback:
	// try other format strings built from date_formats and time_formats
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
	while( !date_result.isValid() && date_formats[i] )
		date_result = QDate::fromString(date_string, date_formats[i++]);
	i=0;
	// find a valid time
	while( !time_result.isValid() && time_formats[i] )
		time_result = QTime::fromString(time_string, time_formats[i++]);

	if(!date_result.isValid())
		date_result.setDate(1900,1,1);	// this is what QDateTime does e.g. for
													// QDateTime::fromString("00:00","hh:mm");
	return QDateTime(date_result, time_result);
}
