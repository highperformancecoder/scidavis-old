/***************************************************************************
    File                 : tablecommands.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Commands used in Table (part of the undo/redo framework)
                           
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

#include <QUndoCommand>
#include <QAbstractItemModel>
#include <QModelIndex>
class TableModel;
#include "AbstractDataSource.h"
#include "AbstractColumnData.h"
#include "AbstractFilter.h"

///////////////////////////////////////////////////////////////////////////
// class TableShowCommentsCmd
///////////////////////////////////////////////////////////////////////////
//! Toggles showing of column comments
class TableShowCommentsCmd : public QUndoCommand
{
public:
	TableShowCommentsCmd(TableModel * model, bool show, QUndoCommand * parent = 0 );

	virtual void redo();
	virtual void undo();

private:
	//! New flag state
	bool d_new_state;
	//! Old flag state
	bool d_old_state;
	//! The model to modify
	TableModel * d_model;
};

///////////////////////////////////////////////////////////////////////////
// end of class TableShowCommentsCmd
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// class TableSetColumnPlotDesignationCmd
///////////////////////////////////////////////////////////////////////////
//! Sets a column's plot designation
class TableSetColumnPlotDesignationCmd : public QUndoCommand
{
public:
	TableSetColumnPlotDesignationCmd(TableModel * model, int col, AbstractDataSource::PlotDesignation pd, QUndoCommand * parent = 0 );

	virtual void redo();
	virtual void undo();

private:
	//! The changed column's index
	int d_col;
	//! New plot designation
	AbstractDataSource::PlotDesignation d_new_pd;
	//! Old plot designation
	AbstractDataSource::PlotDesignation d_old_pd;
	//! The model to modify
	TableModel * d_model;
};

///////////////////////////////////////////////////////////////////////////
// end of class TableSetColumnPlotDesignationCmd
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// class TableSetColumnLabelCmd
///////////////////////////////////////////////////////////////////////////
//! Sets a column's label
class TableSetColumnLabelCmd : public QUndoCommand
{
public:
	TableSetColumnLabelCmd(TableModel * model, int col, const QString& label, QUndoCommand * parent = 0 );

	virtual void redo();
	virtual void undo();

private:
	//! The changed column's index
	int d_col;
	//! New column label
	QString d_new_label;
	//! Old column label
	QString d_old_label;
	//! The model to modify
	TableModel * d_model;
};

///////////////////////////////////////////////////////////////////////////
// end of class TableSetColumnLabelCmd
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// class TableSetColumnCommentCmd
///////////////////////////////////////////////////////////////////////////
//! Sets a column's comment
class TableSetColumnCommentCmd : public QUndoCommand
{
public:
	TableSetColumnCommentCmd(TableModel * model, int col, const QString& comment, QUndoCommand * parent = 0 );

	virtual void redo();
	virtual void undo();

private:
	//! The changed column's index
	int d_col;
	//! New column comment
	QString d_new_comment;
	//! Old column comment
	QString d_old_comment;
	//! The model to modify
	TableModel * d_model;
};

///////////////////////////////////////////////////////////////////////////
// end of class TableSetColumnCommentCmd
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// class TableClearColumnCmd
///////////////////////////////////////////////////////////////////////////
//! Removes all data from a column
/**
 * This command removes all data from a column except the label, the comment,
 * the plot designation and the formulas. It replaces the column with an
 * empty one, therefore the old column never needs to be copied. So this command
 * should be pretty fast in redo() as well as undo().
 */
class TableClearColumnCmd : public QUndoCommand
{
public:
	TableClearColumnCmd( TableModel * model, int col, QUndoCommand * parent = 0 );
	~TableClearColumnCmd();

	virtual void redo();
	virtual void undo();

private:
	//! The cleared column's index
	int d_col;
	//! The original column
	AbstractColumnData * d_orig_col;
	//! The cleared column
	AbstractColumnData * d_cleared_col;
	//! The model to modify
	TableModel * d_model;
};

///////////////////////////////////////////////////////////////////////////
// end of class TableClearColumnCmd
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// class TableUserInputCmd
///////////////////////////////////////////////////////////////////////////
//! Handles user input
class TableUserInputCmd : public QUndoCommand
{
public:
	TableUserInputCmd( TableModel * model, const QModelIndex& index, QUndoCommand * parent = 0 );
	~TableUserInputCmd();

	virtual void redo();
	virtual void undo();

private:
	//! The model index of the edited cell
	QModelIndex d_index;
	//! The previous data
	QVariant d_old_data;
	//! The previous data
	QVariant d_new_data;
	//! The state flag 
	/**
	 * true means that his command has been undone at least once
	 */
	bool d_previous_undo;
	//! The model to modify
	TableModel * d_model;
};

///////////////////////////////////////////////////////////////////////////
// end of class TableUserInputCmd
///////////////////////////////////////////////////////////////////////////

