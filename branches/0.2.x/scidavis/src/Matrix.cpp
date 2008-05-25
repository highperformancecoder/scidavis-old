/***************************************************************************
    File                 : Matrix.cpp
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Ion Vasilief,
                           Tilman Benkert,
                           Knut Franke
    Email (use @ for *)  : ion_vasilief*yahoo.fr, thzs*gmx.net
                           knut.franke*gmx.de
    Description          : Matrix worksheet class

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
#include "Matrix.h"
#include "future/matrix/MatrixView.h"
#include "ScriptEdit.h"

#include <QtGlobal>
#include <QTextStream>
#include <QList>
#include <QEvent>
#include <QContextMenuEvent>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QHeaderView>
#include <QDateTime>
#include <QApplication>
#include <QMessageBox>
#include <QVarLengthArray>
#include <QClipboard>
#include <QShortcut>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QLocale>

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include <gsl/gsl_linalg.h>
#include <gsl/gsl_math.h>

Matrix::Matrix(ScriptingEnv *env, int r, int c, const QString& label, QWidget* parent, const char* name, Qt::WFlags f)
	: MatrixView(label, parent, name, f), scripted(env)
{
	d_future_matrix = new future::Matrix(0, r, c, label);
	init(r, c);
}

void Matrix::init(int rows, int cols)
{	
	MatrixView::setMatrix(d_future_matrix);	
	d_future_matrix->setView(this);	
	d_future_matrix->setNumericFormat('f');
	d_future_matrix->setDisplayedDigits(6);
	d_future_matrix->setCoordinates(1.0, 10.0, 1.0, 10.0);
	// TODO
	dMatrix = 0;

	setBirthDate(d_future_matrix->creationTime().toString(Qt::LocalDate));

	// this is not very nice but works for the moment
	ui.gridLayout2->removeWidget(ui.formula_box);
	delete ui.formula_box;
    ui.formula_box = new ScriptEdit(scriptEnv, ui.formula_tab);
    ui.formula_box->setObjectName(QString::fromUtf8("formula_box"));
    ui.formula_box->setMinimumSize(QSize(60, 10));
    ui.formula_box->setAcceptRichText(false);
	ui.gridLayout2->addWidget(ui.formula_box, 1, 0, 1, 3);

	ui.add_cell_combobox->addItem("cell(i, j)");
	ui.add_function_combobox->addItems(scriptEnv->mathFunctions());

	connect(ui.button_set_formula, SIGNAL(pressed()), 
		this, SLOT(applyFormula()));
	connect(ui.add_function_button, SIGNAL(pressed()), 
		this, SLOT(addFunction()));
	connect(ui.add_cell_button, SIGNAL(pressed()), 
		this, SLOT(addCell()));

	// keyboard shortcuts
	QShortcut * sel_all = new QShortcut(QKeySequence(tr("Ctrl+A", "Matrix: select all")), this);
	connect(sel_all, SIGNAL(activated()), this, SLOT(selectAll()));
	// remark: the [TAB] behaviour is now nicely done by Qt4

	connect(d_future_matrix, SIGNAL(columnsInserted(int, int)), this, SLOT(handleChange()));
	connect(d_future_matrix, SIGNAL(columnsRemoved(int, int)), this, SLOT(handleChange()));
	connect(d_future_matrix, SIGNAL(rowsInserted(int, int)), this, SLOT(handleChange()));
	connect(d_future_matrix, SIGNAL(rowsRemoved(int, int)), this, SLOT(handleChange()));
	connect(d_future_matrix, SIGNAL(dataChanged(int, int, int, int)), this, SLOT(handleChange()));
	connect(d_future_matrix, SIGNAL(coordinatesChanged()), this, SLOT(handleChange()));
	connect(d_future_matrix, SIGNAL(formulaChanged()), this, SLOT(handleChange()));
	connect(d_future_matrix, SIGNAL(formatChanged()), this, SLOT(handleChange()));

}

void Matrix::handleChange()
{
    emit modifiedWindow(this);
}

	// TODO: handle formula entry
#if 0
void Matrix::cellEdited(int row,int col)
{
	QString cell_text = text(row,col);
	if(cell_text.isEmpty()) return;

	QString cell_formula = cell_text;

	bool ok = true;
    QLocale locale;
  	double res = locale.toDouble(cell_text, &ok);
	if (ok)
		setText(row, col, locale.toString(res, d_future_matrix->numericFormat().toAscii(), num_precision));
	else
	{
		Script *script = scriptEnv->newScript(cell_formula, this, QString("<%1_%2_%3>").arg(name()).arg(row).arg(col));
		connect(script, SIGNAL(error(const QString&,const QString&,int)), scriptEnv, SIGNAL(error(const QString&,const QString&,int)));

		script->setInt(row+1, "row");
		script->setInt(row+1, "i");
		script->setInt(col+1, "col");
		script->setInt(col+1, "j");

		QVariant ret = script->eval();
		if(ret.type()==QVariant::Int || ret.type()==QVariant::UInt || ret.type()==QVariant::LongLong
				|| ret.type()==QVariant::ULongLong)
			setText(row, col, ret.toString());
		else if(ret.canConvert(QVariant::Double))
			setText(row, col, locale.toString(ret.toDouble(), d_future_matrix->numericFormat().toAscii(), num_precision));
		else
			setText(row, col, "");
	}

    if(row+1 >= numRows())
        this->setRowCount(row + 2);

    emit modifiedWindow(this);
}
#endif

double Matrix::cell(int row, int col)
{
	// TODO
	if(dMatrix)
		return dMatrix[row][col];
	else 
		return d_future_matrix->cell(row, col);
}

void Matrix::setCell(int row, int col, double value)
{
	d_future_matrix->setCell(row, col, value);
}

QString Matrix::text(int row, int col)
{
	return d_future_matrix->text(row, col);
}

void Matrix::setText(int row, int col, const QString & new_text )
{
	bool ok = true;
    QLocale locale;
  	double res = locale.toDouble(new_text, &ok);
	if (ok)
		d_future_matrix->setCell(row, col, res);
	else
	{
		Script *script = scriptEnv->newScript(new_text, this, QString("<%1_%2_%3>").arg(name()).arg(row).arg(col));
		connect(script, SIGNAL(error(const QString&,const QString&,int)), scriptEnv, SIGNAL(error(const QString&,const QString&,int)));

		script->setInt(row+1, "row");
		script->setInt(row+1, "i");
		script->setInt(col+1, "col");
		script->setInt(col+1, "j");

		QVariant ret = script->eval();
		setCell(row, col, ret.toDouble());
	}
}

	// TODO: is this used anywhere at all?
#if 0
bool Matrix::isEmptyRow(int row)
{
	int cols = this->columnCount();

	for(int i=0; i<cols; i++)
		if (!text(row, i).isEmpty())
			return false;

	return true;
}
#endif

void Matrix::setCoordinates(double xs, double xe, double ys, double ye)
{
	d_future_matrix->setCoordinates(xs, xe, ys, ye);
}

QString Matrix::saveToString(const QString &info)
{
	QString s = "<matrix>\n";
	s += QString(name()) + "\t";
	s += QString::number(numRows())+"\t";
	s += QString::number(numCols())+"\t";
	s += birthDate() + "\n";
	s += info;
	s += "ColWidth\t" + QString::number(columnWidth(0))+"\n";
	s += "<formula>\n" + formula() + "\n</formula>\n";
	s += "TextFormat\t" + QString(d_future_matrix->numericFormat()) + "\t" + QString::number(d_future_matrix->displayedDigits()) + "\n";
	s += "WindowLabel\t" + windowLabel() + "\t" + QString::number(captionPolicy()) + "\n";
	s += "Coordinates\t" + QString::number(xStart(),'g',15) + "\t" +QString::number(xEnd(),'g',15) + "\t";
	s += QString::number(yStart(),'g',15) + "\t" + QString::number(yEnd(),'g',15) + "\n";
	s += saveText();
	s +="</matrix>\n";
	return s;
}

QString Matrix::saveAsTemplate(const QString &info)
{
	QString s= "<matrix>\t";
	s+= QString::number(numRows())+"\t";
	s+= QString::number(numCols())+"\n";
	s+= info;
	s+= "ColWidth\t" + QString::number(columnWidth(0))+"\n";
	s+= "<formula>\n" + formula() + "\n</formula>\n";
	s+= "TextFormat\t" + QString(d_future_matrix->numericFormat()) + "\t" + QString::number(d_future_matrix->displayedDigits()) + "\n";
	s+= "Coordinates\t" + QString::number(xStart(),'g',15) + "\t" +QString::number(xEnd(),'g',15) + "\t";
	s+= QString::number(yStart(),'g',15) + "\t" + QString::number(yEnd(),'g',15) + "\n";
	return s;
}

QString Matrix::saveText()
{
	QString out_text = "<data>\n";
	int cols = d_future_matrix->columnCount();
	for(int i=0; i<d_future_matrix->rowCount(); i++)
	{
		out_text += QString::number(i)+"\t";
		for (int j=0; j<cols-1; j++)
			out_text += QString::number(cell(i,j), 'e', 16)+"\t";

		out_text += QString::number(cell(i,cols-1), 'e', 16)+"\n";
	}
	return out_text + "</data>\n";
}

void Matrix::setFormula(const QString &s)
{
	d_future_matrix->setFormula(s);
}

QString Matrix::formula()
{
	return d_future_matrix->formula();
}

void Matrix::setNumericFormat(const QChar& f, int prec)
{
	if (d_future_matrix->numericFormat() == f && d_future_matrix->displayedDigits() == prec)
		return;

	d_future_matrix->setNumericFormat(f.toAscii());
	d_future_matrix->setDisplayedDigits(prec);

	emit modifiedWindow(this);
}

void Matrix::setTextFormat(const QChar &format, int precision)
{
	d_future_matrix->setNumericFormat(format.toAscii());
	d_future_matrix->setDisplayedDigits(precision);
}

// TODO: remove this
#if 0
int Matrix::columnsWidth()
{
	return this->columnWidth(0);
}
#endif

void Matrix::setColumnsWidth(int width)
{
	for(int i=0; i<d_future_matrix->columnCount(); i++)
		setColumnWidth(i, width);
}

void Matrix::setDimensions(int rows, int cols)
{
	d_future_matrix->setDimensions(rows, cols);
}

int Matrix::numRows()
{
	return d_future_matrix->rowCount();
}

void Matrix::setNumRows(int rows)
{
	d_future_matrix->setDimensions(rows, d_future_matrix->columnCount());
}

int Matrix::numCols()
{
	return d_future_matrix->columnCount();
}

void Matrix::setNumCols(int cols)
{
	d_future_matrix->setDimensions(d_future_matrix->rowCount(), cols);
}

double Matrix::determinant()
{
	int rows = numRows();
	int cols = numCols();

	if (rows != cols){
		QMessageBox::critical(0,tr("Error"),
				tr("Calculation failed, the matrix is not square!"));
		return GSL_POSINF;
	}

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	gsl_matrix *A = gsl_matrix_alloc(rows, cols);
	int i;
	for(i=0; i<rows; i++)
		for(int j=0; j<cols; j++)
			gsl_matrix_set(A, i, j, cell(i, j));

	gsl_permutation * p = gsl_permutation_alloc(rows);
	gsl_linalg_LU_decomp(A, p, &i);

	double det = gsl_linalg_LU_det(A, i);

	gsl_matrix_free(A);
	gsl_permutation_free(p);

	QApplication::restoreOverrideCursor();
	return det;
}

void Matrix::invert()
{
	int rows = numRows();
	int cols = numCols();

	if (rows != cols){
		QMessageBox::critical(0,tr("Error"),
				tr("Inversion failed, the matrix is not square!"));
		return;
	}

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	gsl_matrix *A = gsl_matrix_alloc(rows, cols);
	int i;
	for(i=0; i<rows; i++){
		for(int j=0; j<cols; j++)
			gsl_matrix_set(A, i, j, cell(i, j));
	}

	gsl_permutation * p = gsl_permutation_alloc(cols);
	gsl_linalg_LU_decomp(A, p, &i);

	gsl_matrix *inverse = gsl_matrix_alloc(rows, cols);
	gsl_linalg_LU_invert(A, p, inverse);

	gsl_matrix_free(A);
	gsl_permutation_free(p);

    this->blockSignals(true);
	for(i=0; i<rows; i++){
		for(int j=0; j<cols; j++)
			setCell(i, j, gsl_matrix_get(inverse, i, j));
	}
    this->blockSignals(false);

	gsl_matrix_free(inverse);
	QApplication::restoreOverrideCursor();
	emit modifiedWindow(this);
}

void Matrix::transpose()
{
	d_future_matrix->transpose();
}

//TODO
void Matrix::saveCellsToMemory()
{
	int rows = numRows();
	int cols = numCols();
	dMatrix = allocateMatrixData(rows, cols);
	for(int i=0; i<rows; i++)
	{// initialize the matrix to zero
		for(int j=0; j<cols; j++)
			dMatrix[i][j] = 0.0;
	}

    bool ok = true;
	for (int i=0; i<rows; i++)
	{
        for (int j=0; j<cols; j++)
        {
            dMatrix[i][j] = QLocale().toDouble(text(i, j), &ok);
            if (!ok)
                break;
        }
	}
	if (!ok){// fall back to C locale
	    ok = true;
        for (int i=0; i<rows; i++)
        {
            for (int j=0; j<cols; j++)
            {
                dMatrix[i][j] = QLocale::c().toDouble(text(i, j), &ok);
                if (!ok)
                    break;
            }
        }
	}
	if (!ok){// fall back to German locale
	    ok = true;
        for (int i=0; i<rows; i++)
        {
            for (int j=0; j<cols; j++)
            {
                dMatrix[i][j] = QLocale(QLocale::German).toDouble(text(i, j), &ok);
                if (!ok)
                    break;
            }
        }
	}
	if (!ok){// fall back to French locale
	    ok = true;
        for (int i=0; i<rows; i++)
        {
            for (int j=0; j<cols; j++)
            {
                dMatrix[i][j] = QLocale(QLocale::French).toDouble(text(i, j), &ok);
                if (!ok)
                    break;
            }
        }
	}
}

// TODO
void Matrix::forgetSavedCells()
{
	freeMatrixData(dMatrix, numRows());
	dMatrix = 0;
}

bool Matrix::recalculate()
{
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	Script *script = scriptEnv->newScript(formula(), this, QString("<%1>").arg(name()));
	connect(script, SIGNAL(error(const QString&,const QString&,int)), scriptEnv, SIGNAL(error(const QString&,const QString&,int)));
	connect(script, SIGNAL(print(const QString&)), scriptEnv, SIGNAL(print(const QString&)));
	if (!script->compile())
	{
		QApplication::restoreOverrideCursor();
		return false;
	}

    this->blockSignals(true);

	int startRow = firstSelectedRow(false);
	int	endRow = lastSelectedRow(false);
	int startCol = firstSelectedColumn(false);
	int	endCol = lastSelectedColumn(false);

	QVariant ret;
	saveCellsToMemory();
	double dx = fabs(xEnd()-xStart())/(double)(numRows()-1);
	double dy = fabs(yEnd()-yStart())/(double)(numCols()-1);
	for(int row = startRow; row <= endRow; row++)
		for(int col = startCol; col <= endCol; col++)
		{
			if (!isCellSelected(row, col)) continue;
			script->setInt(row+1, "i");
			script->setInt(row+1, "row");
			script->setDouble(yStart()+row*dy, "y");
			script->setInt(col+1, "j");
			script->setInt(col+1, "col");
			script->setDouble(xStart()+col*dx, "x");
			ret = script->eval();
			setCell(row, col, ret.toDouble());
		}
	forgetSavedCells();

    this->blockSignals(false);
	emit modifiedWindow(this);
	QApplication::restoreOverrideCursor();
	return true;
}

bool Matrix::calculate(int startRow, int endRow, int startCol, int endCol)
{
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	Script *script = scriptEnv->newScript(formula(), this, QString("<%1>").arg(name()));
	connect(script, SIGNAL(error(const QString&,const QString&,int)), scriptEnv, SIGNAL(error(const QString&,const QString&,int)));
	connect(script, SIGNAL(print(const QString&)), scriptEnv, SIGNAL(print(const QString&)));
	if (!script->compile())
	{
		QApplication::restoreOverrideCursor();
		return false;
	}

    this->blockSignals(true);
	int rows = numRows();
	int cols = numCols();

	if (endRow < 0)
		endRow = rows - 1;
	if (endCol < 0)
		endCol = cols - 1;
	if (endCol >= cols)
		setNumCols(endCol+1);
	if (endRow >= rows)
		setNumRows(endRow+1);

	QVariant ret;
	saveCellsToMemory();
	double dx = fabs(xEnd()-xStart())/(double)(numRows()-1);
	double dy = fabs(yEnd()-yStart())/(double)(numCols()-1);
	for(int row = startRow; row <= endRow; row++)
		for(int col = startCol; col <= endCol; col++)
		{
			script->setInt(row+1, "i");
			script->setInt(row+1, "row");
			script->setDouble(yStart()+row*dy, "y");
			script->setInt(col+1, "j");
			script->setInt(col+1, "col");
			script->setDouble(xStart()+col*dx, "x");
			ret = script->eval();
			setCell(row, col, ret.toDouble());
		}
	forgetSavedCells();

    this->blockSignals(false);
	emit modifiedWindow(this);
	QApplication::restoreOverrideCursor();
	return true;
}

void Matrix::clearSelection()
{
	d_future_matrix->clearSelectedCells();
}


void Matrix::copySelection()
{
	d_future_matrix->copySelection();
}

void Matrix::cutSelection()
{
	copySelection();
	clearSelection();
}

bool Matrix::rowsSelected()
{
	for(int i=0; i<numRows(); i++)
	{
		if (this->isRowSelected (i, true))
			return true;
	}
	return false;
}

void Matrix::deleteSelectedRows()
{
	d_future_matrix->removeSelectedRows();
}

void Matrix::insertColumn()
{
	d_future_matrix->insertEmptyColumns();
}

bool Matrix::columnsSelected()
{
	for(int i=0; i<numCols(); i++)
	{
		if (this->isColumnSelected (i, true))
			return true;
	}
	return false;
}

void Matrix::deleteSelectedColumns()
{
	d_future_matrix->removeSelectedColumns();
}

int Matrix::numSelectedRows()
{
	int r=0;
	for(int i=0; i<numRows(); i++)
		if (this->isRowSelected(i, true))
			r++;
	return r;
}

int Matrix::numSelectedColumns()
{
	int c=0;
	for(int i=0; i<numCols(); i++)
		if (this->isColumnSelected(i, true))
			c++;
	return c;
}

void Matrix::insertRow()
{
	d_future_matrix->insertEmptyRows();
}

void Matrix::pasteSelection()
{
	d_future_matrix->pasteIntoSelection();
}

void Matrix::customEvent(QEvent *e)
{
	if (e->type() == SCRIPTING_CHANGE_EVENT)
		scriptingChangeEvent((ScriptingChangeEvent*)e);
}

bool Matrix::eventFilter(QObject *object, QEvent *e)
{
	if (e->type()==QEvent::ContextMenu && object == titleBar)
	{
		emit showTitleBarMenu();
		((QContextMenuEvent*)e)->accept();
		return true;
	}

	return MatrixView::eventFilter(object, e);
}

void Matrix::exportPDF(const QString& fileName)
{
	print(fileName);
}

void Matrix::print()
{
    print(QString());
}

void Matrix::print(const QString& fileName)
{
// TODO
#if 0
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

		QHeaderView *vHeader = this->verticalHeader();

		int rows = numRows();
		int cols = numCols();
		int height = margin;
		int i, vertHeaderWidth = vHeader->width();
		int right = margin + vertHeaderWidth;

		// print header
		p.setFont(QFont());
		QString header_label = this->model()->headerData(0, Qt::Horizontal).toString();
		QRect br = p.boundingRect(br, Qt::AlignCenter, header_label);
		p.drawLine(right, height, right, height+br.height());
		QRect tr(br);

		for(i=0;i<cols;i++)
		{
			int w = this->columnWidth(i);
			tr.setTopLeft(QPoint(right,height));
			tr.setWidth(w);
			tr.setHeight(br.height());
			header_label = this->model()->headerData(i, Qt::Horizontal).toString();
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
		for(i=0;i<rows;i++)
		{
			right = margin;
			QString cell_text = this->model()->headerData(i, Qt::Horizontal).toString()+"\t";
			tr = p.boundingRect(tr, Qt::AlignCenter, cell_text);
			p.drawLine(right, height, right, height+tr.height());

			br.setTopLeft(QPoint(right,height));
			br.setWidth(vertHeaderWidth);
			br.setHeight(tr.height());
			p.drawText(br,Qt::AlignCenter,cell_text,-1);
			right += vertHeaderWidth;
			p.drawLine(right, height, right, height+tr.height());

			for(int j=0;j<cols;j++)
			{
				int w = this->columnWidth (j);
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
#endif
}

void Matrix::range(double *min, double *max)
{
	double d_min = cell(0, 0);
	double d_max = d_min;

	for(int i=0; i<numRows(); i++)
	{
		for(int j=0; j<numCols(); j++)
		{
			double aux = cell(i, j);
			if (aux <= d_min)
				d_min = aux;

			if (aux >= d_max)
				d_max = aux;
		}
	}

	*min = d_min;
	*max = d_max;
}

double** Matrix::allocateMatrixData(int rows, int columns)
{
	double** data = new double* [rows];
	for ( int i = 0; i < rows; ++i)
		data[i] = new double [columns];

	return data;
}

void Matrix::freeMatrixData(double **data, int rows)
{
	for ( int i = 0; i < rows; i++)
		delete [] data[i];

	delete [] data;
}

void Matrix::updateDecimalSeparators()
{
	this->update();
}

void Matrix::copy(Matrix *m)
{
	if (!m)
        return;
	
	d_future_matrix->copy(m->d_future_matrix);
}

Matrix * Matrix::fromImage(const QImage & image)
{
	future::Matrix * fm = future::Matrix::fromImage(image);
	if (!fm) return NULL;
	Matrix * m = new Matrix(0, image.height(), image.width(), tr("Matrix %1").arg(1));
	delete m->d_future_matrix;
	m->d_future_matrix = fm;
	return m;
}

void Matrix::applyFormula()
{
	setFormula(ui.formula_box->toPlainText());
	recalculate();
}

void Matrix::addFunction()
{
	static_cast<ScriptEdit *>(ui.formula_box)->insertFunction(ui.add_function_combobox->currentText());
}

void Matrix::addCell()
{
	ui.formula_box->insertPlainText(ui.add_cell_combobox->currentText());
}


