/***************************************************************************
    File                 : importDialog.cpp
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Ion Vasilief, Tilman Hoener zu Siederdissen
    Email                : ion_vasilief@yahoo.fr, thzs@gmx.net
    Description          : ASCII import options dialog
                           
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
#include "importDialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <q3buttongroup.h>
#include <qmessagebox.h>
#include <qregexp.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <q3vbox.h>
#include <q3whatsthis.h>
//Added by qt3to4:
#include <Q3VBoxLayout>

importDialog::importDialog( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
		setName( "importDialog" );
    setSizeGripEnabled( true );

	Q3VBox  *box=new Q3VBox (this, "box2"); 
	box->setMargin (5);
	box->setSpacing (5);

	Q3HBox  *hbox1=new Q3HBox (box, "hbox1"); 
	hbox1->setSpacing (5);

	sepText = new QLabel(hbox1);
    boxSeparator = new QComboBox(true, hbox1, "boxSeparator" );
	boxSeparator->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));

	QString help = tr("The column separator can be customized. The following special codes can be used:\n\\t for a TAB character \n\\s for a SPACE");
	help += "\n"+tr("The separator must not contain the following characters: 0-9eE.+-");
	Q3WhatsThis::add(boxSeparator, help);
	Q3WhatsThis::add(sepText, help);

	Q3HBox  *hbox2=new Q3HBox (box, "hbox2");
	hbox2->setSpacing (5);

	ignoreLabel = new QLabel(hbox2);
    boxLines = new QSpinBox(0,10000, 1, hbox2, "boxLines" );
	boxLines->setSuffix(" " + tr("lines"));
	
    boxRenameCols = new QCheckBox(box, "boxRenameCols" );

	boxStripSpaces = new QCheckBox(box, "boxStripSpaces" );
	help = tr("By checking this option all white spaces will be removed from the beginning and the end of the lines in the ASCII file.");
	help +="\n\n"+tr("Warning: checking this option leads to column overlaping if the columns in the ASCII file don't have the same number of rows.");
	help +="\n"+tr("To avoid this problem you should precisely define the column separator using TAB and SPACE characters.");

	Q3WhatsThis::add(boxStripSpaces, help);

	boxSimplifySpaces = new QCheckBox(box, "boxSimplifySpaces" );
	help = tr("By checking this option all white spaces will be removed from the beginning and the end of the lines and each sequence of internal whitespaces (including the TAB character) will be replaced with a single space.");
	help +="\n\n"+tr("Warning: checking this option leads to column overlaping if the columns in the ASCII file don't have the same number of rows.");
	help +="\n"+tr("To avoid this problem you should precisely define the column separator using TAB and SPACE characters.");
	Q3WhatsThis::add(boxSimplifySpaces, help);

	GroupBox2 = new Q3ButtonGroup(3,Qt::Horizontal,QString::null,this,"GroupBox2" );
	GroupBox2->setFlat(TRUE);
	// FIXME: replace the next line with Qt4 version
	//X GroupBox2->setLineWidth(0);
	GroupBox2->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

	buttonOk = new QPushButton(GroupBox2, "buttonOk" );
    buttonOk->setDefault( TRUE );
   
    buttonCancel = new QPushButton(GroupBox2, "buttonCancel" );
	buttonHelp = new QPushButton(GroupBox2, "buttonHelp" );
	
	Q3VBoxLayout* hlayout = new Q3VBoxLayout(this,5,5, "hlayout");
    hlayout->addWidget(box);
	hlayout->addWidget(GroupBox2);

    languageChange();
   
    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( buttonHelp, SIGNAL( clicked() ), this, SLOT( help() ) );
}

importDialog::~importDialog()
{
}

void importDialog::help()
{
QString s = tr("The column separator can be customized. The following special codes can be used:\n\\t for a TAB character \n\\s for a SPACE");
s += "\n"+tr("The separator must not contain the following characters: 0-9eE.+-") + "\n\n";
s += tr( "Remove white spaces from line ends" )+ ":\n";
s += tr("By checking this option all white spaces will be removed from the beginning and the end of the lines in the ASCII file.") + "\n\n";
s += tr( "Simplify white spaces" )+ ":\n";
s += tr("By checking this option each sequence of internal whitespaces (including the TAB character) will be replaced with a single space.");

s +="\n\n"+tr("Warning: using these two last options leads to column overlaping if the columns in the ASCII file don't have the same number of rows.");
s +="\n"+tr("To avoid this problem you should precisely define the column separator using TAB and SPACE characters.");

QMessageBox::about(0, tr("QtiPlot - Help"),s);
}

void importDialog::languageChange()
{
    setCaption( tr( "QtiPlot - ASCII Import Options" ) );
    buttonOk->setText(tr("&Apply"));
	buttonCancel->setText( tr("&Close") );	
	buttonHelp->setText( tr( "&Help" ) );

	sepText->setText( tr( "Separator" ));
	boxSeparator->clear();
	boxSeparator->insertItem(tr("TAB"));
    boxSeparator->insertItem(tr("SPACE"));
	boxSeparator->insertItem(";" + tr("TAB"));
	boxSeparator->insertItem("," + tr("TAB"));
	boxSeparator->insertItem(";" + tr("SPACE"));
	boxSeparator->insertItem("," + tr("SPACE"));
    boxSeparator->insertItem(";");
    boxSeparator->insertItem(",");

	boxRenameCols->setText(tr("Use first row to &name columns"));
	boxStripSpaces->setText(tr("&Remove white spaces from line ends"));
	boxSimplifySpaces->setText(tr("&Simplify white spaces" ));
	ignoreLabel->setText( tr( "Ignore first" ));
}

void importDialog::renameCols(bool rename)
{
boxRenameCols->setChecked(rename);	
}

void importDialog::setLines(int lines)
{
boxLines->setValue(lines);
}

void importDialog::setSeparator(const QString& sep)
{
if (sep=="\t")
	boxSeparator->setCurrentItem(0);
else if (sep==" ")
	boxSeparator->setCurrentItem(1);
else if (sep==";\t")
	boxSeparator->setCurrentItem(2);
else if (sep==",\t")
	boxSeparator->setCurrentItem(3);
else if (sep=="; ")
	boxSeparator->setCurrentItem(4);
else if (sep==", ")
	boxSeparator->setCurrentItem(5);
else if (sep==";")
	boxSeparator->setCurrentItem(6);
else if (sep==",")
	boxSeparator->setCurrentItem(7);
else
	{
	QString separator = sep;
	boxSeparator->setCurrentText(separator.replace(" ","\\s").replace("\t","\\t"));
	}
}

void importDialog::setWhiteSpaceOptions(bool strip, bool simplify)
{
boxStripSpaces->setChecked(strip);
boxSimplifySpaces->setChecked(simplify);
}

void importDialog::accept()
{
QString sep = boxSeparator->currentText();
if (boxSimplifySpaces->isChecked())
	sep.replace(tr("TAB"), " ", false);
else
	sep.replace(tr("TAB"), "\t", false);

sep.replace(tr("SPACE"), " ");
sep.replace("\\s", " ");
sep.replace("\\t", "\t");

if (sep.contains(QRegExp("[0-9.eE+-]"))!=0)
	{
	QMessageBox::warning(0, tr("QtiPlot - Import options error"),
				tr("The separator must not contain the following characters: 0-9eE.+-"));
	return;
	}
emit options(sep, boxLines->value(), boxRenameCols->isChecked(),
			  boxStripSpaces->isChecked(), boxSimplifySpaces->isChecked());
}
