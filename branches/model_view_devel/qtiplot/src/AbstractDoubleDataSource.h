/***************************************************************************
    File                 : AbstractDoubleDataSource.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Type-specific reading interface for a double data source

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

#ifndef DOUBLEDATASOURCE_H
#define DOUBLEDATASOURCE_H

#include "AbstractDataSource.h"

//! Type-specific reading interface for a double data source
/**
  * This class defines the specific reading interface for
  * a class storing a vector of double precision values. 
  * It only defines the interface and has no data members itself.
  * \sa AbstractDataSource
  * \sa AbstractColumnData
  */
class AbstractDoubleDataSource : public AbstractDataSource
{
	Q_OBJECT

public:
	//! Dtor
	virtual ~AbstractDoubleDataSource(){};

	//! Return format character as in QString::number
	virtual char numericFormat() const = 0;
	//! Return the number of displayed digits
	virtual int displayedDigits() const = 0;
	//! Return a read-only array pointer for fast data access
	/**
	 * The pointer remains valid as long as the vector is 
	 * not resized. When it is resized it will emit
	 * a AbstractDataSource::dataChanged() signal.
	 */ 
	virtual const double * constDataPointer() const = 0;

};

#endif
