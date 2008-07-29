/***************************************************************************
    File                 : Table.h
    Project              : SciDAVis
    Description          : Table worksheet class
    --------------------------------------------------------------------
    Copyright            : (C) 2006-2008 Tilman Benkert (thzs*gmx.net)
    Copyright            : (C) 2006-2008 Knut Franke (knut.franke*gmx.de)
    Copyright            : (C) 2006-2007 Ion Vasilief (ion_vasilief*yahoo.fr)
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
#ifndef TABLE_H
#define TABLE_H

#include <q3table.h>
#include <q3header.h>
#include <Q3ValueList>
#include <QVarLengthArray>

#include "Graph.h"
#include "MyWidget.h"
#include "ScriptingEnv.h"
#include "Script.h"
#include "future/table/future_Table.h"
#include "future/table/TableView.h"
#include "globals.h"

/*!\brief MDI window providing a spreadsheet table with column logic.
 *
 * \section future_plans Future Plans
 * Port to the Model/View approach used in Qt4 and get rid of the Qt3Support dependancy.
 * [ assigned to thzs ]
 */
class Table: public TableView, public scripted
{
    Q_OBJECT

public:
	future::Table *d_future_table;

// TODO: remove obsolete enum
//	enum PlotDesignation{All = -1, None = 0, X = 1, Y = 2, Z = 3, xErr = 4, yErr = 5};
	enum ColType{Numeric = 0, Text = 1, Date = 2, Time = 3, Month = 4, Day = 5};

   	Table(ScriptingEnv *env, const QString &fname,const QString &sep, int ignoredLines, bool renameCols,
		 bool stripSpaces, bool simplifySpaces, const QString &label,
		 QWidget* parent=0, const char* name=0, Qt::WFlags f=0);
	Table(ScriptingEnv *env, int r,int c, const QString &label, QWidget* parent=0, const char* name=0, Qt::WFlags f=0);

	//! Sets the number of significant digits
	void setNumericPrecision(int prec);

	//! Return the window name
	virtual QString name() { return d_future_table->name();} 
	//! Set the window name
	virtual void setName(const QString& s) { d_future_table->setName(s); setObjectName(s); updateCaption(); }
	//! Return the window label
	virtual QString windowLabel() { return d_future_table->comment(); }
	//! Set the window label
	virtual void setWindowLabel(const QString& s) { d_future_table->setComment(s); updateCaption(); }
public slots:
	void copy(Table *m);
	int numRows();
	int numCols();
	int rowCount();
	int columnCount();
	void setNumRows(int rows);
	void setNumCols(int cols);
	void handleChange();
	void handleRowChange();
	void handleColumnChange(int,int);
	void handleColumnChange(int,int,int,int);
	void handleColumnsRemoved(int,int);

	//! Return column number 'index'
	Column* column(int index) const { return d_future_table->column(index); }
	//! Return the column determined by the given name
	/**
	 * This method should not be used unless absolutely necessary. 
	 * Columns should be addressed by their index. 
	 * This method is mainly meant to be used in scripts.
	 */
	Column* column(const QString & name) const { return d_future_table->column(name); }

	//! Return the value of the cell as a double
	double cell(int row, int col);
	void setCell(int row, int col, double val);

	QString text(int row, int col);
	QStringList columnsList();
	QStringList colNames();
	QString colName(int col);
	QString colLabel(int col);
	int colIndex(const QString& name);

	SciDAVis::PlotDesignation colPlotDesignation(int col);
	void setColPlotDesignation(int col, SciDAVis::PlotDesignation d);
	void setPlotDesignation(SciDAVis::PlotDesignation pd);
	QList<int> plotDesignations();

	void setColName(int col,const QString& text);
	void setHeader(QStringList header);
	void importV0x0001XXHeader(QStringList header);
	void setText(int row,int col,const QString & text);

	void clearCell(int row, int col);

	void print();
	void print(const QString& fileName);
	void exportPDF(const QString& fileName);
    void customEvent(QEvent* e);

	//! \name Column Operations
	//@{
	void removeCol();
	void removeCol(const QStringList& list);
	void clearCol();
	void insertCol();
	void insertCols(int start, int count);
	void addCol(SciDAVis::PlotDesignation pd = SciDAVis::Y);
	void addColumns(int c);
	//@}

	int firstXCol();
	bool noXColumn();
	bool noYColumn();
	int colX(int col);
	int colY(int col);

	//! Set all column formulae.
	void setCommands(const QStringList& com);
	//! Set all column formulae.
	void setCommands(const QString& com);
	//! Set formula for column col.
	void setCommand(int col, const QString& com);
	//! Compute specified cells from column formula.
	bool calculate(int col, int startRow, int endRow);
	//! Compute selected cells from column formulae; use current cell if there's no selection.
	bool calculate();

	//! \name Row Operations
	//@{
	void deleteSelectedRows();
	void insertRow();
	//@}

	//! Selection Operations
	//@{
	void cutSelection();
	void copySelection();
	void clearSelection();
	void pasteSelection();
	void selectAllTable();
	void deselect();
	void clear();
	//@}

	void init();
	QStringList selectedColumns();
	QStringList selectedYColumns();
	QStringList selectedErrColumns();
	QStringList selectedYLabels();
	QStringList drawableColumnSelection();
	QStringList YColumns();
	int selectedColsNumber();

	int columnWidth(int col);
	void setColWidths(const QStringList& widths);

	int numSelectedRows();

	void columnNumericFormat(int col, char *f, int *precision);
	void columnNumericFormat(int col, int *f, int *precision);
	int columnType(int col);

	QList<int> columnTypes();
	void setColumnTypes(QList<int> ctl);
	void setColumnTypes(const QStringList& ctl);
	void setColumnType(int col, SciDAVis::ColumnMode mode);


	QString columnFormat(int col);

	bool exportASCII(const QString& fname, const QString& separator,
					bool withLabels = false, bool exportSelection = false);

	//! \name Saving and Restoring
	//@{
	virtual QString saveToString(const QString& geometry);
	QString saveHeader();
	QString saveComments();
	QString saveCommands();
	QString saveColumnWidths();
	QString saveColumnTypes();
	//@}

	void setBackgroundColor(const QColor& col);
	void setTextColor(const QColor& col);
	void setHeaderColor(const QColor& col);
	void setTextFont(const QFont& fnt);
	void setHeaderFont(const QFont& fnt);

	int verticalHeaderWidth();

	QString colComment(int col);
	void setColComment(int col, const QString& s);
	QStringList colComments();
	void setColComments(const QStringList& lst);
	bool commentsEnabled();

	QString saveAsTemplate(const QString& geometryInfo);
	void restore(const QStringList& lst);

signals:
	void changedColHeader(const QString&, const QString&);
	void removedCol(const QString&);
	void modifiedData(Table *, const QString&);
	void resizedTable(QWidget*);
	void showContextMenu(bool selection);
	void createTable(const QString&,int,int,const QString&);

};

#endif
