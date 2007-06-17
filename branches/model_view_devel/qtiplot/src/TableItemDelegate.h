/***************************************************************************
    File                 : TableItemDelegate.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Item delegate for TableView

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

#include <QItemDelegate>

//! Item delegate for TableView
/**
 * This class makes sure the undo system of
 * is notified when a user finished editing
 * a cell.
 */ 
class TableItemDelegate : public QItemDelegate
{
    Q_OBJECT

public:
	//! Wrapper function that emits userInput()
	virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

signals:
	//! User finished editing a cell
	void userInput(const QModelIndex& index) const; 

};
