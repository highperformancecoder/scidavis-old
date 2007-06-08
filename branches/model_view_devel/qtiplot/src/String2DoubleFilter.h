/***************************************************************************
    File                 : String2DoubleFilter.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Knut Franke
    Email (use @ for *)  : knut.franke*gmx.de
    Description          : Locale-aware converter QString -> double.
                           
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
#ifndef STRING2DOUBLE_FILTER_H
#define STRING2DOUBLE_FILTER_H

#include "AbstractSimpleFilter.h"
#include "AbstractStringDataSource.h"
#include <QLocale>

//! Locale-aware converter QString -> double.
class String2DoubleFilter : public AbstractSimpleFilter<double>
{
	Q_OBJECT

	public:
// simplified filter interface
		//! Using typed ports: only string inputs are accepted.
		virtual bool input(int port, AbstractDataSource *source) {
			if (!source->inherits("AbstractStringDataSource"))
				return false;
			return AbstractSimpleFilter<double>::input(port, source);
		}
		virtual QString label() const {
			return d_inputs.value(0) ? d_inputs.at(0)->label() : QString();
		}
		virtual int numRows() const {
			return d_inputs.value(0) ? d_inputs.at(0)->numRows() : 0;
		}
		virtual double valueAt(int row) const {
			if (!d_inputs.value(0) || row >= d_inputs.at(0)->numRows()) return 0;
			return QLocale().toDouble(d_inputs.at(0)->textAt(row));
		}
};

#endif // ifndef STRING2DOUBLE_FILTER_H

