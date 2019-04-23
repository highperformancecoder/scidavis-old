/***************************************************************************
    File                 : AbstractColumn.h
    Project              : SciDAVis
    Description          : Interface definition for data with column logic
    --------------------------------------------------------------------
    Copyright            : (C) 2007-2009 Tilman Benkert (thzs*gmx.net)
                           (replace * with @ in the email addresses) 

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

#ifndef ABSTRACTCOLUMN_H
#define ABSTRACTCOLUMN_H

#include <QList>
#include <QString>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include "lib/Interval.h"
#include "globals.h"
#include "core/AbstractAspect.h"
#include "Qt.h"

class Column;
class AbstractSimpleFilter;
class QStringList;

//! Interface definition for data with column logic
/**
  This is an abstract base class for column-based data, 
  i.e. mathematically a vector or technically a 1D array or list.
  It only defines the interface but has no data members itself. 

  Classes derived from this are typically table columns or outputs
  of filters which can be chained between table columns and plots. 
  From the point of view of the plot functions there will be no difference 
  between a table column and a filter output since both use this interface.

  Classes derived from this will either store a 
  vector with entries of one certain data type, e.g. double, QString, 
  QDateTime, or generate such values on demand. To determine the data
  type of a class derived from this, use the dataType() function. 
  AbstractColumn defines all access functions for all supported data 
  types but only those corresponding to the return value of dataType() 
  will return a meaningful value. Calling functions not belonging to 
  the data type of the column is safe, but will do nothing (writing
  function) or return some default value (reading functions).

  This class also defines all signals which indicate a data change.
  Any class whose output values are subject to change over time must emit
  the according signals. These signals notify any object working with the
  column before and after a change of the column.
  In some cases it will be necessary for a class using 
  the column to connect aboutToBeDestroyed(), to react 
  to a column's deletion, e.g. a filter's reaction to a 
  table deletion.

  All writing functions have a "do nothing" standard implementation to
  make deriving a read-only class very easy without bothering about the
  writing interface. 
  */
class AbstractColumn : public AbstractAspect
{
  Q_OBJECT

public:
  //! Ctor
  AbstractColumn(const QString& name) : AbstractAspect(name) {}
  //! Dtor
  virtual ~AbstractColumn() { aboutToBeDestroyed(this);}

