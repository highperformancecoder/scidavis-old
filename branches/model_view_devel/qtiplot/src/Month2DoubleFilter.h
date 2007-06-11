/***************************************************************************
    File                 : MonthDoubleFilter.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Knut Franke
    Email (use @ for *)  : knut.franke*gmx.de
    Description          : Conversion filter QDateTime -> double, translating
                           dates into months (January -> 1).
                           
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
#ifndef MONTH2DOUBLE_FILTER_H
#define MONTH2DOUBLE_FILTER_H

#include "AbstractSimpleFilter.h"
#include <QDateTime>

/**
 * \brief Conversion filter QDateTime -> double, translating dates into months (January -> 1).
 *
 * \sa QDate::month()
 */
class Month2DoubleFilter : public AbstractSimpleFilter<double>
{
	Q_OBJECT

	public:
		virtual double valueAt(int row) const {
			if (!d_inputs.value(0)) return 0;
			return double(dateTimeInput()->dateAt(row).month());
			/*
			QDateTime input_value = dateTimeInput()->dateTimeAt(row);
			return double((4713+input_value.date().year())*12 + input_value.date().month()) // integer part plus...
				+ ( double(input_value.date().day() - 1) // ...days into month...
						+ double( -input_value.time().msecsTo(QTime(12,0,0,0)) ) / 86400000.0 ) // ...and fractional days...
				/ double(input_value.date().daysInMonth()); // ...as fraction of the month.
			*/
		}

	protected:
		//! Using typed ports: only string inputs are accepted.
		virtual bool inputAcceptable(int, AbstractDataSource *source) {
			return source->inherits("AbstractDateTimeDataSource");
		}
};

#endif // ifndef MONTH2DOUBLE_FILTER_H

