/***************************************************************************
    File                 : TruncationFilter.h
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
#ifndef TRUNCATION_FILTER
#define TRUNCATION_FILTER

#include "AbstractSimpleFilter.h"

/**
 * \brief A simple filter which truncates its (single) input.
 *
 * After connecting a data source to its input port, you have to tell TruncationFilter
 * which (continuous) subset of the input to return by calling setStartSkip() and setNumRows().
 */
template<class T> class TruncationFilter : public AbstractSimpleFilter<T>
{
// specific to this class
	public:
		TruncationFilter() : d_start(0), d_num_rows(0) {}
		bool setStartSkip(int n) {
			if (!AbstractSimpleFilter<T>::d_inputs.value(0) || n<0 || n>AbstractSimpleFilter<T>::d_inputs[0]->numRows()) return false;
			d_start = n;
			return true;
		}
		bool setNumRows(int n) {
			if (!AbstractSimpleFilter<T>::d_inputs.value(0) || n<0 || d_start+n>=AbstractSimpleFilter<T>::d_inputs[0]->numRows()) return false;
			d_num_rows = n;
			return true;
		}
	private:
		int d_start, d_num_rows;

// simplified filter interface
	public:
		virtual QString label() const {
			return AbstractSimpleFilter<T>::d_inputs.value(0) ?
				QString("%1 [%2:%3]").arg(AbstractSimpleFilter<T>::d_inputs.at(0)->label()).arg(d_start+1).arg(d_start+d_num_rows) :
				QString();
		}
		virtual int numRows() const { return d_num_rows; }
		virtual double valueAt(int row) const {
			if (row<0 || row>=d_num_rows) return 0;
			return AbstractSimpleFilter<T>::d_inputs[0]->valueAt(d_start + row);
		}
		virtual QString textAt(int row) const {
			if (row<0 || row>=d_num_rows) return 0;
			return AbstractSimpleFilter<T>::d_inputs[0]->textAt(d_start + row);
		}
		virtual void inputDataChanged(AbstractDataSource* source)  {
			if (!AbstractSimpleFilter<T>::d_inputs.value(0) || d_start+d_num_rows>=AbstractSimpleFilter<T>::d_inputs[0]->numRows())
				d_num_rows = 0;
			AbstractSimpleFilter<T>::inputDataChanged(source);
		}
};

#endif // ifndef TRUNCATION_FILTER
