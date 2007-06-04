/***************************************************************************
    File                 : ReadOnlyTableModel.cpp
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
                           Knut Franke
    Email (use @ for *)  : thzs*gmx.net,
                           knut.franke*gmx.de
    Description          : Model to display the output of a filter

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

#include "ReadOnlyTableModel.h"

Qt::ItemFlags ReadOnlyTableModel::flags(const QModelIndex & index ) const
{
	if (index.isValid())
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
	else
		return Qt::ItemIsEnabled;
}


QVariant ReadOnlyTableModel::data(const QModelIndex &index, int role) const
{
	if( !index.isValid() ||
			((role != Qt::DisplayRole) && (role != Qt::EditRole) && (role != Qt::ToolTipRole) ) ||
			!d_inputs.value(index.column()) )
		return QVariant();

	return QVariant(d_inputs.at(index.column())->textAt(index.row()));
}




QVariant ReadOnlyTableModel::headerData(int section, Qt::Orientation orientation, int role) const 
{
	if ((role != Qt::DisplayRole) && (role != Qt::EditRole) && (role != Qt::ToolTipRole) )
		return QVariant();
	switch(orientation) 
	{
		case Qt::Horizontal: 
			return d_inputs.value(section) ? d_inputs.at(section)->label() : QVariant();
		case Qt::Vertical: 
			return QVariant(QString::number(section+1));
	}
}

int ReadOnlyTableModel::rowCount(const QModelIndex &) const 
{
	int rows, result = 0;
	foreach(AbstractDataSource *i, d_inputs) 
	{
		if (!i) continue;
		if ((rows = i->numRows()) > result)
			result = rows;
	}
	return result;
}

int ReadOnlyTableModel::columnCount(const QModelIndex &) const 
{ 
	return d_inputs.size(); 
}

QModelIndex ReadOnlyTableModel::index(int row, int column, const QModelIndex &parent) const 
{
	Q_UNUSED(parent)
	return createIndex(row, column);
}

QModelIndex ReadOnlyTableModel::parent(const QModelIndex &) const
{ 
	return QModelIndex(); 
}

int ReadOnlyTableModel::numInputs() const 
{ 	
	return -1; 
}

int ReadOnlyTableModel::numOutputs() const 
{ 
	return 0; 
}

AbstractDataSource* ReadOnlyTableModel::output(int) const 
{ 
	return 0; 
}

void ReadOnlyTableModel::inputDataChanged(int port) 
{
	emit dataChanged(createIndex(0,port), createIndex(d_inputs[port]->numRows()-1,port));
}

void ReadOnlyTableModel::inputDescriptionChanged(int port) 
{
	emit headerDataChanged(Qt::Horizontal, port, port);
}


