/***************************************************************************
    File                 : AbstractDateDataSource.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Type-specific reading interface for a date data source

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

#ifndef DATEDATASOURCE_H
#define DATEDATASOURCE_H

#include "AbstractDataSource.h"
class QDate;
class QString;

//! Type-specific reading interface for a date data source
/**
  * This class defines the specific reading interface for
  * a class storing a list of QDates. It only defines
  * the interface and has no data members itself.
  * \sa AbstractDataSource
  * \sa AbstractColumnData
  */
class AbstractDateDataSource : public AbstractDataSource
{
	Q_OBJECT

public:
	//! Dtor
	virtual ~AbstractDateDataSource(){};

	//! Return the date in row 'row'
	virtual QDate dateAt(int row) const = 0;
	//! Return the format string
	/**
	 * The default format string is "yyyy-MM-dd".
	 * \sa QDate::toString()
	 */
	virtual QString format() const = 0;

};

#endif
