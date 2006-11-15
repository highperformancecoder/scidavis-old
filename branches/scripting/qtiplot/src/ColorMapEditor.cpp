/***************************************************************************
	File                 : ColorMapEditor.cpp
	Project              : QtiPlot
--------------------------------------------------------------------
	Copyright            : (C) 2006 by Ion Vasilief
	Email                : ion_vasilief@yahoo.fr
	Description          : A QwtLinearColorMap Editor Widget
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
#include "ColorMapEditor.h"
#include "colorButton.h"

#include <qlayout.h>
#include <qwidget.h>
#include <qtable.h>
#include <qcolordialog.h>
#include <qpushbutton.h>
#include <qhbox.h>
#include <qcursor.h>
#include <qcheckbox.h>
#include <qregexp.h>
#include <qmessagebox.h>

#include <math.h>

ColorMapEditor::ColorMapEditor(QWidget* parent)
				: QWidget(parent, 0, WDestructiveClose),
				min_val(0), 
				max_val(1),
				color_map(QwtLinearColorMap())
{
table = new QTable(this);
table->setLeftMargin(0);
table->setNumCols(2);
table->setColumnWidth (0, 20);
table->setColumnWidth (1, 80);
table->setColumnReadOnly(1, true);
table->setColumnStretchable(0, true);
table->verticalHeader()->setResizeEnabled (false);
table->horizontalHeader()->setResizeEnabled (false);
table->horizontalHeader()->setClickEnabled (false);
table->viewport()->setMouseTracking(true);
table->viewport()->installEventFilter(this);

QStringList header = QStringList() << tr("Level") << tr("Color");
table->setColumnLabels(header);
table->setMinimumHeight(6*table->horizontalHeader()->height());
table->installEventFilter(this);
table->setCurrentCell(0, 0);

connect(table, SIGNAL(clicked(int, int, int, const QPoint &)), 
		this, SLOT(showColorDialog(int, int, int, const QPoint &)));
connect(table, SIGNAL(clicked(int, int, int, const QPoint &)), 
		this, SLOT(enableButtons(int, int, int, const QPoint &)));
connect(table, SIGNAL(currentChanged (int, int)), this, SLOT(enableButtons(int, int)));
connect(table, SIGNAL(valueChanged (int, int)), this, SLOT(validateLevel(int, int)));

QHBox *hb = new QHBox (this);
insertBtn = new QPushButton(tr("&Insert Level"), hb);
insertBtn->setEnabled(false);
connect(insertBtn, SIGNAL(clicked()), this, SLOT(insertLevel()));

deleteBtn = new QPushButton(tr("&Delete Level"), hb);
deleteBtn->setEnabled(false);
connect(deleteBtn, SIGNAL(clicked()), this, SLOT(deleteLevel()));

scaleColorsBox = new QCheckBox(tr("&Scale Colors"), this);
scaleColorsBox->setChecked(true);
connect(scaleColorsBox, SIGNAL(toggled(bool)), this, SLOT(setScaledColors(bool)));

QVBoxLayout* vl = new QVBoxLayout(this, 0, 0);
vl->addWidget(table);	
vl->addWidget(hb);
vl->addWidget(scaleColorsBox);

setFocusProxy(table);
setMaximumWidth(200);
}

void ColorMapEditor::updateColorMap()
{
int rows = table->numRows();
QColor c_min = QColor(table->text(0, 1));
QColor c_max = QColor(table->text(rows - 1, 1));
QwtLinearColorMap map(c_min, c_max);
for (int i = 1; i < rows-1; i++)
	{
	QwtDoubleInterval range = QwtDoubleInterval(min_val, max_val);
	double val = (table->text(i, 0).toDouble() - min_val)/range.width();
	map.addColorStop (val, QColor(table->text(i, 1)));
	}

color_map = map;
setScaledColors(scaleColorsBox->isChecked());
}

void ColorMapEditor::setColorMap(const QwtLinearColorMap& map)
{
scaleColorsBox->setChecked(map.mode() == QwtLinearColorMap::ScaledColors);

QwtArray <double> colors = map.colorStops();
int rows = (int)colors.size();
table->setNumRows(rows);

for (int i = 0; i < rows; i++)
	{
	QwtDoubleInterval range = QwtDoubleInterval(min_val, max_val);
	double val = min_val + colors[i] * range.width();
	table->setText(i, 0, QString::number(val));

	QRect r = table->cellRect (i, 1);
	QPixmap pix = QPixmap(r.width(), r.height());
	QColor c = QColor(map.rgb(QwtDoubleInterval(0, 1), colors[i]));
	pix.fill(c);
	table->setPixmap(i, 1, pix);
	table->setText(i, 1, c.name());
	}

color_map = map;
}

void ColorMapEditor::setRange(double min, double max)
{
min_val = min;
max_val = max;
}

void ColorMapEditor::insertLevel()
{
int row = table->currentRow();
QwtDoubleInterval range = QwtDoubleInterval(min_val, max_val);

double val = 0.5*(table->text(row, 0).toDouble() + table->text(row - 1, 0).toDouble());
double mapped_val = (val - min_val)/range.width();
QColor c = QColor(color_map.rgb(QwtDoubleInterval(0, 1), mapped_val));

table->insertRows(row);

QRect r = table->cellRect (row, 1);
QPixmap pix = QPixmap(r.width(), r.height());
pix.fill(c);
table->setPixmap(row, 1, pix);

table->setText(row, 0, QString::number(val));
table->setText(row, 1, c.name());

enableButtons(table->currentRow(), 0);
updateColorMap();
}

void ColorMapEditor::deleteLevel()
{
table->removeRow (table->currentRow());
enableButtons(table->currentRow(), 0);
updateColorMap();
}

void ColorMapEditor::showColorDialog(int row, int col, int, const QPoint &)
{
if (col != 1)
	return;

QColor c = QColor(table->text(row, 1));
QColor color = QColorDialog::getColor(c, this);
if (!color.isValid() || color == c)
	return;

QRect r = table->cellRect (row, 1);
QPixmap pix = QPixmap(r.width(), r.height());
pix.fill(color);
table->setPixmap(row, 1, pix);
table->setText(row, 1, color.name());

updateColorMap();
}

bool ColorMapEditor::eventFilter(QObject *object, QEvent *e)
{
if (e->type() == QEvent::MouseMove && object == table->viewport())
	{
	const QMouseEvent *me = (const QMouseEvent *)e;
	QPoint pos = table->viewport()->mapToParent(me->pos());
	int row = table->rowAt(pos.y());
	if (table->columnAt(pos.x()) == 1 && row >= 0 && row < table->numRows())
		setCursor(QCursor(Qt::PointingHandCursor));
	else
		setCursor(QCursor(Qt::ArrowCursor));
	return true;
	}
else if (e->type() == QEvent::Leave && object == table->viewport())
	{
	setCursor(QCursor(Qt::ArrowCursor));
	return true;
	}
else if (e->type() == QEvent::KeyPress && object == table)
	{
	QKeyEvent *ke = (QKeyEvent *)e;
	if (ke->key() == Qt::Key_Return && table->currentColumn() == 1)
		{
		showColorDialog(table->currentRow(), 1, 0, QPoint());
		return true;
		}
	return false;
	}
return QObject::eventFilter(object, e);
}

void ColorMapEditor::validateLevel(int row, int col)
{
if (col)
	return;

if (row == 0 || row == table->numRows() - 1)
	{
	QMessageBox::critical(this, tr("QtiPlot - Input Error"), tr("Sorry, you cannot edit this value!"));
	if (!row)
		table->setText(0, 0, QString::number(min_val));
	else
		table->setText(row, 0, QString::number(max_val));
	return;
	}

bool user_input_error = false;
QString s = table->text(row, 0).remove("-").remove(".").remove(",").remove("+");
if (s.isEmpty() || s.contains(QRegExp("\\D")))
	{
	QMessageBox::critical(this, tr("QtiPlot - Input Error"), tr("Please enter a valid color level value!"));
	user_input_error = true;
	}

QwtDoubleInterval range = QwtDoubleInterval(min_val, max_val);
double val = table->text(row, 0).replace(",", ".").toDouble();
if (!range.contains (val) || user_input_error)
	{
	QwtArray<double> colors = color_map.colorStops();	
	val = min_val + colors[row] * range.width();
	table->setText(row, 0, QString::number(val));
	}
}

void ColorMapEditor::enableButtons(int row, int col, int, const QPoint &)
{
if (col)
	return;

if (row == 0 || row == table->numRows()-1)
	deleteBtn->setEnabled(false);
else
	deleteBtn->setEnabled(true);

if (!row)
	insertBtn->setEnabled(false);
else
	insertBtn->setEnabled(true);
}

void ColorMapEditor::setScaledColors(bool scale)
{
if (scale)
	color_map.setMode(QwtLinearColorMap::ScaledColors);
else
	color_map.setMode(QwtLinearColorMap::FixedColors);
}

