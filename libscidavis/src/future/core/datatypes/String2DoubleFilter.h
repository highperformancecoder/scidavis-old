/***************************************************************************
    File                 : String2DoubleFilter.h
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Knut Franke
    Email (use @ for *)  : knut.franke*gmx.de
    Description          : Locale-aware conversion filter QString -> double.
                           
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

#include "../AbstractSimpleFilter.h"
#include <QLocale>
#include "lib/XmlStreamReader.h"
#include <QXmlStreamWriter>
#include <QtDebug>
#include <QSettings>

//! Locale-aware conversion filter QString -> double.
class String2DoubleFilter : public AbstractSimpleFilter
{
	Q_OBJECT

	public:
		String2DoubleFilter() : d_use_default_locale(true) {}
		// I'm not sure what this locale is, I searched through the whole source
		// code and found zero references to the next two functions.
		void setNumericLocale(QLocale locale) { d_numeric_locale = locale; d_use_default_locale = false; }
		void setNumericLocaleToDefault() { d_use_default_locale = true; }

		// Since valueAt() and isInvalid() do the same thing except the &ok part,
		// I see reason to make a helper function and call it appropriately.
       	// Please see convertToDouble() and getLocale() below
		virtual double valueAt(int row) const {
			if (!d_inputs.value(0)) return 0;
			// I prefer to initialize doubles with this to catch troubles
			// if it is inappropriate for some reason, it may be left uninitialized
			double val=std::numeric_limits<double>::quiet_NaN();
			convertToDouble(d_inputs.value(0)->textAt(row), val);
			return val;
		}
		virtual bool isInvalid(int row) const { 
			if (!d_inputs.value(0)) return false;
			double val=std::numeric_limits<double>::quiet_NaN();
			return !convertToDouble(d_inputs.value(0)->textAt(row), val);
		}
		virtual bool isInvalid(Interval<int> i) const {
			if (!d_inputs.value(0)) return false;
			double val=std::numeric_limits<double>::quiet_NaN();
			QLocale locale = getLocale();
			bool allowForeignSeparator = isAnyDecimalSeparatorAllowed();
			for (int row = i.start(); row <= i.end(); row++) {
				if (convertToDouble(d_inputs.value(0)->textAt(row), 
						val, locale, allowForeignSeparator))
					return false;
			}
			return true;
		}
		virtual QList< Interval<int> > invalidIntervals() const 
		{
			IntervalAttribute<bool> validity;
			if (d_inputs.value(0)) {
				int rows = d_inputs.value(0)->rowCount();
				for (int i=0; i<rows; i++) 
					validity.setValue(i, isInvalid(i));
			}
			return validity.intervals();
		}


		//! Return the data type of the column
		virtual SciDAVis::ColumnDataType dataType() const { return SciDAVis::TypeDouble; }

	protected:
		//! Using typed ports: only string inputs are accepted.
		virtual bool inputAcceptable(int, const AbstractColumn *source) {
			return source->dataType() == SciDAVis::TypeQString;
		}

	private:
		QLocale getLocale() const {
		    if (d_use_default_locale)
		        return QLocale(); // new QLocale instance in case it was changed between calls
				else
		        return d_numeric_locale;
		}

	 	bool isAnyDecimalSeparatorAllowed() const
		{
	#ifdef Q_OS_MAC // Mac
		QSettings settings(QSettings::IniFormat,QSettings::UserScope, "SciDAVis", "SciDAVis");
	#else
		QSettings settings(QSettings::NativeFormat,QSettings::UserScope, "SciDAVis", "SciDAVis");
	#endif
		return settings.value("/General/UseForeignSeparator").toBool();
		}

		QLocale d_numeric_locale;
		bool d_use_default_locale;

		// convenience overload
		bool convertToDouble(const QString& str, double& value) const {
		    return convertToDouble(str, value, getLocale(), isAnyDecimalSeparatorAllowed());
		}

		bool convertToDouble(const QString& str, double& value, const QLocale& locale,
				             const bool accept_any_decimal_separator) const {
		    bool ok;
		    auto tstr = QString(str);
		    if (accept_any_decimal_separator)
		    {
		        QChar decimalSeparator = locale.decimalPoint();    // get the decimal separator for this locale
		        QChar foreignSeparator = decimalSeparator;         // safeguard initialization just in case there are other decimal separators.
		        if ('.'==decimalSeparator)
		            foreignSeparator = ',';
		        if (','==decimalSeparator)
		            foreignSeparator = '.';

		        tstr.replace(foreignSeparator, decimalSeparator);  // while I'm not sure about if it is good to replace every occurrence in the input string,
		                                                           // I think that it does not break anything: if the input value has single decimal
		                                                           // separator, the replacement is valid; if multiple separators present, but not desired,
		                                                           // then the conversion will not succeed anyway. If multiple separators present and desired,
		                                                           // as in QLocale::German(), then user should not enable this option anyway, because there is no
		                                                           // sense in "universal" decimal separator and no one could tell if 1.234.567 is just
		                                                           // ill-formed input or 1.234,567 with typo. The only other way is to use a regular expression
		                                                           // to parse the string, but then it would not be compatible with Qt::QLocale methods.
		    }
		    value = locale.toDouble(tstr, &ok);

		    return ok;
		}
};

#endif // ifndef STRING2DOUBLE_FILTER_H
