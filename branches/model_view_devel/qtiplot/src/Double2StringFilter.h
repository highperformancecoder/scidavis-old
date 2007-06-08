/***************************************************************************
    File                 : Double2StringFilter.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Knut Franke
    Email (use @ for *)  : knut.franke*gmx.de
    Description          : Locale-aware converter double -> QString.
                           
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
#ifndef DOUBLE2STRING_FILTER_H
#define DOUBLE2STRING_FILTER_H

#include "AbstractSimpleFilter.h"
#include "AbstractDoubleDataSource.h"
#include <QLocale>

//! Locale-aware converter double -> QString.
class Double2StringFilter : public AbstractSimpleFilter<QString>
{
	Q_OBJECT

	public:
// select conversion format
		//! Set format character as in QString::number
		virtual void setNumericFormat(char format) { d_format = format; }
		//! Set number of displayed digits
		virtual void setNumDigits(int digits) { d_digits = digits; }

// simplified filter interface
		virtual QString textAt(int row) const {
			// if we're using filters like this one for conversion, we can drop
			// AbstractDataSource::valueAt() and AbstractDataSource::textAt(), replacing them with
			// virtual Type TypeDataSource::valueAt(int row)
			// format options could then be removed from *DataSource and *ColumnData
			if (!d_inputs.value(0) || row >= d_inputs.at(0)->numRows()) return QString();
			return QLocale().toString(d_inputs.at(0)->valueAt(row), d_format, d_digits);
		}
		virtual QString label() const {
			return d_inputs.value(0) ? d_inputs.at(0)->label() : QString();
		}
		virtual int numRows() const {
			return d_inputs.value(0) ? d_inputs.at(0)->numRows() : 0;
		}
		//! Using typed ports: only double inputs are accepted.
		virtual bool input(int port, AbstractDataSource *source) {
			if (!source->inherits("AbstractDoubleDataSource"))
				return false;
			return AbstractSimpleFilter<QString>::input(port, source);
		}

	private:
		char d_format;
		int d_digits;
};

#endif // ifndef DOUBLE2STRING_FILTER_H

