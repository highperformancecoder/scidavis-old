/***************************************************************************
    File                 : AbstractSimpleFilter.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Knut Franke
    Email (use @ for *)  : knut.franke*gmx.de
    Description          : Simplified filter interface for filters with
                           only one output port.

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
#ifndef ABSTRACT_SIMPLE_FILTER
#define ABSTRACT_SIMPLE_FILTER

#include "AbstractFilter.h"
#include "AbstractDoubleDataSource.h"
#include "AbstractStringDataSource.h"
#include "AbstractDateTimeDataSource.h"

template<class T> class AbstractDataSourceTemplate {};
template<> class AbstractDataSourceTemplate<double> : public AbstractDoubleDataSource {};
template<> class AbstractDataSourceTemplate<QString> : public AbstractStringDataSource {};
template<> class AbstractDataSourceTemplate<QDateTime> : public AbstractDateTimeDataSource {};

/**
 * \brief Simplified filter interface for filters with only one output port.
 *
 * This class is only meant to simplify implementation of a restricted subtype of filter.
 * It should not be used as type for variables, which should always use either
 * AbstractFilter or (if necessary) an actual (non-abstract) implementation.
 *
 * The trick here is that, in a sense, the filter is its own output port. This means you
 * can implement a complete filter in only one class and don't have to coordinate data
 * transfer between a filter class and a data source class.
 * Additionaly, AbstractSimpleFilter offers some useful convenience methods which make writing
 * filters as painless as possible.
 *
 * For the template argument, only double, QString and QDateTime are supported.
 */
template<class T> class AbstractSimpleFilter : public AbstractDataSourceTemplate<T>, public AbstractFilter
{
	public:
		//! Default to one input port.
		virtual int numInputs() const { return 1; }
		//! We manage only one output port (don't override unless you really know what you are doing).
		virtual int numOutputs() const { return 1; }
		//! Return a pointer to myself on port 0 (don't override unless you really know what you are doing).
		virtual AbstractDataSource* output(int port) const {
			return port == 0 ? const_cast<AbstractSimpleFilter<T>*>(this) : 0;
		}
		//! Copy label of input port 0.
		virtual QString label() const {
			return d_inputs.value(0) ? d_inputs.at(0)->label() : QString();
		}
		//! Copy plot designation of input port 0.
		virtual AbstractDataSource::PlotDesignation plotDesignation() const {
			return d_inputs.value(0) ?
				d_inputs.at(0)->plotDesignation() :
				AbstractDataSource::noDesignation;
		}
		//! Assume a 1:1 correspondence between input and output rows.
		virtual int numRows() const {
			return d_inputs.value(0) ? d_inputs.at(0)->numRows() : 0;
		}

	protected:
		/**
		 * \brief Only use this if you are sure that an AbstractDoubleDataSource is connected to the given port.
		 *
		 * The standard way of ensuring this is reimplementing AbstractFilter::inputAcceptable():
		 *
		 * <code>
		 * virtual bool inputAcceptable(int, AbstractDataSource *source) {
		 * 	return source->inherits("AbstractDoubleDataSource");
		 * }
		 * </code>
		 */
		AbstractDoubleDataSource *doubleInput(int port=0) const {
			return static_cast<AbstractDoubleDataSource*>(d_inputs.value(port));
		}
		/**
		 * \brief Only use this if you are sure that an AbstractStringDataSource is connected to the given port.
		 *
		 * The standard way of ensuring this is reimplementing AbstractFilter::inputAcceptable():
		 *
		 * <code>
		 * virtual bool inputAcceptable(int, AbstractDataSource *source) {
		 * 	return source->inherits("AbstractStringDataSource");
		 * }
		 * </code>
		 */
		AbstractStringDataSource *stringInput(int port=0) const {
			return static_cast<AbstractStringDataSource*>(d_inputs.value(port));
		}
		/**
		 * \brief Only use this if you are sure that an AbstractDateTimeDataSource is connected to the given port.
		 *
		 * The standard way of ensuring this is reimplementing AbstractFilter::inputAcceptable():
		 *
		 * <code>
		 * virtual bool inputAcceptable(int, AbstractDataSource *source) {
		 * 	return source->inherits("AbstractDateTimeDataSource");
		 * }
		 * </code>
		 */
		AbstractDateTimeDataSource *dateTimeInput(int port=0) const {
			return static_cast<AbstractDateTimeDataSource*>(d_inputs.value(port));
		}

		virtual void inputDescriptionAboutToChange(AbstractDataSource*) { emit descriptionAboutToChange(this); }
		virtual void inputDescriptionChanged(AbstractDataSource*) { emit descriptionChanged(this); }
		virtual void inputPlotDesignationAboutToChange(AbstractDataSource*) { emit plotDesignationAboutToChange(this); }
		virtual void inputPlotDesignationChanged(AbstractDataSource*) { emit plotDesignationChanged(this); }
		virtual void inputDataAboutToChange(AbstractDataSource*) { emit dataAboutToChange(this); }
		virtual void inputDataChanged(AbstractDataSource*) { emit dataChanged(this); }
};

#endif // ifndef ABSTRACT_SIMPLE_FILTER

