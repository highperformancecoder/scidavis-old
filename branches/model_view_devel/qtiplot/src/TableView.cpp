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
#include "TableDataModel.h"

TableView::TableView(QWidget * parent, TableDataModel * model )
 : QTableView( parent )
{
    QItemSelectionModel * selections = new QItemSelectionModel(model);
	setModel(model);
	setSelectionModel(selections);
	
    setContextMenuPolicy(Qt::DefaultContextMenu);
}


TableView::~TableView() 
{
}


void TableView::contextMenuEvent(QContextMenuEvent *)
{
    qDebug("void TableView::contextMenuEvent()");
    
    return ;
}


QSize TableView::minimumSizeHint () const
{
	// This size will be used for new windows and when cascading etc.
	return QSize(640,480);
}
