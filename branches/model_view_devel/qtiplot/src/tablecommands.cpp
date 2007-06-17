/***************************************************************************
    File                 : tablecommands.cpp
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

#include "tablecommands.h"
#include "TableModel.h"
#include "StringColumnData.h"
#include "DoubleColumnData.h"
#include "DateTimeColumnData.h"
#include <QObject>

///////////////////////////////////////////////////////////////////////////
// class TableShowCommentsCmd
///////////////////////////////////////////////////////////////////////////
TableShowCommentsCmd::TableShowCommentsCmd( TableModel * model, bool show, QUndoCommand * parent )
 : QUndoCommand( parent ), d_new_state(show), d_model(model)
{
	if(show)
		setText(QObject::tr("show column comments"));
	else
		setText(QObject::tr("hide column comments"));
}

void TableShowCommentsCmd::redo()
{
	d_old_state = d_model->areCommentsShown();
	d_model->showComments(d_new_state);
}

void TableShowCommentsCmd::undo()
{
	d_model->showComments(d_old_state);
}

///////////////////////////////////////////////////////////////////////////
// end of class TableShowCommentsCmd
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// class TableSetColumnPlotDesignationCmd
///////////////////////////////////////////////////////////////////////////
TableSetColumnPlotDesignationCmd::TableSetColumnPlotDesignationCmd( TableModel * model, int col, AbstractDataSource::PlotDesignation pd , QUndoCommand * parent )
 : QUndoCommand( parent ), d_col(col), d_new_pd(pd), d_model(model)
{
		setText(QObject::tr("set column plot designation"));
}

void TableSetColumnPlotDesignationCmd::redo()
{
	d_old_pd = d_model->columnPlotDesignation(d_col);
	d_model->setColumnPlotDesignation(d_col, d_new_pd);
}

void TableSetColumnPlotDesignationCmd::undo()
{
	d_model->setColumnPlotDesignation(d_col, d_old_pd);
}

///////////////////////////////////////////////////////////////////////////
// end of class TableSetColumnPlotDesignationCmd
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// class TableSetColumnLabelCmd
///////////////////////////////////////////////////////////////////////////
TableSetColumnLabelCmd::TableSetColumnLabelCmd( TableModel * model, int col, const QString& label , QUndoCommand * parent )
 : QUndoCommand( parent ), d_col(col), d_new_label(label), d_model(model)
{
		setText(QObject::tr("set column label"));
}

void TableSetColumnLabelCmd::redo()
{
	d_old_label = d_model->columnLabel(d_col);
	d_model->setColumnLabel(d_col, d_new_label);
}

void TableSetColumnLabelCmd::undo()
{
	d_model->setColumnLabel(d_col, d_old_label);
}

///////////////////////////////////////////////////////////////////////////
// end of class TableSetColumnLabelCmd
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// class TableSetColumnCommentCmd
///////////////////////////////////////////////////////////////////////////
TableSetColumnCommentCmd::TableSetColumnCommentCmd( TableModel * model, int col, const QString& comment , QUndoCommand * parent )
 : QUndoCommand( parent ), d_col(col), d_new_comment(comment), d_model(model)
{
		setText(QObject::tr("set column comment"));
}

void TableSetColumnCommentCmd::redo()
{
	d_old_comment = d_model->columnComment(d_col);
	d_model->setColumnComment(d_col, d_new_comment);
}

void TableSetColumnCommentCmd::undo()
{
	d_model->setColumnComment(d_col, d_old_comment);
}

///////////////////////////////////////////////////////////////////////////
// end of class TableSetColumnCommentCmd
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// class TableClearColumnCmd
///////////////////////////////////////////////////////////////////////////
TableClearColumnCmd::TableClearColumnCmd( TableModel * model, int col, QUndoCommand * parent )
 : QUndoCommand( parent ), d_col(col), d_model(model)
{
	d_orig_col = 0;
	d_cleared_col = 0;
	setText(QObject::tr("clear column"));
}
TableClearColumnCmd::~TableClearColumnCmd()
{
	if(d_orig_col)
		delete d_orig_col;
	if(d_cleared_col)
		delete d_cleared_col;
}

void TableClearColumnCmd::redo()
{
	d_orig_col = d_model->columnPointer(d_col);

	if(!d_cleared_col) // no previous undo
	{
		// create a column of the correct type
		//if(dynamic_cast<StringColumnData *>(d_model->columnPointer(d_col)))
		if((d_model->columnPointer(d_col))->inherits("StringColumnData"))
			d_cleared_col = new StringColumnData();
		if(dynamic_cast<DoubleColumnData *>(d_model->columnPointer(d_col)))
		//else if((d_model->columnPointer(d_col))->inherits("DoubleColumnData"))
			d_cleared_col = new DoubleColumnData();
		else
			d_cleared_col = new DateTimeColumnData();

		// keep the designation, label and comment
		d_cleared_col->setPlotDesignation(d_orig_col->asDataSource()->plotDesignation());
		d_cleared_col->setLabel(d_orig_col->asDataSource()->label());
		d_cleared_col->setComment(d_orig_col->asDataSource()->comment());
		// keep the formulas
		QList< Interval<int> > formulas = d_orig_col->formulaIntervals();
		foreach(Interval<int> i, formulas)
			d_cleared_col->setFormula(i, d_orig_col->formula(i.start()));
	}
	// replace the column with the cleared one
	d_model->replaceColumn(d_col, d_cleared_col);
	d_cleared_col = 0; // don't delete the active col in dtor
}

void TableClearColumnCmd::undo()
{
	d_cleared_col = d_model->columnPointer(d_col);
	d_model->replaceColumn(d_col, d_orig_col);
	d_orig_col = 0; // don't delete the active col in dtor
}

///////////////////////////////////////////////////////////////////////////
// end of class TableClearColumnCmd
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// class TableUserInputCmd
///////////////////////////////////////////////////////////////////////////
TableUserInputCmd::TableUserInputCmd( TableModel * model, const QModelIndex& index, QUndoCommand * parent )
 : QUndoCommand( parent ), d_index(index), d_model(model)
{
	d_previous_undo = false;
	setText(QObject::tr("user input"));
}
TableUserInputCmd::~TableUserInputCmd()
{
}

void TableUserInputCmd::redo()
{
	if(d_previous_undo) 
		d_model->setData(d_index, d_new_data, Qt::EditRole); 
	else
		d_old_data = d_model->data(d_index, Qt::EditRole);
}

void TableUserInputCmd::undo()
{
		d_new_data = d_model->data(d_index, Qt::EditRole);
		d_model->setData(d_index, d_old_data, Qt::EditRole); 
		d_previous_undo = true;
}

///////////////////////////////////////////////////////////////////////////
// end of class TableUserInputCmd
///////////////////////////////////////////////////////////////////////////



