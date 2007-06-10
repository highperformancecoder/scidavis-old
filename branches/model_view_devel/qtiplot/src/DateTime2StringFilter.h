/***************************************************************************
    File                 : DateTime2StringFilter.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
                           Knut Franke
    Email (use @ for *)  : thzs*gmx.net, knut.franke*gmx.de
    Description          : Conversion filter QDateTime -> QString.
                           
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
#ifndef DATE_TIME2STRING_FILTER_H
#define DATE_TIME2STRING_FILTER_H

#include "AbstractSimpleFilter.h"
#include "AbstractDateTimeDataSource.h"
#include <QDateTime>

//! Conversion filter QDateTime -> QString.
class DateTime2StringFilter : public AbstractSimpleFilter<QString>
{
	Q_OBJECT

	public:
// select conversion format
		//! Standard constructor.
		explicit DateTime2StringFilter(QString format="yyyy-MM-dd hh:mm:ss.zzz") : d_format(format) {}
		//! Set the format string to be used for conversion.
		void setFormat(QString format) { d_format = format; }
		//! Return the format string
		/**
		 * The default format string is "yyyy-MM-dd hh:mm:ss.zzz".
		 * \sa QDate::toString()
		 */
		QString format() const { return d_format; }

// simplified filter interface
		virtual QString label() const {
			return d_inputs.value(0) ? d_inputs.at(0)->label() : QString();
		}
		virtual int numRows() const {
			return d_inputs.value(0) ? d_inputs.at(0)->numRows() : 0;
		}
		virtual QString textAt(int row) const {
			if (!d_inputs.value(0)) return QString();
			QDateTime temp = static_cast<AbstractDateTimeDataSource*>(d_inputs.at(0))->dateTimeAt(row);
			if(!temp.date().isValid() && temp.time().isValid())
				temp.setDate(QDate(1900,1,1)); // see class documentation
			return temp.toString(d_format);
		}
	protected:
		//! Using typed ports: only DateTime inputs are accepted.
		virtual bool inputAcceptable(int, AbstractDataSource *source) {
			return source->inherits("AbstractDateTimeDataSource");
		}

	private:
		//! The format string.
		QString d_format;
};

#endif // ifndef DATE_TIME2STRING_FILTER_H