  //! Return the data type of the column
  virtual SciDAVis::ColumnDataType dataType() const = 0;
  /*
   * This function is most used by tables but can also be used
   * by plots. The column mode specifies how to interpret 
   * the values in the column additional to the data type.
   */ 
  virtual SciDAVis::ColumnMode columnMode() const = 0;
  //! Return the data vector size
  virtual int rowCount() const = 0;
  //! Return the column plot designation
  virtual SciDAVis::PlotDesignation plotDesignation() const = 0;
  //! \name IntervalAttribute related functions
  /// @{
  //! Return whether a certain row contains an invalid value 	 
  virtual bool isInvalid(int row) const { return !Interval<int>(0, rowCount()-1).contains(row); } 	 
  //! Return whether a certain interval of rows contains only invalid values 	 
  virtual bool isInvalidInterval(Interval<int> i) const { return !Interval<int>(0, rowCount()-1).contains(i); } 	 
  //! Return all intervals of invalid rows
  virtual QList< Interval<int> > invalidIntervals() const { return QList< Interval<int> >(); } 	 
  //! Return whether a certain row is masked 	 
  virtual bool isMasked(int row) const { Q_UNUSED(row); return false; } 	 
  //! Return whether a certain interval of rows rows is fully masked 	 
  virtual bool isMasked(Interval<int> i) const { Q_UNUSED(i); return false; }
  //! Return all intervals of masked rows
  virtual QList< Interval<int> > maskedIntervals() const { return QList< Interval<int> >(); } 	 
  /// @}
  /**
   * Use this only when dataType() is QString
   */
  virtual std::string textAt(int row) const { Q_UNUSED(row); return ""; }
  /**
   * Use this only when dataType() is QDateTime
   */
  virtual QtCore::QDate dateAt(int row) const { Q_UNUSED(row); return QDate(); };
  /**
   * Use this only when dataType() is QDateTime
   */
  virtual QtCore::QTime timeAt(int row) const { Q_UNUSED(row); return QTime(); };
  /**
   * Use this only when dataType() is QDateTime
   */
  virtual QtCore::QDateTime dateTimeAt(int row) const { Q_UNUSED(row); return QDateTime(); };
  /**
   * Use this only when dataType() is double
   */
  virtual double valueAt(int row) const { Q_UNUSED(row); return 0; };

signals: 
  //! Column plot designation will be changed
  /**
   * 'source' is always the this pointer of the column that
   * emitted this signal. This way it's easier to use
   * one handler for lots of columns.
   */
  void plotDesignationAboutToChange(const AbstractColumn * source); 
  //! Column plot designation changed
  /**
   * 'source' is always the this pointer of the column that
   * emitted this signal. This way it's easier to use
   * one handler for lots of columns.
   */
  void plotDesignationChanged(const AbstractColumn * source); 
  //! Column mode (possibly also the data type) will be changed
  /**
   * 'source' is always the this pointer of the column that
   * emitted this signal. This way it's easier to use
   * one handler for lots of columns.
   */
  void modeAboutToChange(const AbstractColumn * source); 
  //! Column mode (possibly also the data type) changed
  /**
   * 'source' is always the this pointer of the column that
   * emitted this signal. This way it's easier to use
   * one handler for lots of columns.
   */
  void modeChanged(const AbstractColumn * source); 
  //! Data (including validity) of the column will be changed
  /**
   * 'source' is always the this pointer of the column that
   * emitted this signal. This way it's easier to use
   * one handler for lots of columns.
   */
  void dataAboutToChange(const AbstractColumn * source); 
  //! Data (including validity) of the column has changed
  /**
   * Important: When data has changed also the number
   * of rows in the column may have changed without
   * any other signal emission.
   * 'source' is always the this pointer of the column that
   * emitted this signal. This way it's easier to use
   * one handler for lots of columns.
   */
  void dataChanged(const AbstractColumn * source); 
  //! The column will be replaced
  /**
   * This is used then a column is replaced by another
   * column, possibly of another type. 
   *
   * \param new_col Pointer to the column this one is to be replaced with.
   *
   * \param source is always a pointer to the column that
   * emitted this signal. This way it's easier to use
   * one handler for lots of columns.
   */
  void aboutToBeReplaced(const AbstractColumn * source, const AbstractColumn* new_col); 
  //! Rows will be inserted
  /**
   *	\param source the column that emitted the signal
   *	\param before the row to insert before
   *	\param count the number of rows to be inserted
   */
  void rowsAboutToBeInserted(const AbstractColumn * source, int before, int count); 
  //! Rows have been inserted
  /**
   *	\param source the column that emitted the signal
   *	\param before the row to insert before
   *	\param count the number of rows to be inserted
   */
  void rowsInserted(const AbstractColumn * source, int before, int count); 
  //! Rows will be deleted
  /**
   *	\param source the column that emitted the signal
   *	\param first the first row to be deleted
   *	\param count the number of rows to be deleted
   */
  void rowsAboutToBeRemoved(const AbstractColumn * source, int first, int count); 
  //! Rows have been deleted
  /**
   *	\param source the column that emitted the signal
   *	\param first the first row that was deleted
   *	\param count the number of deleted rows
   */
  void rowsRemoved(const AbstractColumn * source, int first, int count); 
  //! IntervalAttribute related signal
  void maskingAboutToChange(const AbstractColumn * source); 
  //! IntervalAttribute related signal
  void maskingChanged(const AbstractColumn * source); 
  // TODO: Check whether aboutToBeDestroyed is needed 
  //! Emitted shortly before this data source is deleted.
  /**
   * \param source the object emitting this signal
   *
   * This is needed by AbstractFilter. 
   */
  void aboutToBeDestroyed(const AbstractColumn * source);

  friend class ColumnPrivate;
  friend class AbstractSimpleFilter;
  friend class SimpleMappingFilter;
  friend class SimpleCopyThroughFilter;
};

#endif
