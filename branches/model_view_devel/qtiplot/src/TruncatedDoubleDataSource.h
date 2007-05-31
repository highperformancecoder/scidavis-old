/***************************************************************************
    File                 : TruncatedDoubleDataSource.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Knut Franke
    Email (use @ for *)  : knut.franke*gmx.de
    Description          : A simple filter which truncates its (single) input.

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
#ifndef TRUNCATED_DOUBLE_DATA_SOURCE
#define TRUNCATED_DOUBLE_DATA_SOURCE

#include "AbstractDoubleSimpleFilter.h"

class TruncatedDoubleDataSource : public AbstractDoubleSimpleFilter
{
	Q_OBJECT

// specific to this class
	public:
		TruncatedDoubleDataSource() : d_start(0), d_num_rows(0) {}
		bool setStartSkip(int n) {
			if (!d_inputs.value(0) || n<0 || n>d_inputs[0]->numRows()) return false;
			d_start = n;
			return true;
		}
		bool setNumRows(int n) {
			if (!d_inputs.value(0) || n<0 || d_start+n>=d_inputs[0]->numRows()) return false;
			d_num_rows = n;
			return true;
		}
	private:
		int d_start, d_num_rows;

// simplified filter interface
	public:
		virtual QString label() const {
			return d_inputs.value(0) ?
				QString("%1 [%2:%3]").arg(d_inputs.at(0)->label()).arg(d_start+1).arg(d_start+d_num_rows) :
				QString();
		}
		virtual int numRows() const { return d_num_rows; }
		virtual double valueAt(int row) const {
			if (row<0 || row>=d_num_rows) return 0;
			return d_inputs[0]->valueAt(d_start + row);
		}
};

#endif // ifndef TRUNCATED_DOUBLE_DATA_SOURCE
