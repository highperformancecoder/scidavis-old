/***************************************************************************
    File                 : Double2DateTimeFilter.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Knut Franke
    Email (use @ for *)  : knut.franke*gmx.de
    Description          : Conversion filter double -> QDateTime (using Julian day).
                           
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
#ifndef DOUBLE2DATE_TIME_FILTER_H
#define DOUBLE2DATE_TIME_FILTER_H

#include "AbstractSimpleFilter.h"
#include "AbstractDoubleDataSource.h"
#include <QDateTime>
#include <QDate>
#include <QTime>

//! Conversion filter double -> QDateTime (using Julian day).
class Double2DateTimeFilter : public AbstractSimpleFilter<QDateTime>
{
	Q_OBJECT

	public:
// simplified filter interface
		virtual QString label() const {
			return d_inputs.value(0) ? d_inputs.at(0)->label() : QString();
		}
		virtual int numRows() const {
			return d_inputs.value(0) ? d_inputs.at(0)->numRows() : 0;
		}
		virtual QDate dateAt(int row) const {
			if (!d_inputs.value(0) || !d_inputs.at(0)->isValid(row)) return QDate();
			return QDate::fromJulianDay(int(static_cast<AbstractDoubleDataSource*>(d_inputs.at(0))->valueAt(row)));
		}
		virtual QTime timeAt(int row) const {
			if (!d_inputs.value(0) || !d_inputs.at(0)->isValid(row)) return QTime();
			// we only want the digits behind the dot and 
			// convert them from fraction of day to milliseconds
			double val = static_cast<AbstractDoubleDataSource*>(d_inputs.at(0))->valueAt(row);
			return QTime(12,0,0,0).addMSecs(int( (val - int(val)) * 86400000.0 ));
		}
		virtual QDateTime dateTimeAt(int row) const {
			return QDateTime(dateAt(row), timeAt(row));
		}

	protected:
		//! Using typed ports: only double inputs are accepted.
		virtual bool inputAcceptable(int, AbstractDataSource *source) {
			return source->inherits("AbstractDoubleDataSource");
		}
};

#endif // ifndef DOUBLE2DATE_TIME_FILTER_H

