/***************************************************************************
    File                 : SymbolDialog.cpp
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Ion Vasilief, Tilman Hoener zu Siederdissen
    Email                : ion_vasilief@yahoo.fr, thzs@gmx.net
    Description          : Dialog to select special text characters
                           
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
#include "symbolDialog.h"
#include <QtDebug>

SymbolDialog::SymbolDialog(CharSet charSet, QWidget* parent, Qt::WFlags fl )
: QDialog( parent, fl )
{
	setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
	setSizeGripEnabled( false );

	QFont f("Arial", 14); // it is hard to recognize the symbols in 12 pt
	setFont(f);
	
	buttons = new QButtonGroup(this);
	mainLayout = new QVBoxLayout(this);
	gridLayout = new QGridLayout();

	if (charSet == SymbolDialog::lowerGreek)
		initLowerGreekChars();
	else if (charSet == SymbolDialog::upperGreek)
		initUpperGreekChars();
	else if (charSet == SymbolDialog::mathSymbols)
		initMathSymbols();
	else if (charSet == SymbolDialog::arrowSymbols)
		initArrowSymbols();
	else
		initNumberSymbols();

	closeButton = new QPushButton(tr("close window"), this);
	
	mainLayout->addLayout( gridLayout );
	mainLayout->addWidget( closeButton );

	languageChange();

	connect(buttons, SIGNAL(buttonClicked(int)), this, SLOT(getChar(int)));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
	QShortcut *shortcut = new QShortcut(Qt::Key_Return, this);
	connect( shortcut , SIGNAL(activated()),
			this, SLOT(addCurrentChar()) );

}

void SymbolDialog::initLowerGreekChars()
{
	for (int i=0;i<25;i++)
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x3B1)));
		btn->setMaximumWidth(40);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
		buttons->addButton(btn,i+1);
		gridLayout->addWidget(btn,i/5,i%5);
	}
	numButtons = 25;
}

void SymbolDialog::initUpperGreekChars()
{
	for (int i=0;i<25;i++)
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x391)));
		btn->setMaximumWidth(40);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
		buttons->addButton(btn,i+1);
		gridLayout->addWidget(btn,i/5,i%5);
	}
	numButtons = 25;
}

void SymbolDialog::initNumberSymbols()
{
	int i, counter = 0;
	for ( i=0 ; i <= (0x216B-0x2153) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x2153)));
		btn->setMaximumWidth(40);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
		buttons->addButton(btn,counter+1);
		gridLayout->addWidget(btn,counter/8,counter%8);
	}
	for ( i=0 ; i <= (0x217B-0x2170) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x2170)));
		btn->setMaximumWidth(40);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
		buttons->addButton(btn,counter+1);
		gridLayout->addWidget(btn,counter/8,counter%8);
	}
	numButtons = counter;
}

void SymbolDialog::initMathSymbols()
{
	int i, counter = 0;
	for ( i=0 ; i <= (0x22FF-0x2200) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x2200)));
		btn->setMaximumWidth(40);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
		buttons->addButton(btn,counter+1);
		gridLayout->addWidget(btn,counter/20,counter%20);
	}
	numButtons = counter;
}

void SymbolDialog::initArrowSymbols()
{
	int i, counter = 0;
	for ( i=0 ; i <= (0x21FF-0x2190) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x2190)));
		btn->setMaximumWidth(40);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
		buttons->addButton(btn,counter+1);
		gridLayout->addWidget(btn,counter/12,counter%12);
	}
	for ( i=0 ; i <= (0x2B11-0x2B00) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x2B00)));
		btn->setMaximumWidth(40);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
		buttons->addButton(btn,counter+1);
		gridLayout->addWidget(btn,counter/12,counter%12);
	}
	numButtons = counter;
}

void SymbolDialog::addCurrentChar()
{
	for (int i=1; i < numButtons; i++)
	{
		QPushButton *btn = (QPushButton *) buttons->button(i);
		if (btn && btn->hasFocus())
			emit addLetter(btn->text());
	}
}

void SymbolDialog::getChar(int btnIndex)
{
	QPushButton * btn = (QPushButton *)buttons->button( btnIndex );
	if(btn)
		emit addLetter(btn->text());
}


void SymbolDialog::languageChange()
{
	setWindowTitle( tr( "QtiPlot - Choose Symbol" ) );
}


SymbolDialog::~SymbolDialog()
{
}


void SymbolDialog::focusInEvent( QFocusEvent * event )
{
	Q_UNUSED(event)
	// select the first button as default (in case [return] is pressed)
	((QPushButton *)buttons->button(1))->setFocus(Qt::TabFocusReason);
}
