/***************************************************************************
    File                 : importDialog.h
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
#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
//Added by qt3to4:
#include <QLabel>

class Q3ButtonGroup;
class QPushButton;
class QComboBox;
class QSpinBox;
class QCheckBox;
class QLabel;
	
class importDialog : public QDialog
{
    Q_OBJECT

public:
    importDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~importDialog();

    QPushButton* buttonOk;
	QPushButton* buttonCancel;
	QPushButton* buttonHelp;
    Q3ButtonGroup* GroupBox1, *GroupBox2;
    QComboBox* boxSeparator;
	QSpinBox* boxLines;
	QCheckBox *boxRenameCols, *boxSimplifySpaces, *boxStripSpaces;
	QLabel *ignoreLabel,*sepText;

public slots:
    virtual void languageChange();
	void setSeparator(const QString& sep);
	void setWhiteSpaceOptions(bool strip, bool simplify);
	void setLines(int lines);
	void renameCols(bool rename);
	void accept();
	void help();

signals:
	void options(const QString&, int, bool, bool, bool);
};

#endif // IMPORTDIALOG_H
