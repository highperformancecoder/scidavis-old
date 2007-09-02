/***************************************************************************
    File                 : columncommands.h
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Commands to change columns

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

#ifndef COLUMNCOMMANDS_H
#define COLUMNCOMMANDS_H

#include <QUndoCommand>
#include "Column.h"
#include "ColumnPrivate.h"
#include "core/AbstractFilter.h"
#include "lib/IntervalAttribute.h"

///////////////////////////////////////////////////////////////////////////
// class ColumnSetModeCmd
///////////////////////////////////////////////////////////////////////////
//! Set the column mode 
class ColumnSetModeCmd : public QUndoCommand
{
public:
	//! Ctor
	ColumnSetModeCmd(Column * col, SciDAVis::ColumnMode mode, QUndoCommand * parent = 0 );
	//! Dtor
	~ColumnSetModeCmd();

	//! Execute the command
	virtual void redo();
	//! Undo the command
	virtual void undo();

private:
	//! The column to modify
	Column * d_col;
	//! The previous mode
	SciDAVis::ColumnMode d_old_mode;	
	//! The new mode
	SciDAVis::ColumnMode d_mode;
	//! The old data type
	SciDAVis::ColumnDataType d_old_type;
	//! The new data type
	SciDAVis::ColumnDataType d_new_type;
	//! Pointer to old data
	void * d_old_data;
	//! Pointer to new data
	void * d_new_data;
	//! The new input filter
	AbstractFilter * d_new_in_filter;
	//! The new output filter
	AbstractFilter * d_new_out_filter;
	//! The old input filter
	AbstractFilter * d_old_in_filter;
	//! The old output filter
	AbstractFilter * d_old_out_filter;
	//! The old validity information
	IntervalAttribute<bool> d_old_validity;
	//! The new validity information
	IntervalAttribute<bool> d_new_validity;
	//! A status flag
	bool d_undone;
	//! A status flag
	bool d_executed;
};
///////////////////////////////////////////////////////////////////////////
// end of class ColumnSetModeCmd
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
// class ColumnFullCopyCmd
///////////////////////////////////////////////////////////////////////////
//! Copy a complete column 
class ColumnFullCopyCmd : public QUndoCommand
{
public:
	//! Ctor
	ColumnFullCopyCmd(Column * col, const AbstractColumn * src, QUndoCommand * parent = 0 );
	//! Dtor
	~ColumnFullCopyCmd();

	//! Execute the command
	virtual void redo();
	//! Undo the command
	virtual void undo();

private:
	//! The column to modify
	Column * d_col;
	//! The column to copy
	const  AbstractColumn * d_src;
	//! A backup column
	Column * d_backup;

};
///////////////////////////////////////////////////////////////////////////
// end of class ColumnFullCopyCmd
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// class ColumnPartialCopyCmd
///////////////////////////////////////////////////////////////////////////
//! Copy parts of a column
class ColumnPartialCopyCmd : public QUndoCommand
{
public:
	//! Ctor
	ColumnPartialCopyCmd(Column * col, const AbstractColumn * src, int src_start, int dest_start, int num_rows, QUndoCommand * parent = 0 );
	//! Dtor
	~ColumnPartialCopyCmd();

	//! Execute the command
	virtual void redo();
	//! Undo the command
	virtual void undo();

private:
	//! The column to modify
	Column * d_col;
	//! The column to copy
	const AbstractColumn * d_src;
	//! A backup of the orig. column
	Column * d_col_backup;
	//! A backup of the source column
	Column * d_src_backup;
	//! Start index in source column
	int d_src_start;
	//! Start index in destination column
	int d_dest_start;
	//! Number of rows to copy
	int d_num_rows;
	

};
///////////////////////////////////////////////////////////////////////////
// end of class ColumnPartialCopyCmd
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
// class ColumnInsertRowsCmd
///////////////////////////////////////////////////////////////////////////
//! Insert empty rows 
class ColumnInsertRowsCmd : public QUndoCommand
{
public:
	//! Ctor
	ColumnInsertRowsCmd(Column * col, int before, int count, QUndoCommand * parent = 0 );
	//! Dtor
	~ColumnInsertRowsCmd();

	//! Execute the command
	virtual void redo();
	//! Undo the command
	virtual void undo();

private:
	//! The column to modify
	Column * d_col;
	//! Row to insert before
	int d_before;
	//! Number of rows
	int d_count;

};
///////////////////////////////////////////////////////////////////////////
// end of class ColumnInsertRowsCmd
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// class ColumnRemoveRowsCmd
///////////////////////////////////////////////////////////////////////////
//!  
class ColumnRemoveRowsCmd : public QUndoCommand 
{
public:
	//! Ctor
	ColumnRemoveRowsCmd(Column * col, int first, int count, QUndoCommand * parent = 0 );
	//! Dtor
	~ColumnRemoveRowsCmd();

	//! Execute the command
	virtual void redo();
	//! Undo the command
	virtual void undo();

private:
	//! The column to modify
	Column * d_col;
	//! The first row
	int d_first;
	//! The number of rows to be removed
	int d_count;
	//! Column saving the removed rows
	Column * d_backup;
};
///////////////////////////////////////////////////////////////////////////
// end of class ColumnRemoveRowsCmd
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// class ColumnSetPlotDesignationCmd
///////////////////////////////////////////////////////////////////////////
//! Sets a column's plot designation
class ColumnSetPlotDesignationCmd : public QUndoCommand
{
public:
	//! Ctor
	ColumnSetPlotDesignationCmd(Column * col, SciDAVis::PlotDesignation pd, QUndoCommand * parent = 0 );
	//! Dtor
	~ColumnSetPlotDesignationCmd();

	//! Execute the command
	virtual void redo();
	//! Undo the command
	virtual void undo();

private:
	//! The column to modify
	Column * d_col;
	//! New plot designation
	SciDAVis::PlotDesignation d_new_pd;
	//! Old plot designation
	SciDAVis::PlotDesignation d_old_pd;
};
///////////////////////////////////////////////////////////////////////////
// end of class ColumnSetPlotDesignationCmd
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
// class ColumnClearCmd
///////////////////////////////////////////////////////////////////////////
//! Clear the column 
class ColumnClearCmd : public QUndoCommand
{
public:
	//! Ctor
	ColumnClearCmd(Column * col, QUndoCommand * parent = 0 );
	//! Dtor
	~ColumnClearCmd();

	//! Execute the command
	virtual void redo();
	//! Undo the command
	virtual void undo();

private:
	//! The column to modify
	Column * d_col;
	//! The column's data type
	SciDAVis::ColumnDataType d_type;
	//! Pointer to the old data pointer
	void * d_data;
	//! Pointer to an empty data vector
	void * d_empty_data;
	//! The old validity
	IntervalAttribute<bool> d_validity;
	//! Status flag
	bool d_undone;

};
///////////////////////////////////////////////////////////////////////////
// end of class ColumnClearCmd
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
// class ColumnClearValidityCmd
///////////////////////////////////////////////////////////////////////////
//! Clear validity information 
class ColumnClearValidityCmd : public QUndoCommand
{
public:
	//! Ctor
	ColumnClearValidityCmd(Column * col, QUndoCommand * parent = 0 );
	//! Dtor
	~ColumnClearValidityCmd();

	//! Execute the command
	virtual void redo();
	//! Undo the command
	virtual void undo();

private:
	//! The column to modify
	Column * d_col;
	//! The old validity
	IntervalAttribute<bool> d_validity;
	//! A status flag
	bool b_copied;

};
///////////////////////////////////////////////////////////////////////////
// end of class ColumnClearValidityCmd
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// class ColumnClearMasksCmd
///////////////////////////////////////////////////////////////////////////
//! Clear validity information 
class ColumnClearMasksCmd : public QUndoCommand
{
public:
	//! Ctor
	ColumnClearMasksCmd(Column * col, QUndoCommand * parent = 0 );
	//! Dtor
	~ColumnClearMasksCmd();

	//! Execute the command
	virtual void redo();
	//! Undo the command
	virtual void undo();

private:
	//! The column to modify
	Column * d_col;
	//! The old masks
	IntervalAttribute<bool> d_masking;
	//! A status flag
	bool b_copied;

};
///////////////////////////////////////////////////////////////////////////
// end of class ColumnClearMasksCmd
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
// class ColumnSetInvalidCmd
///////////////////////////////////////////////////////////////////////////
//! Mark an interval of rows as invalid 
class ColumnSetInvalidCmd : public QUndoCommand
{
public:
	//! Ctor
	ColumnSetInvalidCmd(Column * col, Interval<int> interval, bool invalid, QUndoCommand * parent = 0 );
	//! Dtor
	~ColumnSetInvalidCmd();

	//! Execute the command
	virtual void redo();
	//! Undo the command
	virtual void undo();

private:
	//! The column to modify
	Column * d_col;
	//! The interval
	Interval<int> d_interval;
	//! Valid/invalid flag
	bool d_invalid;
	//! Interval attribute backup
	IntervalAttribute<bool> d_validity;
	//! A status flag
	bool b_copied;

};
///////////////////////////////////////////////////////////////////////////
// end of class ColumnSetInvalidCmd
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// class ColumnSetMaskedCmd
///////////////////////////////////////////////////////////////////////////
//! Mark an interval of rows as masked
class ColumnSetMaskedCmd : public QUndoCommand
{
public:
	//! Ctor
	ColumnSetMaskedCmd(Column * col, Interval<int> interval, bool masked, QUndoCommand * parent = 0 );
	//! Dtor
	~ColumnSetMaskedCmd();

	//! Execute the command
	virtual void redo();
	//! Undo the command
	virtual void undo();

private:
	//! The column to modify
	Column * d_col;
	//! The interval
	Interval<int> d_interval;
	//! Mask/unmask flag
	bool d_masked;
	//! Interval attribute backup
	IntervalAttribute<bool> d_masking;
	//! A status flag
	bool b_copied;

};
///////////////////////////////////////////////////////////////////////////
// end of class ColumnSetMaskedCmd
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// class ColumnSetFormulaCmd
///////////////////////////////////////////////////////////////////////////
//! Set the formula for a given interval
class ColumnSetFormulaCmd : public QUndoCommand
{
public:
	//! Ctor
	ColumnSetFormulaCmd(Column * col, Interval<int> interval, QString formula, QUndoCommand * parent = 0 );
	//! Dtor
	~ColumnSetFormulaCmd();

	//! Execute the command
	virtual void redo();
	//! Undo the command
	virtual void undo();

private:
	//! The column to modify
	Column * d_col;
	//! The interval
	Interval<int> d_interval;
	//! The new formula
	QString d_formula;
	//! Interval attribute backup
	IntervalAttribute<QString> d_formulas;
	//! A status flag
	bool b_copied;

};
///////////////////////////////////////////////////////////////////////////
// end of class ColumnSetFormulaCmd
///////////////////////////////////////////////////////////////////////////

#endif
