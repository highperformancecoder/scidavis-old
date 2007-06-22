/***************************************************************************
    File                 : Double2MonthFilter.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Knut Franke
    Email (use @ for *)  : knut.franke*gmx.de
    Description          : Conversion filter double -> QDateTime, interpreting
                           the input numbers as months of the year.
                           
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
#ifndef DOUBLE2MONTH_FILTER_H
#define DOUBLE2MONTH_FILTER_H

#include "AbstractSimpleFilter.h"
#include <QDateTime>
#include <math.h>

//! Conversion filter double -> QDateTime, interpreting the input numbers as days of the week (1 = Monday).
class Double2MonthFilter : public AbstractSimpleFilter<QDateTime>
{
	Q_OBJECT
	public:
		virtual QDate dateAt(int row) const {
			return dateTimeAt(row).date();
		}
		virtual QTime timeAt(int row) const {
			return dateTimeAt(row).time();
		}
		virtual QDateTime dateTimeAt(int row) const {
			if (!d_inputs.value(0)) return QDateTime();
			double input_value = doubleInput()->valueAt(row);
			// Julian day 0 is a January, the first. We want 1 -> January, so our
			// reference month is the December before Julian epoch.
			QDate result_date = QDate::fromJulianDay(0).addMonths(int(input_value) - 1);
			/*
			// translate fractional digits to fractions of the target month
			double day_of_month = double(input_value - int(input_value)) * double(result_date.daysInMonth()) + 1.0;
			result_date = result_date.addDays(int(day_of_month - 0.5));
			// translate fraction of the day to time after noon
			QTime result_time = QTime(12,0,0,0).addMSecs(int( (day_of_month - int(day_of_month - 0.5)) * 86400000.0 ));
			*/
			QTime result_time = QTime(0,0,0,0);
			return QDateTime(result_date, result_time);
		}

	protected:
		virtual bool inputAcceptable(int, AbstractDataSource *source) {
			return source->inherits("AbstractDoubleDataSource");
		}
};

#endif // ifndef DOUBLE2MONTH_FILTER_H

