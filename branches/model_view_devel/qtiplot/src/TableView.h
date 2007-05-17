/***************************************************************************
    File                 : TableView.h
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


#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QWidget>
#include <QTableView>
#include <QMessageBox>
#include <QHeaderView>
#include <QSize>

class TableDataModel;

//! View class for table data
class TableView : public QTableView
{
    Q_OBJECT

public:
    //! Constructor
    TableView(QWidget * parent, TableDataModel * model );
    //! Destructor
    ~TableView();

	//! Overloaded function: give decent size hint
    QSize minimumSizeHint () const;
		
protected:
	//! overloaded function (cf. Qt documentation)
    virtual void contextMenuEvent( QContextMenuEvent * );

};

#endif
