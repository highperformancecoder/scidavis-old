/***************************************************************************
    File                 : AbstractDoubleSimpleFilter.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Knut Franke
    Email (use @ for *)  : knut.franke*gmx.de
    Description          : Simplified filter interface for filters with
                           only one double-typed output port.

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
#ifndef ABSTRACT_DOUBLE_SIMPLE_FILTER
#define ABSTRACT_DOUBLE_SIMPLE_FILTER

#include "AbstractFilter.h"
#include "AbstractDoubleDataSource.h"

/**
 * \brief Simplified filter interface for filters with only one double-typed output port.
 *
 * This class is only meant to simplify implementation of a restricted subtype of filter.
 * It should not be used as type for variables, which should always use either
 * AbstractFilter or (if necessary) an actual (non-abstract) implementation.
 */
class AbstractDoubleSimpleFilter : public AbstractDoubleDataSource, public AbstractFilter
{
	Q_OBJECT
	public:
		virtual int numInputs() const { return 1; }
		virtual int numOutputs() const { return 1; }
		virtual const AbstractDataSource* output(int port) const { return port == 0 ? this : 0; }
		virtual PlotDesignation plotDesignation() const { return d_inputs.value(0) ? d_inputs[0]->plotDesignation() : noDesignation; }

	protected:
		virtual void inputDescriptionAboutToChange(AbstractDataSource*) { emit descriptionAboutToChange(this); }
		virtual void inputDescriptionChanged(AbstractDataSource*) { emit descriptionChanged(this); }
		virtual void inputPlotDesignationAboutToChange(AbstractDataSource*) { emit plotDesignationAboutToChange(this); }
		virtual void inputPlotDesignationChanged(AbstractDataSource*) { emit plotDesignationChanged(this); }
		virtual void inputDataAboutToChange(AbstractDataSource*) { emit dataAboutToChange(this); }
		virtual void inputDataChanged(AbstractDataSource*) { emit dataChanged(this); }
};

#endif // ifndef ABSTRACT_DOUBLE_SIMPLE_FILTER

