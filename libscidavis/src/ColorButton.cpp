/***************************************************************************
    File                 : ColorButton.cpp
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Ion Vasilief, Tilman Benkert
    Email (use @ for *)  : ion_vasilief*yahoo.fr, thzs*gmx.net
    Description          : A button used for color selection

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
#include "ColorButton.h"

#include <QColorDialog>
#include <QPalette>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFrame>

const QColor ColorButton::colors[] = {
  QColor(Qt::black),
  QColor(Qt::red),
  QColor(Qt::green),
  QColor(Qt::blue),
  QColor(Qt::cyan),
  QColor(Qt::magenta),
  QColor(Qt::yellow),
  QColor(Qt::darkYellow),
  QColor(Qt::darkBlue),
  QColor(Qt::darkMagenta),
  QColor(Qt::darkRed),
  QColor(Qt::darkGreen),
  QColor(Qt::darkCyan),
  QColor("#0000A0"),
  QColor("#FF8000"),
  QColor("#8000FF"),
  QColor("#FF0080"),
  QColor(Qt::white),
  QColor(Qt::lightGray),
  QColor(Qt::gray),
  QColor("#FFFF80"),
  QColor("#80FFFF"),
  QColor("#FF80FF"),
  QColor(Qt::darkGray),
};

const int ColorButton::colors_count=sizeof(colors)/sizeof(colors[0]);

ColorButton::ColorButton(QWidget *parent) : QWidget(parent)
{
	init();
}

void ColorButton::init()
{
	for (int i=0; i<8; i++)
	{
		for (int j=0; j<3; j++)
		{
			QColorDialog::setStandardColor(6*i+j,   colors[i+8*j].rgb());
		}
	}
	const int btn_size = 28;
	selectButton = new QPushButton(QPixmap(":/palette.xpm"), QString(), this);
	selectButton->setMinimumWidth(btn_size);
	selectButton->setMinimumHeight(btn_size);

	display = new QFrame(this);
	display->setLineWidth(2);
	display->setFrameStyle (QFrame::Panel | QFrame::Sunken);
	display->setMinimumHeight(btn_size);
	display->setMinimumWidth(2*btn_size);
	display->setAutoFillBackground(true);
	setColor(QColor(Qt::white));

	QHBoxLayout *l = new QHBoxLayout(this);
	l->setMargin( 0 );
	l->addWidget( display );
	l->addWidget( selectButton );

	setMaximumWidth(3*btn_size);
	setMaximumHeight(btn_size);

	connect(selectButton, SIGNAL(clicked()), this, SLOT(pickColor()));
}

void ColorButton::setColor(const QColor& c)
{
	QPalette pal;
	pal.setColor(QPalette::Window, c);
	display->setPalette(pal);
	emit changed(c);
}

QColor ColorButton::color() const
{
	return display->palette().color(QPalette::Window);
}

unsigned int ColorButton::colorIndex(const QColor& c)
{
	const QColor *ite = std::find(std::begin(colors), std::end(colors), c);
	if (ite->isValid())
		return (ite - colors);
	else
		return c.rgba();
}

QColor ColorButton::color(unsigned int colorIndex)
{
	if (colorIndex < colors_count)
		return colors[colorIndex];
	else
	{
		QColor qc = QColor::fromRgba(colorIndex);
		if (qc.isValid())
			return qc;
		else
			return QColor(Qt::black); // default color is black.
	}
}

bool ColorButton::isValidColor(const QColor& c)
{
	const QColor *ite = std::find(std::begin(colors), std::end(colors), c);
	return (ite->isValid());
}

int ColorButton::numPredefinedColors()
{
	return colors_count;
}

QSize ColorButton::sizeHint () const
{
	return QSize(4*btn_size, btn_size);
}

void ColorButton::pickColor()
{
	QColor c = QColorDialog::getColor(color(), this, "Select color", QColorDialog::DontUseNativeDialog | QColorDialog::ShowAlphaChannel);
	if ( !c.isValid() || c == color() )
		return;
	setColor ( c ) ;
}
