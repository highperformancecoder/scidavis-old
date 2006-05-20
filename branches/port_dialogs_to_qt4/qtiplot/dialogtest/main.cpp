/***************************************************************************
    File                 : main.cpp
    Project              : QtiPlot
    --------------------------------------------------------------------
    Date created         : 13.05.2006 14:15:01 CEST
    Copyright            : (C) 2006 by Tilman Hoener zu Siederdissen
    Email                : thzs@gmx.net
    Description          : Test application to help me port QtiPlot's dialogs
                           
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

#include <QApplication>
#include <textDialog.h>
#include <plotWizard.h>
#include <importDialog.h>

#include <QStringList>
#include <QString>


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );	

	TextDialog dlg(TextDialog::TextMarker);
	dlg.setText("this text should not be visible");	
	dlg.setText("enter text here\n(multiline supported)");	
	dlg.exec();

/*
	TextDialog dlg2(TextDialog::AxisTitle);
	dlg2.setText("enter text here\n(multiline supported)");	
	dlg2.exec();
	PlotWizard dlg3;
*/
/*
	QStringList tables;
	tables.append("table 1");
	tables.append("table 2");
	QStringList columns;
	columns.append("table 1_col1");
	columns.append("table 1_col2");
	columns.append("table 2_colA");
	columns.append("table 2_colB");
	dlg3.insertTablesList(tables);
	dlg3.setColumnsList(columns);
	dlg3.changeColumnsList(tables[0]);
	dlg3.exec();
*/
/*	
	ImportDialog dlg4;
	dlg4.exec();
*/	
	//return app.exec();
	return 0;
}

