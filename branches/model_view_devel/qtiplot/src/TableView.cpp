/***************************************************************************
    File                 : TableView.cpp
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : View class for table data

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

#include "TableView.h"
#include "TableModel.h"
#include "TableItemDelegate.h"

#include "StringColumnData.h"
#include "DoubleColumnData.h"
#include "DateTimeColumnData.h"
#include "AbstractFilter.h"
#include "CopyThroughFilter.h"
#include "Double2StringFilter.h"
#include "String2DoubleFilter.h"
#include "DateTime2StringFilter.h"
#include "String2DateTimeFilter.h"

#include <QKeyEvent>
#include <QtDebug>

TableView::TableView(QWidget * parent, TableModel * model )
 : QTableView( parent ), d_model(model)
{
  //  QItemSelectionModel * selections = new QItemSelectionModel(model);
	setModel(model);
//	setSelectionModel(selections);
//TODO: The use of a custom item delegate may not be necessary
//	d_delegate = new TableItemDelegate;
//	setItemDelegate(d_delegate);
//	connect(d_delegate, SIGNAL(returnPressed()), this, SLOT(advanceCell()));

    setContextMenuPolicy(Qt::DefaultContextMenu);
}

TableView::TableView(QWidget * parent, TableModel * model, int rows, int columns )
 : QTableView( parent ), d_model(model)
{
  /*
  //  QItemSelectionModel * selections = new QItemSelectionModel(model);
	setModel(model);
	model->appendColumns(AbstractColumnData::Double, columns);
	model->appendRows(rows);
//	setSelectionModel(selections);
	d_delegate = new TableItemDelegate;
	setItemDelegate(d_delegate);
	connect(d_delegate, SIGNAL(returnPressed()), this, SLOT(advanceCell()));

    setContextMenuPolicy(Qt::DefaultContextMenu);
	*/
}

TableView::~TableView() 
{
	delete d_delegate;
}


void TableView::contextMenuEvent(QContextMenuEvent *)
{
    qDebug("void TableView::contextMenuEvent()");
    
    return ;
}
	
void TableView::keyPressEvent(QKeyEvent * event)
{
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
		advanceCell();
	QTableView::keyPressEvent(event);
}



QSize TableView::minimumSizeHint () const
{
	// This size will be used for new windows and when cascading etc.
	return QSize(640,480);
}

void TableView::advanceCell()
{
	QModelIndex idx = currentIndex();
    if(idx.row()+1 >= d_model->rowCount())
        d_model->appendRows(1);

	setCurrentIndex(idx.sibling(idx.row()+1, idx.column()));
}

