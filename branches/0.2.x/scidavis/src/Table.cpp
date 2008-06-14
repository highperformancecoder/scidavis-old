/***************************************************************************
    File                 : Table.cpp
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
#include "Table.h"
#include "SortDialog.h"
#include "TableDialog.h"
#include "core/column/Column.h"
#include "lib/Interval.h"
#include "table/TableModel.h"

#include <QMessageBox>
#include <QDateTime>
#include <QTextStream>
#include <QClipboard>
#include <QApplication>
#include <QPainter>
#include <QEvent>
#include <QLayout>
#include <QPrintDialog>
#include <QLocale>
#include <QShortcut>
#include <QProgressDialog>
#include <QFile>

#include <Q3TextStream>
#include <q3paintdevicemetrics.h>
#include <q3dragobject.h>
#include <Q3TableSelection>
#include <Q3MemArray>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_sort.h>
#include <gsl/gsl_sort_vector.h>

Table::Table(ScriptingEnv *env, const QString &fname,const QString &sep, int ignoredLines, bool renameCols,
			 bool stripSpaces, bool simplifySpaces, const QString& label,
			 QWidget* parent, const char* name, Qt::WFlags f)
	: TableView(label, parent, name,f), scripted(env)
{
	d_future_table = new future::Table(0, 0, 0, label);
	TableView::setTable(d_future_table);	
	d_future_table->setView(this);	
	importASCII(fname, sep, ignoredLines, renameCols, stripSpaces, simplifySpaces, true);
}

Table::Table(ScriptingEnv *env, int r, int c, const QString& label, QWidget* parent, const char* name, Qt::WFlags f)
	: TableView(label, parent, name,f), scripted(env)
{
	d_future_table = new future::Table(0, r, c, label);
	init(r,c);
}

void Table::init(int rows, int cols)
{
	TableView::setTable(d_future_table);	
	d_future_table->setView(this);	
	d_saved_cells = 0;

	setBirthDate(d_future_table->creationTime().toString(Qt::LocalDate));

// TODO: remove this
#if 0
	d_table = new MyTable(rows, cols, this, "table");
	d_table->setFocusPolicy(Qt::StrongFocus);
	d_table->setFocus();
	d_table->setSelectionMode (Q3Table::Single);
	d_table->setRowMovingEnabled(true);
	// TODO: would be useful, but then we have to interpret
	// signal Q3Header::indexChange ( int section, int fromIndex, int toIndex )
	// and update some variables
	//d_table->setColumnMovingEnabled(true);

	connect(d_table->verticalHeader(), SIGNAL(indexChange ( int, int, int )),
			this, SLOT(notifyChanges()));

	QHBoxLayout* hlayout = new QHBoxLayout(this);
	hlayout->setMargin(0);
	hlayout->addWidget(d_table);

	for (int i=0; i<cols; i++)
	{
		commands << "";
		colTypes << Numeric;
		col_format << "0/16";
		comments << "";
		col_label << QString::number(i+1);
		col_plot_type << Y;
	}

	Q3Header* head=(Q3Header*)d_table->horizontalHeader();
	head->setMouseTracking(true);
	head->setResizeEnabled(true);
	head->installEventFilter(this);
	connect(head, SIGNAL(sizeChange(int, int, int)), this, SLOT(colWidthModified(int, int, int)));

	col_plot_type[0] = X;
	setHeaderColType();

	int w=4*(d_table->horizontalHeader())->sectionSize(0);
	int h;
	if (rows>11)
		h=11*(d_table->verticalHeader())->sectionSize(0);
	else
		h=(rows+1)*(d_table->verticalHeader())->sectionSize(0);
	setGeometry(50, 50, w + 45, h);

	d_table->verticalHeader()->setResizeEnabled(false);
	d_table->verticalHeader()->installEventFilter(this);

	QShortcut *accelTab = new QShortcut(QKeySequence(Qt::Key_Tab), this);
	connect(accelTab, SIGNAL(activated()), this, SLOT(moveCurrentCell()));

	QShortcut *accelAll = new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_A), this);
	connect(accelAll, SIGNAL(activated()), this, SLOT(selectAllTable()));

	connect(d_table, SIGNAL(valueChanged(int,int)),this, SLOT(cellEdited(int,int)));
#endif

	connect(d_future_table, SIGNAL(columnsInserted(int, int)), this, SLOT(handleChange()));
	connect(d_future_table, SIGNAL(columnsReplaced(int, int)), this, SLOT(handleChange()));
	connect(d_future_table, SIGNAL(columnsRemoved(int, int)), this, SLOT(handleChange()));
	connect(d_future_table, SIGNAL(rowsInserted(int, int)), this, SLOT(handleChange()));
	connect(d_future_table, SIGNAL(rowsRemoved(int, int)), this, SLOT(handleChange()));
	connect(d_future_table, SIGNAL(dataChanged(int, int, int, int)), this, SLOT(handleChange()));
	connect(d_future_table, SIGNAL(headerDataChanged(Qt::Orientation, int, int)), this, SLOT(handleChange()));
// TODO	connect(d_future_table, SIGNAL(recalculate()), this, SLOT(recalculate()));
	// TODO: Check whether the columns have to be connected too
}

void Table::handleChange()
{
    emit modifiedWindow(this);
}

#if 0
void Table::colWidthModified(int, int, int)
{
	emit modifiedWindow(this);
	setHeaderColType();
}
#endif

void Table::setBackgroundColor(const QColor& col)
{
	d_view_widget->setPaletteBackgroundColor( col );
}

void Table::setTextColor(const QColor& col)
{
	d_view_widget->setPaletteForegroundColor(col);
}

void Table::setTextFont(const QFont& fnt)
{
	d_view_widget->setFont(fnt);
}

void Table::setHeaderColor(const QColor& col)
{
	d_view_widget->horizontalHeader()->setPaletteForegroundColor (col);
}

void Table::setHeaderFont(const QFont& fnt)
{
	d_view_widget->horizontalHeader()->setFont(fnt);
}

void Table::exportPDF(const QString& fileName)
{
	print(fileName);
}

void Table::print()
{
	print(QString());
}

void Table::print(const QString& fileName)
{
	QPrinter printer;
	printer.setColorMode (QPrinter::GrayScale);

	if (!fileName.isEmpty())
	{
		printer.setCreator("SciDAVis");
		printer.setOutputFormat(QPrinter::PdfFormat);
		printer.setOutputFileName(fileName);
	}
	else
	{
		QPrintDialog printDialog(&printer);
		if (printDialog.exec() != QDialog::Accepted)
			return;
	}

	printer.setFullPage( true );
	QPainter p;
	if ( !p.begin(&printer ) )
		return; // paint on printer
	int dpiy = printer.logicalDpiY();
	const int margin = (int) ( (1/2.54)*dpiy ); // 1 cm margins

	QHeaderView *hHeader = d_view_widget->horizontalHeader();
	QHeaderView *vHeader = d_view_widget->verticalHeader();

	int rows = numRows();
	int cols = numCols();
	int height = margin;
	int i, vertHeaderWidth = vHeader->width();
	int right = margin + vertHeaderWidth;

	// print header
	p.setFont(hHeader->font());
	QString header_label = d_view_widget->model()->headerData(0, Qt::Horizontal).toString();
	QRect br = p.boundingRect(br, Qt::AlignCenter, header_label);
	p.drawLine(right, height, right, height+br.height());
	QRect tr(br);

	for (i=0;i<cols;i++)
	{
		int w = columnWidth(i);
		tr.setTopLeft(QPoint(right,height));
		tr.setWidth(w);
		tr.setHeight(br.height());
		header_label = d_view_widget->model()->headerData(i, Qt::Horizontal).toString();
		p.drawText(tr, Qt::AlignCenter, header_label,-1);
		right += w;
		p.drawLine(right, height, right, height+tr.height());

		if (right >= printer.width()-2*margin )
			break;
	}

	p.drawLine(margin + vertHeaderWidth, height, right-1, height);//first horizontal line
	height += tr.height();
	p.drawLine(margin, height, right-1, height);

	// print table values
	for (i=0;i<rows;i++)
	{
		right = margin;
		QString cell_text = d_view_widget->model()->headerData(i, Qt::Horizontal).toString()+"\t";
		tr = p.boundingRect(tr, Qt::AlignCenter, cell_text);
		p.drawLine(right, height, right, height+tr.height());

		br.setTopLeft(QPoint(right,height));
		br.setWidth(vertHeaderWidth);
		br.setHeight(tr.height());
		p.drawText(br, Qt::AlignCenter, cell_text, -1);
		right += vertHeaderWidth;
		p.drawLine(right, height, right, height+tr.height());

		for(int j=0;j<cols;j++)
		{
			int w = columnWidth (j);
			cell_text = text(i,j)+"\t";
			tr = p.boundingRect(tr,Qt::AlignCenter,cell_text);
			br.setTopLeft(QPoint(right,height));
			br.setWidth(w);
			br.setHeight(tr.height());
			p.drawText(br, Qt::AlignCenter, cell_text, -1);
			right += w;
			p.drawLine(right, height, right, height+tr.height());

			if (right >= printer.width()-2*margin )
				break;
		}
		height += br.height();
		p.drawLine(margin, height, right-1, height);

		if (height >= printer.height()-margin )
		{
			printer.newPage();
			height = margin;
			p.drawLine(margin, height, right, height);
		}
	}
}

// TODO: remove
#if 0
void Table::cellEdited(int row, int col)
{
	QString text = text(row,col).remove(QRegExp("\\s"));
	if (columnType(col) != Numeric || text.isEmpty())
	{
		emit modifiedData(this, colName(col));
		emit modifiedWindow(this);
		return;
	}

	char f;
	int precision;
	columnNumericFormat(col, &f, &precision);
	bool ok = true;
	QLocale locale;
	double res = locale.toDouble(text, &ok);
	if (ok)
		setText(row, col, locale.toString(res, f, precision));
	else
	{
		Script *script = scriptEnv->newScript(text(row,col),this,QString("<%1_%2_%3>").arg(name()).arg(row+1).arg(col+1));
		connect(script, SIGNAL(error(const QString&,const QString&,int)), scriptEnv, SIGNAL(error(const QString&,const QString&,int)));

		script->setInt(row+1, "i");
		script->setInt(col+1, "j");
		QVariant ret = script->eval();
		if(ret.type()==QVariant::Int || ret.type()==QVariant::UInt || ret.type()==QVariant::LongLong || ret.type()==QVariant::ULongLong)
			setText(row, col, ret.toString());
		else if(ret.canCast(QVariant::Double))
			setText(row, col, locale.toString(ret.toDouble(), f, precision));
		else
			setText(row, col, "");
	}

	emit modifiedData(this, colName(col));
	emit modifiedWindow(this);
}
#endif
// TODO: modifiedData signal

int Table::colX(int col)
{
	return d_future_table->colX(col);
}

int Table::colY(int col)
{
	return d_future_table->colY(col);
}

void Table::setPlotDesignation(SciDAVis::PlotDesignation pd)
{
	d_future_table->setSelectionAs(pd);
}

void Table::columnNumericFormat(int col, int *f, int *precision)
{
// TODO
#if 0
	QStringList format = col_format[col].split("/", QString::KeepEmptyParts);
	if (format.count() == 2)
	{
		*f = format[0].toInt();
		*precision = format[1].toInt();
	}
	else
	{
		*f = 0;
		*precision = 14;
	}
#endif
}

void Table::columnNumericFormat(int col, char *f, int *precision)
{
// TODO
#if 0
	QStringList format = col_format[col].split("/", QString::KeepEmptyParts);
	if (format.count() == 2)
	{
		switch(format[0].toInt())
		{
			case 0:
				*f = 'g';
				break;

			case 1:
				*f = 'f';
				break;

			case 2:
				*f = 'e';
				break;
		}
		*precision = format[1].toInt();
	}
	else
	{
		*f = 'g';
		*precision = 14;
	}
#endif
}

int Table::columnWidth(int col)
{
	return d_view_widget->columnWidth(col);
}

QStringList Table::columnWidths()
{
// TODO: replace
	QStringList widths;
	for (int i=0;i<numCols();i++)
		widths << QString::number(columnWidth(i));

	return widths;
}

void Table::setColWidths(const QStringList& widths)
{
	for (int i=0;i<widths.count();i++)
		d_view_widget->setColumnWidth(i, widths[i].toInt() );
}

void Table::setColumnTypes(const QStringList& ctl)
{
// TODO: replace
#if 0
	int n = qMin((int)ctl.count(), numCols());
	for (int i=0; i<n; i++)
	{
		QStringList l = ctl[i].split(";");
		colTypes[i] = l[0].toInt();

		if ((int)l.count() == 2 && !l[1].isEmpty())
			col_format[i] = l[1];
		else
			col_format[i] = "0/6";
	}
#endif
}

QString Table::saveColumnWidths()
{
// TODO: move to "import old format" code
	QString s="ColWidth\t";
	for (int i=0;i<numCols();i++)
		s+=QString::number(columnWidth(i))+"\t";

	return s+"\n";
}

QString Table::saveColumnTypes()
{
// TODO: move to "import old format" code
	QString s="ColType";
#if 0
	for (int i=0; i<numCols(); i++)
		s += "\t"+QString::number(colTypes[i])+";"+col_format[i];
#endif
	return s+"\n";
}

void Table::setCommands(const QStringList& com)
{
	for(int i=0; i<(int)com.size() && i<numCols(); i++)
		d_future_table->column(i)->setFormula(Interval<int>(0, numRows()-1), com.at(i).trimmed());
}

void Table::setCommand(int col, const QString& com)
{
	d_future_table->column(col)->setFormula(Interval<int>(0, numRows()-1), com.trimmed());
}

void Table::setCommands(const QString& com)
{
	QStringList lst = com.split("\t");
	lst.pop_front();
	setCommands(lst);
}

bool Table::calculate(int col, int startRow, int endRow)
{
// TODO
	if (col < 0 || col >= numCols())
		return false;

	QApplication::setOverrideCursor(Qt::WaitCursor);

	if (d_future_table->column(col)->formula(0).isEmpty())
	{
		for (int i=startRow; i<=endRow; i++)
			setText(i, col, "");
		QApplication::restoreOverrideCursor();
		return true;
	}

	Script *colscript = scriptEnv->newScript(d_future_table->column(col)->formula(0), this,  QString("<%1>").arg(colName(col)));
	connect(colscript, SIGNAL(error(const QString&,const QString&,int)), scriptEnv, SIGNAL(error(const QString&,const QString&,int)));
	connect(colscript, SIGNAL(print(const QString&)), scriptEnv, SIGNAL(print(const QString&)));

	if (!colscript->compile())
	{
		QApplication::restoreOverrideCursor();
		return false;
	}
	if (endRow >= numRows())
		resizeRows(endRow + 1);

	colscript->setInt(col+1, "j");
	colscript->setInt(startRow+1, "sr");
	colscript->setInt(endRow+1, "er");
	QVariant ret;
	for (int i=startRow; i<=endRow; i++)
	{
		colscript->setInt(i+1,"i");
		ret = colscript->eval();
		if(ret.type()==QVariant::Double) {
			int prec;
			char f;
			columnNumericFormat(col, &f, &prec);
			setText(i, col, QLocale().toString(ret.toDouble(), f, prec));
		} else if(ret.canConvert(QVariant::String))
			setText(i, col, ret.toString());
		else {
			QApplication::restoreOverrideCursor();
			return false;
		}
	}

	emit modifiedData(this, colName(col));
	emit modifiedWindow(this);
	QApplication::restoreOverrideCursor();
	return true;
}

bool Table::calculate()
{
// TODO
	bool success = true;
	for (int col=firstSelectedColumn(); col<=lastSelectedColumn(); col++)
		if (!calculate(col, firstSelectedRow(), lastSelectedRow()))
			success = false;
	return success;
}

QString Table::saveCommands()
{
// TODO: obsolete
	QString s="<com>\n";
#if 0
	for (int col=0; col<numCols(); col++)
		if (!commands[col].isEmpty())
		{
			s += "<col nr=\""+QString::number(col)+"\">\n";
			s += commands[col];
			s += "\n</col>\n";
		}
#endif
	s += "</com>\n";
	return s;
}

QString Table::saveComments()
{
// TODO: obsolete
	QString s = "Comments\t";
#if 0
	for (int i=0; i<numCols(); i++)
	{
		if (comments.count() > i)
			s += comments[i] + "\t";
		else
			s += "\t";
	}
#endif
	return s + "\n";
}

QString Table::saveToString(const QString& geometry)
{
// TODO: obsolete
	QString s="<table>\n";
	s+=QString(name())+"\t";
	s+=QString::number(numRows())+"\t";
	s+=QString::number(numCols())+"\t";
	s+=birthDate()+"\n";
	s+=geometry;
	s+=saveHeader();
	s+=saveColumnWidths();
	s+=saveCommands();
	s+=saveColumnTypes();
	s+=saveComments();
	s+="WindowLabel\t" + windowLabel() + "\t" + QString::number(captionPolicy()) + "\n";
	s+=saveText();
	return s+="</table>\n";
}

QString Table::saveHeader()
{
// TODO: obsolete
	QString s = "header";
#if 0
	for (int j=0; j<numCols(); j++){
		if (col_plot_type[j] == X)
			s += "\t" + colLabel(j) + "[X]";
		else if (col_plot_type[j] == Y)
			s += "\t" + colLabel(j) + "[Y]";
		else if (col_plot_type[j] == Z)
			s += "\t" + colLabel(j) + "[Z]";
		else if (col_plot_type[j] == xErr)
			s += "\t" + colLabel(j) + "[xEr]";
		else if (col_plot_type[j] == yErr)
			s += "\t" + colLabel(j) + "[yEr]";
		else
			s += "\t" + colLabel(j);
	}
#endif
	return s += "\n";
}

int Table::firstXCol()
{
	for (int j=0; j<numCols(); j++)
	{
		if (d_future_table->column(j)->plotDesignation() == SciDAVis::X)
			return j;
	}
	return -1;
}

void Table::enumerateRightCols(bool checked)
{
// TODO: decide what to do with this
#if 0
	if (!checked)
		return;

	QString oldLabel = colLabel(selectedCol);
	QStringList oldLabels = colNames();
	QString caption = QString(this->name())+"_";
	int n=1;
	for (int i=selectedCol; i<numCols(); i++)
	{
		QString newLabel = oldLabel + QString::number(n);
		commands.replaceInStrings("col(\""+colLabel(i)+"\")", "col(\""+newLabel+"\")");
		setColName(i, newLabel);
		emit changedColHeader(caption+oldLabels[i],colName(i));
		n++;
	}
	emit modifiedWindow(this);
#endif
}

void Table::setColComment(int col, const QString& s)
{
	d_future_table->column(col)->setComment(s);
}

void Table::changeColWidth(int width, bool allCols)
{
// TODO: obsolete
	int cols=numCols();
	if (allCols)
	{
		for (int i=0;i<cols;i++)
			d_view_widget->setColumnWidth (i, width);

		emit modifiedWindow(this);
	}
	else
	{
		d_view_widget->setColumnWidth (firstSelectedColumn(), width);
		emit modifiedWindow(this);
	}
}

void Table::changeColWidth(int width, int col)
{
// TODO
	if (columnWidth(col) == width)
		return;

	d_view_widget->setColumnWidth (col, width);
	emit modifiedWindow(this);
}

void Table::changeColName(const QString& text)
{
// TODO
	QString caption = this->name();
	QString oldName = colName(firstSelectedColumn());
	QString newName = caption+"_"+text;

	if (oldName == newName)
		return;

	if (caption == text)
	{
		QMessageBox::critical(0,tr("Error"),
				tr("The column name must be different from the table name : <b>"+caption+"</b></b>!<p>Please choose another name!"));
		return;
	}

	QStringList labels=colNames();
	if (labels.contains(text)>0)
	{
		QMessageBox::critical(0,tr("Error"),
				tr("There is already a column called : <b>"+text+"</b> in table <b>"+caption+"</b>!<p>Please choose another name!"));
		return;
	}

// TODO: port this functionality
///	commands.replaceInStrings("col(\""+colLabel(selectedCol)+"\")", "col(\""+text+"\")");

////	setColName(selectedCol, text);
	emit changedColHeader(oldName, newName);
	emit modifiedWindow(this);
}

void Table::setColName(int col, const QString& text)
{
	d_future_table->column(col)->setName(text);
}

QStringList Table::selectedColumns()
{
// TODO: extended selection support, Column * lists
	QStringList names;
	for (int i=0; i<numCols(); i++)
	{
		if(isColumnSelected (i))
			names << name() + "_" + column(i)->name();
	}
	return names;
}

QStringList Table::YColumns()
{
// TODO: Column * list
	QStringList names;
	for (int i=0;i<numCols();i++)
	{
		if(column(i)->plotDesignation() == SciDAVis::Y)
			names << name() + "_" + column(i)->name();
	}
	return names;
}

QStringList Table::selectedYColumns()
{
// TODO: Column * list
	QStringList names;
	for (int i=0;i<numCols();i++)
	{
		if(isColumnSelected (i) && column(i)->plotDesignation() == SciDAVis::Y)
			names << name() + "_" + column(i)->name();
	}
	return names;
}

QStringList Table::selectedErrColumns()
{
// TODO: Column * list
	QStringList names;
	for (int i=0;i<numCols();i++)
	{
		if (isColumnSelected (i) && 
				(column(i)->plotDesignation() == SciDAVis::xErr || 
				 column(i)->plotDesignation() == SciDAVis::yErr) )
			names << name() + "_" + column(i)->name();
	}
	return names;
}

QStringList Table::drawableColumnSelection()
{
// TODO: Column * list
	QStringList names;
	for (int i=0; i<numCols(); i++)
	{
		if(isColumnSelected (i) && column(i)->plotDesignation() == SciDAVis::Y)
			names << name() + "_" + column(i)->name();
	}

	for (int i=0; i<numCols(); i++)
	{
		if (isColumnSelected (i) && 
				(column(i)->plotDesignation() == SciDAVis::xErr || 
				 column(i)->plotDesignation() == SciDAVis::yErr) )
			names << name() + "_" + column(i)->name();
	}
	return names;
}

QStringList Table::selectedYLabels()
{
	QStringList names;
	for (int i=0;i <numCols(); i++)
	{
		if(isColumnSelected (i) && column(i)->plotDesignation() == SciDAVis::Y)
			names << column(i)->name();
	}
	return names;
}

QStringList Table::columnsList()
{
	QStringList names;
	for (int i=0; i<numCols(); i++)
		names << name() +"_" + column(i)->name();

	return names;
}

int Table::numSelectedRows()
{
	return selectedRowCount();
}

int Table::selectedColsNumber()
{
	return selectedColumnCount();
}

QString Table::colName(int col)
{//returns the table name + horizontal header text
	if (col<0 || col >= numCols())
		return QString();

	return QString(name() + "_" + d_future_table->column(col)->name());
}

QVarLengthArray<double> Table::col(int ycol)
{
// TODO: replace with Column
	int i;
	int rows=numRows();
	int cols=numCols();
	QVarLengthArray<double> Y(rows);
	if (ycol<=cols)
	{
		for (i=0;i<rows;i++)
			Y[i]=text(i,ycol).toDouble();
	}
	return Y;
}

void Table::insertCols(int start, int count)
{
	if (start < 0)
		start = 0;

	QList<Column*> cols;
	for(int i=0; i<count; i++)
		cols << new Column(QString::number(i+1), SciDAVis::Numeric);
	d_future_table->insertColumns(start, cols);
}

void Table::insertCol()
{
	d_future_table->insertEmptyColumns();
}

void Table::insertRow()
{
	d_future_table->insertEmptyRows();
}

void Table::addCol(SciDAVis::PlotDesignation pd)
{
	d_future_table->addColumn();
	d_future_table->column(d_future_table->columnCount()-1)->setPlotDesignation(pd);
}

void Table::addColumns(int c)
{
	QList<Column*> cols;
	for(int i=0; i<c; i++)
		cols << new Column(QString::number(i+1), SciDAVis::Numeric);
	d_future_table->appendColumns(cols);
}

void Table::clearCol()
{
	d_future_table->clearSelectedColumns();
}

void Table::clearCell(int row, int col)
{
	setText(row, col, "");
}

void Table::deleteSelectedRows()
{
	d_future_table->removeSelectedRows();
}

void Table::cutSelection()
{
	d_future_table->cutSelection();
}

void Table::selectAllTable()
{
	selectAll();
}

void Table::deselect()
{
	d_view_widget->clearSelection();
}

void Table::clearSelection()
{
	d_future_table->clearSelectedCells();
}

void Table::copySelection()
{
	d_future_table->copySelection();
}

void Table::pasteSelection()
{
	d_future_table->pasteIntoSelection();
}

void Table::removeCol()
{
	d_future_table->removeSelectedColumns();
}

void Table::removeCol(const QStringList& list)
{
	foreach(QString name, list)
		d_future_table->removeColumn(d_future_table->column(name));
}

void Table::normalizeSelection()
{
	for (int i=0; i<numCols(); i++)
	{
		if (isColumnSelected(i))
			normalizeCol(i);
	}
}

void Table::normalize()
{
	for (int i=0; i<numCols(); i++)
		normalizeCol(i);
}

void Table::normalizeCol(int col)
{
	// TODO: implement normalize in future::Table
	if (col<0) col = firstSelectedColumn();
	double max=text(0,col).toDouble();
	double aux=0.0;
	int rows=numRows();
	for (int i=0; i<rows; i++)
	{
		QString text=this->text(i,col);
		aux=text.toDouble();
		if (!text.isEmpty() && fabs(aux)>fabs(max))
			max=aux;
	}

	if (max == 1.0)
		return;

	int prec;
	char f;
	columnNumericFormat(col, &f, &prec);

	for (int i=0; i<rows; i++)
	{
		QString text = this->text(i, col);
		aux=text.toDouble();
		if ( !text.isEmpty() )
			setText(i, col, QLocale().toString(aux/max, f, prec));
	}

	QString name=colName(col);
	emit modifiedData(this, name);
}

void Table::sortColumnsDialog()
{
	d_future_table->sortSelectedColumns();
}

void Table::sortTableDialog()
{
	d_future_table->sortTable();
}

void Table::sort(int type, int order, const QString& leadCol)
{
	// TODO
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
////	sortColumns(col_label, type, order, leadCol);
	QApplication::restoreOverrideCursor();
}

void Table::sortColumns(int type, int order, const QString& leadCol)
{
	// TODO
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
////	sortColumns(selectedColumns(), type, order, leadCol);
	QApplication::restoreOverrideCursor();
}


	// TODO: replace
#if 0
void Table::sortColumns(const QStringList&s, int type, int order, const QString& leadCol)
{
	int cols=s.count();
	if(!type){//Sort columns separately
		for(int i=0;i<cols;i++)
			sortColumn(colIndex(s[i]), order);
	}else{
		int leadcol = colIndex(leadCol);
		if (leadcol < 0){
			QMessageBox::critical(this, tr("Error"),
					tr("Please indicate the name of the leading column!"));
			return;
		}
		if (columnType(leadcol) == Table::Text){
			QMessageBox::critical(this, tr("Error"),
					tr("The leading column has the type set to 'Text'! Operation aborted!"));
			return;
		}

		int rows=numRows();
		int non_empty_cells = 0;
		QVarLengthArray<int> valid_cell(rows);
		QVarLengthArray<double> data_double(rows);
		for (int j = 0; j <rows; j++){
			if (!text(j, leadcol).isEmpty()){
				data_double[non_empty_cells] = cell(j,leadcol);
				valid_cell[non_empty_cells] = j;
				non_empty_cells++;
			}
		}

		if (!non_empty_cells){
			QMessageBox::critical(this, tr("Error"),
					tr("The leading column is empty! Operation aborted!"));
			return;
		}

		data_double.resize(non_empty_cells);
		valid_cell.resize(non_empty_cells);
		QVarLengthArray<QString> data_string(non_empty_cells);
		size_t *p= new size_t[non_empty_cells];

		// Find the permutation index for the lead col
		gsl_sort_index(p, data_double.data(), 1, non_empty_cells);

		for(int i=0;i<cols;i++){// Since we have the permutation index, sort all the columns
			int col=colIndex(s[i]);
			if (columnType(col) == Text){
				for (int j=0; j<non_empty_cells; j++)
					data_string[j] = text(valid_cell[j], col);
				if(!order)
					for (int j=0; j<non_empty_cells; j++)
						setText(valid_cell[j], col, data_string[p[j]]);
				else
					for (int j=0; j<non_empty_cells; j++)
						setText(valid_cell[j], col, data_string[p[non_empty_cells-j-1]]);
			}else{
				for (int j = 0; j<non_empty_cells; j++)
					data_double[j] = cell(valid_cell[j], col);
				int prec;
				char f;
				columnNumericFormat(col, &f, &prec);
				if(!order)
					for (int j=0; j<non_empty_cells; j++)
						setText(valid_cell[j], col, QLocale().toString(data_double[p[j]], f, prec));
				else
					for (int j=0; j<non_empty_cells; j++)
						setText(valid_cell[j], col, QLocale().toString(data_double[p[non_empty_cells-j-1]], f, prec));
			}
			emit modifiedData(this, colName(col));
		}
		delete[] p;
	}
	emit modifiedWindow(this);
}

void Table::sortColumn(int col, int order)
{
	if (col < 0)
		col = d_table->currentColumn();

	int rows=numRows();
	int non_empty_cells = 0;
	QVarLengthArray<int> valid_cell(rows);
	QVarLengthArray<double> r(rows);
	QStringList text_cells;
	for (int i = 0; i <rows; i++){
		if (!text(i, col).isEmpty()){
			if (columnType(col) == Table::Text)
				text_cells << text(i, col);
			else
				r[non_empty_cells] = this->text(i,col).toDouble();
			valid_cell[non_empty_cells] = i;
			non_empty_cells++;
		}
	}

	if (!non_empty_cells)
		return;

	valid_cell.resize(non_empty_cells);
	if (columnType(col) == Table::Text){
		r.clear();
		text_cells.sort();
	} else {
		r.resize(non_empty_cells);
		gsl_sort(r.data(), 1, non_empty_cells);
	}

	if (columnType(col) == Table::Text){
		if (!order){
			for (int i=0; i<non_empty_cells; i++)
				setText(valid_cell[i], col, text_cells[i]);
		} else {
			for (int i=0; i<non_empty_cells; i++)
				setText(valid_cell[i], col, text_cells[non_empty_cells-i-1]);
		}
	} else {
		int prec;
		char f;
		columnNumericFormat(col, &f, &prec);
		if (!order) {
			for (int i=0; i<non_empty_cells; i++)
				setText(valid_cell[i], col, QLocale().toString(r[i], f, prec));
		} else {
			for (int i=0; i<non_empty_cells; i++)
				setText(valid_cell[i], col, QLocale().toString(r[non_empty_cells-i-1], f, prec));
		}
	}
	emit modifiedData(this, colName(col));
	emit modifiedWindow(this);
}
#endif

// TODO: remove?
#if 0
void Table::sortColAsc()
{
	sortColumn(d_table->currentColumn ());
}
#endif

// TODO: remove?
#if 0
void Table::sortColDesc()
{
	sortColumn(d_table->currentColumn(), 1);
}
#endif

int Table::numRows()
{
	return d_future_table->rowCount();
}

int Table::numCols()
{
	return d_future_table->columnCount();
}

QString Table::saveText()
{
	QString out_text = "<data>\n";
	int cols = numCols();
	int rows = numRows();

	for (int i=0; i<rows; i++)
	{
		out_text += QString::number(i)+"\t";
		for (int j=0; j<cols-1; j++)
			out_text += text(i,j)+"\t";

		out_text += text(i,cols-1)+"\n";
	}
	return out_text + "</data>\n";
}

double Table::cell(int row, int col)
{
	// TODO: check whether this is used for non numeric columns anywhere
	return d_future_table->column(col)->valueAt(row);
}

void Table::setCell(int row, int col, double val)
{
	d_future_table->column(col)->setValueAt(row, val);
}

QString Table::text(int row, int col)
{
////	return d_future_table->column(col)->textAt(row);
	return d_model->data(d_model->index(row, col), Qt::DisplayRole).toString();
}

void Table::setText(int row, int col, const QString & text)
{
	d_future_table->column(col)->setTextAt(row, text);
}

void Table::saveToMemory()
{
// TODO: replace/port
	d_saved_cells = new double* [numCols()];
	for ( int i = 0; i < numCols(); ++i)
		d_saved_cells[i] = new double [numRows()];

	for (int col = 0; col<numCols(); col++){// initialize the matrix to zero
		for (int row=0; row<numRows(); row++)
			d_saved_cells[col][row] = 0.0;}

	for (int col = 0; col<numCols(); col++){
		if (d_future_table->column(col)->columnMode() == Time){
			QTime ref = QTime(0, 0);
			for (int row=0; row<numRows(); row++){
///				QTime t = QTime::fromString(text(row, col), col_format[col]);
///				d_saved_cells[col][row] = ref.msecsTo(t);
			}
		}
		else if (d_future_table->column(col)->columnMode() == Date){
			QTime ref = QTime(0, 0);
			for (int row=0; row<numRows(); row++){
///				QDateTime dt = QDateTime::fromString(text(row, col), col_format[col]);
///				d_saved_cells[col][row] = dt.date().toJulianDay() - 1 + (double)ref.msecsTo(dt.time())/864.0e5;
			}
		}
	}

	bool wrongLocale = false;
	for (int col = 0; col<numCols(); col++){
		if (d_future_table->column(col)->columnMode() == Numeric){
			bool ok = false;
			for (int row=0; row<numRows(); row++){
				if (!text(row, col).isEmpty()){
					d_saved_cells[col][row] = QLocale().toDouble(text(row, col), &ok);
					if (!ok){
						wrongLocale = true;
						break;
					}
				}
			}
			if (wrongLocale)
				break;
		}
	}

	if (wrongLocale){// fall back to C locale
		wrongLocale = false;
		for (int col = 0; col<numCols(); col++){
			if (d_future_table->column(col)->columnMode() == Numeric){
				bool ok = false;
				for (int row=0; row<numRows(); row++){
					if (!text(row, col).isEmpty()){
						d_saved_cells[col][row] = QLocale::c().toDouble(text(row, col), &ok);
						if (!ok){
							wrongLocale = true;
							break;
						}
					}
				}
				if (wrongLocale)
					break;
			}
		}
	}
	if (wrongLocale){// fall back to German locale
		wrongLocale = false;
		for (int col = 0; col<numCols(); col++){
			if (d_future_table->column(col)->columnMode() == Numeric){
				bool ok = false;
				for (int row=0; row<numRows(); row++){
					if (!text(row, col).isEmpty()){
						d_saved_cells[col][row] = QLocale(QLocale::German).toDouble(text(row, col), &ok);
						if (!ok){
							wrongLocale = true;
							break;
						}
					}
				}
				if (wrongLocale)
					break;
			}
		}
	}
	if (wrongLocale){// fall back to French locale
		wrongLocale = false;
		for (int col = 0; col<numCols(); col++){
			if (d_future_table->column(col)->columnMode() == Numeric){
				bool ok = false;
				for (int row=0; row<numRows(); row++){
					if (!text(row, col).isEmpty()){
						d_saved_cells[col][row] = QLocale(QLocale::French).toDouble(text(row, col), &ok);
						if (!ok){
							wrongLocale = true;
							break;
						}
					}
				}
				if (wrongLocale)
					break;
			}
		}
	}
}

void Table::freeMemory()
{
// TODO: replace
	for ( int i = 0; i < numCols(); i++)
		delete[] d_saved_cells[i];

	delete[] d_saved_cells;
	d_saved_cells = 0;
}

void Table::setTextFormat(int col)
{
	// TODO: obsolete
	if (col >= 0 && col < numCols())
		d_future_table->column(col)->setColumnMode(SciDAVis::Text);
}

void Table::setColNumericFormat(int f, int prec, int col, bool updateCells)
{
	// TODO: obsolete
	if (d_future_table->column(col)->columnMode() == SciDAVis::Numeric)
	{
		int old_f, old_prec;
		columnNumericFormat(col, &old_f, &old_prec);
		if (old_f == f && old_prec == prec)
			return;
	}

	d_future_table->column(col)->setColumnMode(SciDAVis::Numeric);
////	col_format[col] = QString::number(f)+"/"+QString::number(prec);

	if (!updateCells)
		return;

	char format = 'g';
	for (int i=0; i<numRows(); i++) {
		QString t = text(i, col);
		if (!t.isEmpty()) {
			if (!f)
				prec = 6;
			else if (f == 1)
				format = 'f';
			else if (f == 2)
				format = 'e';

			if (d_saved_cells)
				setText(i, col, QLocale().toString(d_saved_cells[col][i], format, prec));
			else
				setText(i, col, QLocale().toString(QLocale().toDouble(t), format, prec));
		}
	}
}

void Table::setColumnsFormat(const QStringList& lst)
{
	// TODO: obsolete
#if 0
	if (col_format == lst)
		return;

	col_format = lst;
#endif
}

bool Table::setDateFormat(const QString& format, int col, bool updateCells)
{
	// TODO: obsolete
#if 0
	if (d_future_table->column(col)->columnMode() == Date && col_format[col] == format)
		return true;

	bool first_time = false;
	if (updateCells){
		for (int i=0; i<numRows(); i++){
			QString s = text(i,col);
			if (!s.isEmpty()){
				QDateTime d = QDateTime::fromString (s, format);
				if (d_future_table->column(col)->columnMode() != Date && d.isValid()){
					//This might be the first time the user assigns a date format.
					//If Qt understands the format we break the loop, assign it to the column and return true!
					first_time = true;
					break;
				}

				if (d_saved_cells){
					d = QDateTime(QDate::fromJulianDay(int(d_saved_cells[col][i]+1)));
					double secs = (d_saved_cells[col][i] - int(d_saved_cells[col][i]))*86400;
					d.setTime(d.time().addSecs(int(secs)+1));

					if (d.isValid())
						setText(i, col, d.toString(format));
				}
			}
		}
	}
	d_future_table->column(col)->setColumnMode( Date);
	col_format[col] = format;
	QTime ref = QTime(0, 0);
	if (first_time){//update d_saved_cells in case the user changes the time format before pressing OK in the column dialog
		for (int i=0; i<numRows(); i++){
			QDateTime dt = QDateTime::fromString(text(i, col), format);
			d_saved_cells[col][i] = dt.date().toJulianDay() - 1 + (double)ref.msecsTo(dt.time())/864.0e5;
		}
	}
#endif
	return true;
}

bool Table::setTimeFormat(const QString& format, int col, bool updateCells)
{
	// TODO: obsolete
#if 0
	if (d_future_table->column(col)->columnMode() == Time && col_format[col] == format)
		return true;

	QTime ref = QTime(0, 0);
	bool first_time = false;
	if (updateCells){
		for (int i=0; i<numRows(); i++){
			QString s = text(i,col);
			if (!s.isEmpty()){
				QTime t = QTime::fromString (s, format);
				if (d_future_table->column(col)->columnMode() != Time && t.isValid()){
					//This is the first time the user assigns a time format.
					//If Qt understands the format we break the loop, assign it to the column and return true!
					first_time = true;
					break;
				}

				if (d_saved_cells){
					if (d_saved_cells[col][i] < 1)// import of Origin files
						t = ref.addMSecs(int(d_saved_cells[col][i]*86400000));
					else
						t = ref.addMSecs(int(d_saved_cells[col][i]));

					if (t.isValid())
						setText(i, col, t.toString(format));
				}
			}
		}
	}
	d_future_table->column(col)->setColumnMode( Time);
	col_format[col] = format;
	if (first_time){//update d_saved_cells in case the user changes the time format before pressing OK in the column dialog
		for (int i=0; i<numRows(); i++){
			QTime t = QTime::fromString(text(i, col), format);
			d_saved_cells[col][i] = ref.msecsTo(t);
		}
	}
#endif
	return true;
}

void Table::setMonthFormat(const QString& format, int col, bool updateCells)
{
	// TODO: obsolete
#if 0
	if (d_future_table->column(col)->columnMode() == Month && col_format[col] == format)
		return;

	d_future_table->column(col)->setColumnMode( Month);
	col_format[col] = format;

	if (!updateCells)
		return;

	for (int i=0; i<numRows(); i++){
		QString t = text(i,col);
		if (!t.isEmpty()){
			int day;
			if (d_saved_cells)
				day = int(d_saved_cells[col][i]) % 12;
			else
				day = t.toInt() % 12;
			if (!day)
				day = 12;

			if (format == "M")
				setText(i, col, QDate::shortMonthName(day).left(1));
			else if (format == "MMM")
				setText(i, col, QDate::shortMonthName(day));
			else if (format == "MMMM")
				setText(i, col, QDate::longMonthName(day));
		}
	}
#endif
}

void Table::setDayFormat(const QString& format, int col, bool updateCells)
{
	// TODO: obsolete
#if 0
	if (d_future_table->column(col)->columnMode() == Day && col_format[col] == format)
		return;

	d_future_table->column(col)->setColumnMode( Day);
	col_format[col] = format;

	if (!updateCells)
		return;

	for (int i=0; i<numRows(); i++){
		QString t = text(i,col);
		if (!t.isEmpty()){
			int day;
			if (d_saved_cells)
				day = int(d_saved_cells[col][i]) % 7;
			else
				day = t.toInt() % 7;
			if (!day)
				day = 7;

			if (format == "d")
				setText(i, col, QDate::shortDayName(day).left(1));
			else if (format == "ddd")
				setText(i, col, QDate::shortDayName(day));
			else if (format == "dddd")
				setText(i, col, QDate::longDayName(day));
		}
	}
#endif
}

void Table::setRandomValues()
{
	// TODO: obsolete
#if 0
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	int rows=numRows();
	QStringList list=selectedColumns();

	time_t tmp;
	srand(time(&tmp));

	for (int j=0;j<(int) list.count(); j++)
	{
		QString name=list[j];
		selectedCol=colIndex(name);

		int prec;
		char f;
		columnNumericFormat(selectedCol, &f, &prec);

		for (int i=0; i<rows; i++)
			setText(i, selectedCol, QLocale().toString(double(rand())/double(RAND_MAX), f, prec));

		emit modifiedData(this, name);
	}

	emit modifiedWindow(this);
	QApplication::restoreOverrideCursor();
#endif
}

void Table::loadHeader(QStringList header)
{
	// TODO: -> import
#if 0
	col_label = QStringList();
	col_plot_type = QList <int>();
	for (int i=0; i<header.count();i++)
	{
		if (header[i].isEmpty())
			continue;

		QString s = header[i].replace("_","-");
		if (s.contains("[X]"))
		{
			col_label << s.remove("[X]");
			col_plot_type << X;
		}
		else if (s.contains("[Y]"))
		{
			col_label << s.remove("[Y]");
			col_plot_type << Y;
		}
		else if (s.contains("[Z]"))
		{
			col_label << s.remove("[Z]");
			col_plot_type << Z;
		}
		else if (s.contains("[xEr]"))
		{
			col_label << s.remove("[xEr]");
			col_plot_type << xErr;
		}
		else if (s.contains("[yEr]"))
		{
			col_label << s.remove("[yEr]");
			col_plot_type << yErr;
		}
		else
		{
			col_label << s;
			col_plot_type << None;
		}
	}
	setHeaderColType();
#endif
}

void Table::setHeader(QStringList header)
{
	// TODO: obsolete
///	col_label = header;
	setHeaderColType();
}

int Table::colIndex(const QString& name)
{
	// TODO: no more name concatenation with _
	int pos=name.find("_",false);
	QString label=name.right(name.length()-pos-1);
	return d_future_table->columnIndex(d_future_table->column(label));
}

void Table::setHeaderColType()
{
	// TODO: obsolete
#if 0
	int xcols=0;
	for (int j=0;j<(int)numCols();j++)
	{
		if (col_plot_type[j] == X)
			xcols++;
	}

	if (xcols>1)
	{
		xcols = 0;
		for (int i=0; i<(int)numCols(); i++)
		{
			if (col_plot_type[i] == X)
				setColumnHeader(i, col_label[i]+"[X" + QString::number(++xcols) +"]");
			else if (col_plot_type[i] == Y)
			{
				if(xcols>0)
					setColumnHeader(i, col_label[i]+"[Y"+ QString::number(xcols) +"]");
				else
					setColumnHeader(i, col_label[i]+"[Y]");
			}
			else if (col_plot_type[i] == Z)
			{
				if(xcols>0)
					setColumnHeader(i, col_label[i]+"[Z"+ QString::number(xcols) +"]");
				else
					setColumnHeader(i, col_label[i]+"[Z]");
			}
			else if (col_plot_type[i] == xErr)
				setColumnHeader(i, col_label[i]+"[xEr]");
			else if (col_plot_type[i] == yErr)
				setColumnHeader(i, col_label[i]+"[yEr]");
			else
				setColumnHeader(i, col_label[i]);
		}
	}
	else
	{
		for (int i=0; i<(int)numCols(); i++)
		{
			if (col_plot_type[i] == X)
				setColumnHeader(i, col_label[i]+"[X]");
			else if (col_plot_type[i] == Y)
				setColumnHeader(i, col_label[i]+"[Y]");
			else if (col_plot_type[i] == Z)
				setColumnHeader(i, col_label[i]+"[Z]");
			else if (col_plot_type[i] == xErr)
				setColumnHeader(i, col_label[i]+"[xEr]");
			else if (col_plot_type[i] == yErr)
				setColumnHeader(i, col_label[i]+"[yEr]");
			else
				setColumnHeader(i, col_label[i]);
		}
	}
#endif
}

void Table::setAscValues()
{
	// TODO: obsolete
#if 0
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	int rows=numRows();
	QStringList list=selectedColumns();

	for (int j=0;j<(int) list.count(); j++)
	{
		QString name=list[j];
		selectedCol=colIndex(name);

		if (columnType(selectedCol) != Numeric) {
			d_future_table->column(selectedCol)->setColumnMode( Numeric);
			col_format[selectedCol] = "0/6";
		}

		int prec;
		char f;
		columnNumericFormat(selectedCol, &f, &prec);

		for (int i=0; i<rows; i++)
			setText(i,selectedCol,QString::number(i+1, f, prec));

		emit modifiedData(this, name);
	}

	emit modifiedWindow(this);
	QApplication::restoreOverrideCursor();
#endif
}

bool Table::noXColumn()
{
	return d_future_table->columnCount(SciDAVis::X) == 0;
}

bool Table::noYColumn()
{
	return d_future_table->columnCount(SciDAVis::Y) == 0;
}

void Table::importMultipleASCIIFiles(const QString &fname, const QString &sep, int ignoredLines,
		bool renameCols, bool stripSpaces, bool simplifySpaces,
		int importFileAs)
{
	// TODO: port
#if 0
	QFile f(fname);
	Q3TextStream t( &f );// use a text stream
	if ( f.open(QIODevice::ReadOnly) ){
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

		int i, rows = 1, cols = 0;
		int r = numRows();
		int c = numCols();
		for (i=0; i<ignoredLines; i++)
			t.readLine();

		QString s = t.readLine();//read first line after the ignored ones
		while (!t.atEnd()){
			t.readLine();
			rows++;
			qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
		}

		if (simplifySpaces)
			s = s.simplifyWhiteSpace();
		else if (stripSpaces)
			s = s.trimmed();
		QStringList line = s.split(sep);
		cols = (int)line.count();

		bool allNumbers = true;
		for (i=0; i<cols; i++)
		{//verify if the strings in the line used to rename the columns are not all numbers
			QLocale().toDouble(line[i], &allNumbers);
			if (!allNumbers)
				break;
		}

		if (renameCols && !allNumbers)
			rows--;

		QProgressDialog progress(this);
		int steps = int(rows/1000);
		progress.setRange(0, steps+1);
		progress.setWindowTitle("Reading file...");
		progress.setLabelText(fname);
		progress.setActiveWindow();

		QApplication::restoreOverrideCursor();

		if (!importFileAs)
			init (rows, cols);
		else if (importFileAs == 1){//new cols
			addColumns(cols);
			if (r < rows)
				d_table->setNumRows(rows);
		}
		else if (importFileAs == 2){//new rows
			if (c < cols)
				addColumns(cols-c);
			d_table->setNumRows(r+rows);
		}

		f.reset();
		for (i=0; i<ignoredLines; i++)
			t.readLine();

		int startRow = 0, startCol =0;
		if (importFileAs == 2)
			startRow = r;
		else if (importFileAs == 1)
			startCol = c;

		if (renameCols && !allNumbers)
		{//use first line to set the table header
			s = t.readLine();
			if (simplifySpaces)
				s = s.simplifyWhiteSpace();
			else if (stripSpaces)
				s = s.trimmed();

			line = s.split(sep, QString::SkipEmptyParts);
			int end = startCol+(int)line.count();
			for (i=startCol; i<end; i++)
				col_label[i] = QString::null;
			for (i=startCol; i<end; i++){
				comments[i] = line[i-startCol];
				s = line[i-startCol].replace("-","_").remove(QRegExp("\\W")).replace("_","-");
				int n = col_label.count(s);
				if(n){
					//avoid identical col names
					while (col_label.contains(s+QString::number(n)))
						n++;
					s += QString::number(n);
				}
				col_label[i] = s;
			}
		}
		d_table->blockSignals(true);
		setHeaderColType();

		for (i=0; i<steps; i++){
			if (progress.wasCanceled()){
				f.close();
				return;
			}

			for (int k=0; k<1000; k++){
				s = t.readLine();
				if (simplifySpaces)
					s = s.simplifyWhiteSpace();
				else if (stripSpaces)
					s = s.trimmed();
				line = s.split(sep);
				for (int j=startCol; j<numCols(); j++)
					setText(startRow + k, j, line[j-startCol]);
			}

			startRow += 1000;
			progress.setValue(i);
		}

		for (i=startRow; i<numRows(); i++){
			s = t.readLine();
			if (simplifySpaces)
				s = s.simplifyWhiteSpace();
			else if (stripSpaces)
				s = s.trimmed();
			line = s.split(sep);
			for (int j=startCol; j<numCols(); j++)
				setText(i, j, line[j-startCol]);
		}
		progress.setValue(steps+1);
		d_table->blockSignals(false);
		f.close();

		if (importFileAs)
		{
			for (i=0; i<numCols(); i++)
				emit modifiedData(this, colName(i));
		}
	}
#endif
}

void Table::importASCII(const QString &fname, const QString &sep, int ignoredLines,
		bool renameCols, bool stripSpaces, bool simplifySpaces, bool newTable)
{
	// TODO: port
#if 0
	QFile f(fname);
	if (f.open(QIODevice::ReadOnly)) //| QIODevice::Text | QIODevice::Unbuffered ))
	{
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		Q3TextStream t(&f);//TODO: use QTextStream instead and find a way to make it read the end-of-line char correctly.
		//Opening the file with the above combination doesn't seem to help: problems on Mac OS X generated ASCII files!

		int i, c, rows = 1, cols = 0;
		for (i=0; i<ignoredLines; i++)
			t.readLine();

		QString s = t.readLine();//read first line after the ignored ones
		while ( !t.atEnd() ){
			t.readLine();
			rows++;
			qApp->processEvents(QEventLoop::ExcludeUserInput);
		}

		if (simplifySpaces)
			s = s.simplifyWhiteSpace();
		else if (stripSpaces)
			s = s.trimmed();

		QStringList line = s.split(sep);
		cols = (int)line.count();

		bool allNumbers = true;
		for (i=0; i<cols; i++)
		{//verify if the strings in the line used to rename the columns are not all numbers
			QLocale().toDouble(line[i], &allNumbers);
			if (!allNumbers)
				break;
		}

		if (renameCols && !allNumbers)
			rows--;
		int steps = int(rows/1000);

		QProgressDialog progress(this);
		progress.setWindowTitle("Reading file...");
		progress.setLabelText(fname);
		progress.setActiveWindow();
		progress.setAutoClose(true);
		progress.setAutoReset(true);
		progress.setRange(0, steps+1);

		QApplication::restoreOverrideCursor();

		QStringList oldHeader;
		if (newTable)
			init (rows, cols);
		else{
			if (numRows() != rows)
				d_table->setNumRows(rows);

			c = numCols();
			oldHeader = col_label;
			if (c != cols){
				if (c < cols)
					addColumns(cols - c);
				else{
					d_table->setNumCols(cols);
					for (int i=c-1; i>=cols; i--){
						emit removedCol(QString(name()) + "_" + oldHeader[i]);
						commands.removeLast();
						comments.removeLast();
						col_format.removeLast();
						col_label.removeLast();
						colTypes.removeLast();
						col_plot_type.removeLast();
					}
				}
			}
		}

		f.reset();
		for (i=0; i<ignoredLines; i++)
			t.readLine();

		if (renameCols && !allNumbers)
		{//use first line to set the table header
			s = t.readLine();
			if (simplifySpaces)
				s = s.simplifyWhiteSpace();
			else if (stripSpaces)
				s = s.trimmed();
			line = s.split(sep, QString::SkipEmptyParts);
			for (i=0; i<(int)line.count(); i++)
				col_label[i] = QString::null;

			for (i=0; i<(int)line.count(); i++)
			{
				comments[i] = line[i];
				s = line[i].replace("-","_").remove(QRegExp("\\W")).replace("_","-");
				int n = col_label.count(s);
				if(n)
				{
					//avoid identical col names
					while (col_label.contains(s+QString::number(n)))
						n++;
					s += QString::number(n);
				}
				col_label[i] = s;
			}
		}

		d_table->blockSignals(true);
		setHeaderColType();

		int start = 0;
		for (i=0; i<steps; i++)
		{
			if (progress.wasCanceled())
			{
				f.close();
				return;
			}

			start = i*1000;
			for (int k=0; k<1000; k++)
			{
				s = t.readLine();
				if (simplifySpaces)
					s = s.simplifyWhiteSpace();
				else if (stripSpaces)
					s = s.trimmed();
				line = s.split(sep);
				int lc = (int)line.count();
				if (lc > cols) {
					addColumns(lc - cols);
					cols = lc;
				}
				for (int j=0; j<cols && j<lc; j++)
					setText(start + k, j, line[j]);
			}
			progress.setValue(i);
			qApp->processEvents();
		}

		start = steps*1000;
		for (i=start; i<rows; i++)
		{
			s = t.readLine();
			if (simplifySpaces)
				s = s.simplifyWhiteSpace();
			else if (stripSpaces)
				s = s.trimmed();
			line = s.split(sep);
			int lc = (int)line.count();
			if (lc > cols) {
				addColumns(lc - cols);
				cols = lc;
			}
			for (int j=0; j<cols && j<lc; j++)
				setText(i, j, line[j]);
		}
		progress.setValue(steps+1);
		qApp->processEvents();
		d_table->blockSignals(false);
		f.close();

		if (!newTable)
		{
			if (cols > c)
				cols = c;
			for (i=0; i<cols; i++)
			{
				emit modifiedData(this, colName(i));
				if (colLabel(i) != oldHeader[i])
					emit changedColHeader(QString(name())+"_"+oldHeader[i],
							QString(name())+"_"+colLabel(i));
			}
		}
	}
#endif
}

bool Table::exportASCII(const QString& fname, const QString& separator,
		bool withLabels,bool exportSelection)
{
	// TODO: port
#if 0
	QFile f(fname);
	if ( !f.open( QIODevice::WriteOnly ) ){
		QApplication::restoreOverrideCursor();
		QMessageBox::critical(0, tr("ASCII Export Error"),
				tr("Could not write to file: <br><h4>"+fname+ "</h4><p>Please verify that you have the right to write to this location!").arg(fname));
		return false;
	}

	QString text;
	int i,j;
	int rows=numRows();
	int cols=numCols();
	int selectedCols = 0;
	int topRow = 0, bottomRow = 0;
	int *sCols;
	if (exportSelection){
		for (i=0; i<cols; i++){
			if (d_table->isColumnSelected(i))
				selectedCols++;
		}

		sCols = new int[selectedCols];
		int aux = 1;
		for (i=0; i<cols; i++){
			if (d_table->isColumnSelected(i)){
				sCols[aux] = i;
				aux++;
			}
		}

		for (i=0; i<rows; i++)
		{
			if (d_table->isRowSelected(i))
			{
				topRow = i;
				break;
			}
		}

		for (i=rows - 1; i>0; i--)
		{
			if (d_table->isRowSelected(i))
			{
				bottomRow = i;
				break;
			}
		}
	}

	if (withLabels)
	{
		QStringList header=colNames();
		QStringList ls=header.grep ( QRegExp ("\\D"));
		if (exportSelection)
		{
			for (i=1;i<selectedCols;i++)
			{
				if (ls.count()>0)
					text+=header[sCols[i]]+separator;
				else
					text+="C"+header[sCols[i]]+separator;
			}

			if (ls.count()>0)
				text+=header[sCols[selectedCols]] + "\n";
			else
				text+="C"+header[sCols[selectedCols]] + "\n";
		}
		else
		{
			if (ls.count()>0)
			{
				for (j=0; j<cols-1; j++)
					text+=header[j]+separator;
				text+=header[cols-1]+"\n";
			}
			else
			{
				for (j=0; j<cols-1; j++)
					text+="C"+header[j]+separator;
				text+="C"+header[cols-1]+"\n";
			}
		}
	}// finished writting labels

	if (exportSelection)
	{
		for (i=topRow;i<=bottomRow; i++)
		{
			for (j=1;j<selectedCols;j++)
				text+=text(i, sCols[j]) + separator;
			text+=text(i, sCols[selectedCols]) + "\n";
		}
		delete[] sCols;//free memory
	}
	else
	{
		for (i=0;i<rows;i++)
		{
			for (j=0;j<cols-1;j++)
				text+=text(i,j)+separator;
			text+=text(i,cols-1)+"\n";
		}
	}
	QTextStream t( &f );
	t << text;
	f.close();
#endif
	return true;
}

// TODO: obsolete
#if 0
void Table::contextMenuEvent(QContextMenuEvent *e)
{
	// TODO: remove
	QRect r = d_table->horizontalHeader()->sectionRect(numCols()-1);
	setFocus();
	if (e->pos().x() > r.right() + d_table->verticalHeader()->width())
		emit showContextMenu(false);
	else
		emit showContextMenu(true);
	e->accept();
}
#endif

void Table::moveCurrentCell()
{
	// TODO: remove
#if 0
	int cols=numCols();
	int row=d_table->currentRow();
	int col=d_table->currentColumn();
	d_table->clearSelection (true);

	if (col+1<cols)
	{
		d_table->setCurrentCell(row, col+1);
		d_table->selectCells(row, col+1, row, col+1);
	}
	else
	{
		if(row+1 >= numRows())
			d_table->setNumRows(row + 11);

		d_table->setCurrentCell (row+1, 0);
		d_table->selectCells(row+1, 0, row+1, 0);
	}
#endif
}

	// TODO: remove
#if 0
bool Table::eventFilter(QObject *object, QEvent *e)
{
	Q3Header *hheader = d_table->horizontalHeader();
	Q3Header *vheader = d_table->verticalHeader();

	if (e->type() == QEvent::MouseButtonDblClick && object == (QObject*)hheader) {
		const QMouseEvent *me = (const QMouseEvent *)e;
		selectedCol = hheader->sectionAt (me->pos().x() + hheader->offset());

		QRect rect = hheader->sectionRect (selectedCol);
		rect.setLeft(rect.right() - 2);
		rect.setWidth(4);

		if (rect.contains (me->pos())) {
			d_table->adjustColumn(selectedCol);
			emit modifiedWindow(this);
		} else
			emit optionsDialog();
		setActiveWindow();
		return true;
	} else if (e->type() == QEvent::MouseButtonPress && object == (QObject*)hheader) {
		const QMouseEvent *me = (const QMouseEvent *)e;
		if (me->button() == Qt::LeftButton && me->state() == Qt::ControlButton) {
			selectedCol = hheader->sectionAt (me->pos().x() + hheader->offset());
			d_table->selectColumn (selectedCol);
			d_table->setCurrentCell (0, selectedCol);
			setActiveWindow();
			return true;
		} else if (selectedColsNumber() <= 1) {
			selectedCol = hheader->sectionAt (me->pos().x() + hheader->offset());
			d_table->clearSelection();
			d_table->selectColumn (selectedCol);
			d_table->setCurrentCell (0, selectedCol);
			setActiveWindow();
			return false;
		}
	} else if (e->type() == QEvent::MouseButtonPress && object == (QObject*)vheader) {
		const QMouseEvent *me = (const QMouseEvent *)e;
		if (me->button() == Qt::RightButton && numSelectedRows() <= 1) {
			d_table->clearSelection();
			int row = vheader->sectionAt(me->pos().y() + vheader->offset());
			d_table->selectRow (row);
			d_table->setCurrentCell (row, 0);
			setActiveWindow();
		}
	} else if (e->type()==QEvent::ContextMenu && object == titleBar) {
		emit showTitleBarMenu();
		((QContextMenuEvent*)e)->accept();
		setActiveWindow();
		return true;
	}

	return MyWidget::eventFilter(object, e);
}
#endif

void Table::customEvent(QEvent *e)
{
	if (e->type() == SCRIPTING_CHANGE_EVENT)
		scriptingChangeEvent((ScriptingChangeEvent*)e);
}

void Table::setNumRows(int rows)
{
	d_future_table->setRowCount(rows);
}

void Table::setNumCols(int cols)
{
	d_future_table->setColumnCount(cols);
}

void Table::resizeRows(int r)
{
	// TODO: obsolete
	int rows = numRows();
	if (rows == r)
		return;

	if (rows > r)
	{
		QString text= tr("Rows will be deleted from the table!");
		text+="<p>"+tr("Do you really want to continue?");
		int i,cols = numCols();
		switch( QMessageBox::information(this,tr("SciDAVis"), text, tr("Yes"), tr("Cancel"), 0, 1 ) )
		{
			case 0:
				QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
				setNumRows(r);
				for (i=0; i<cols; i++)
					emit modifiedData(this, colName(i));

				QApplication::restoreOverrideCursor();
				break;

			case 1:
				return;
				break;
		}
	}
	else
	{
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		setNumRows(r);
		QApplication::restoreOverrideCursor();
	}

	emit modifiedWindow(this);
}

void Table::resizeCols(int c)
{
	// TODO: obsolete
#if 0
	int cols = numCols();
	if (cols == c)
		return;

	if (cols > c){
		QString text= tr("Columns will be deleted from the table!");
		text+="<p>"+tr("Do you really want to continue?");
		switch( QMessageBox::information(this,tr("SciDAVis"), text, tr("Yes"), tr("Cancel"), 0, 1 ) ){
			case 0: {
						QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
						Q3MemArray<int> columns(cols-c);
						for (int i=cols-1; i>=c; i--){
							QString name = colName(i);
							emit removedCol(name);
							columns[i-c]=i;

							commands.removeLast();
							comments.removeLast();
							col_format.removeLast();
							col_label.removeLast();
							colTypes.removeLast();
							col_plot_type.removeLast();
						}

						d_table->removeColumns(columns);
						QApplication::restoreOverrideCursor();
						break;
					}

			case 1:
					return;
					break;
		}
	}
	else{
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		addColumns(c-cols);
		setHeaderColType();
		QApplication::restoreOverrideCursor();
	}
	emit modifiedWindow(this);
#endif
}

void Table::copy(Table *m)
{
	// TODO: port
#if 0
	for (int i=0; i<numRows(); i++)
	{
		for (int j=0; j<numCols(); j++)
			setText(i,j,m->text(i,j));
	}

	setColWidths(m->columnWidths());
	col_label = m->colNames();
	col_plot_type = m->plotDesignations();
	d_show_comments = m->commentsEnabled();
	comments = m->colComments();
	setHeaderColType();

	commands = m->getCommands();
	setColumnTypes(m->columnTypes());
	col_format = m->getColumnsFormat();
#endif
}

QString Table::saveAsTemplate(const QString& geometryInfo)
{
	QString s="<table>\t"+QString::number(numRows())+"\t";
	s+=QString::number(numCols())+"\n";
	s+=geometryInfo;
	s+=saveHeader();
	s+=saveColumnWidths();
	s+=saveCommands();
	s+=saveColumnTypes();
	s+=saveComments();
	return s;
}

void Table::restore(const QStringList& lst)
{
	// TODO: ...
#if 0
	QStringList l;
	QStringList::const_iterator i=lst.begin();

	l= (*i++).split("\t");
	l.remove(l.first());
	loadHeader(l);

	setColWidths((*i).right((*i).length()-9).split("\t", QString::SkipEmptyParts));
	i++;

	l = (*i++).split("\t");
	if (l[0] == "com")
	{
		l.remove(l.first());
		setCommands(l);
	} else if (l[0] == "<com>") {
		commands.clear();
		for (int col=0; col<numCols(); col++)
			commands << "";
		for (; i != lst.end() && *i != "</com>"; i++)
		{
			int col = (*i).mid(9,(*i).length()-11).toInt();
			QString formula;
			for (i++; i!=lst.end() && *i != "</col>"; i++)
				formula += *i + "\n";
			formula.truncate(formula.length()-1);
			commands[col] = formula;
		}
		i++;
	}

	l = (*i++).split("\t");
	l.remove(l.first());
	setColumnTypes(l);

	l = (*i++).split("\t");
	l.remove(l.first());
	setColComments(l);
#endif
}

void Table::notifyChanges()
{
	// TODO: replace
	for (int i=0; i<numCols(); i++)
		emit modifiedData(this, colName(i));

	emit modifiedWindow(this);
}

void Table::clear()
{
	d_future_table->clear();
}

void Table::setColumnHeader(int index, const QString& label)
{
	// TODO: remove
#if 0
	Q3Header *head = d_table->horizontalHeader();

	if (d_show_comments)
	{
		QString s = label;

		int lines = d_table->columnWidth(index)/d_table->horizontalHeader()->fontMetrics().averageCharWidth();
		head->setLabel(index, s.remove("\n") + "\n" + QString(lines, '_') + "\n" + comments[index]);
	}
	else
		head->setLabel(index, label);
#endif
}

void Table::setNumericPrecision(int prec)
{
	// TODO: obsolete
#if 0
	d_numeric_precision = prec;
	for (int i=0; i<numCols(); i++)
		col_format[i] = "0/"+QString::number(prec);
#endif
}

void Table::updateDecimalSeparators(const QLocale& oldSeparators)
{
	// TODO: replace
	for (int i=0; i<numCols(); i++){
		if (d_future_table->column(i)->columnMode() != Numeric)
			continue;

		char format;
		int prec;
		columnNumericFormat(i, &format, &prec);

		for (int j=0; j<numRows(); j++){
			if (!text(j, i).isEmpty()){
				double val = oldSeparators.toDouble(text(j, i));
				setText(j, i, QLocale().toString(val, format, prec));
			}
		}
	}
}

void Table::updateDecimalSeparators()
{
	// TODO: replace
	saveToMemory();

	for (int i=0; i<numCols(); i++){
		if (d_future_table->column(i)->columnMode() != Numeric)
			continue;

		char format;
		int prec;
		columnNumericFormat(i, &format, &prec);

		for (int j=0; j<numRows(); j++){
			if (!text(j, i).isEmpty())
				setText(j, i, QLocale().toString(d_saved_cells[i][j], format, prec));
		}
	}

	freeMemory();
}

QStringList Table::colNames()
{
	QStringList list;
	for (int i=0; i<d_future_table->columnCount(); i++)
		list << d_future_table->column(i)->name();
	return list;
}

QString Table::colLabel(int col)
{
	return d_future_table->column(col)->name();
}

SciDAVis::PlotDesignation Table::colPlotDesignation(int col)
{
	return d_future_table->column(col)->plotDesignation();
}

void Table::setColPlotDesignation(int col, SciDAVis::PlotDesignation d)
{
	d_future_table->column(col)->setPlotDesignation(d);
}

QList<int> Table::plotDesignations()
{
	QList<int> list;
	for (int i=0; i<d_future_table->columnCount(); i++)
		list << d_future_table->column(i)->plotDesignation();
	return list;
}

QStringList Table::getCommands()
{
	QStringList list;
	if (d_future_table->rowCount() < 1) 
		return list;
	for (int i=0; i<d_future_table->columnCount(); i++)
		list << d_future_table->column(i)->formula(0);
	return list;
}

QList<int> Table::columnTypes()
{
	QList<int> list;
	for (int i=0; i<d_future_table->columnCount(); i++)
		list << d_future_table->column(i)->columnMode();
	return list;
}

int Table::columnType(int col)
{
	return d_future_table->column(col)->columnMode();
}

void Table::setColumnTypes(QList<int> ctl)
{
	Q_ASSERT(ctl.size() == d_future_table->columnCount());
	for (int i=0; i<d_future_table->columnCount(); i++)
		d_future_table->column(i)->setColumnMode((SciDAVis::ColumnMode)ctl.at(i));
}

void Table::setColumnType(int col, SciDAVis::ColumnMode mode) 
{ 
	d_future_table->column(col)->setColumnMode(mode);
}

QString Table::columnFormat(int col)
{
	// TODO: obsolete
	return QString();
}

QStringList Table::getColumnsFormat()
{
	// TODO: obsolete
	return QStringList();
}

void Table::saveToMemory(double **cells)
{
	d_saved_cells = cells;
}

int Table::verticalHeaderWidth()
{
	return d_view_widget->verticalHeader()->width();
}

QString Table::colComment(int col)
{
	return d_future_table->column(col)->comment();
}

QStringList Table::colComments()
{
	QStringList list;
	for (int i=0; i<d_future_table->columnCount(); i++)
		list << d_future_table->column(i)->comment();
	return list;
}

void Table::setColComments(const QStringList& list)
{
	for (int i=0; i<d_future_table->columnCount(); i++)
		d_future_table->column(i)->setComment(list.at(i));
}

bool Table::commentsEnabled()
{
	return areCommentsShown();
}

