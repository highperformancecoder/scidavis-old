/***************************************************************************
    File                 : MyWidget.cpp
    Project              : SciDAVis
    Description          : MDI window widget
    --------------------------------------------------------------------
    Copyright            : (C) 2006-2009 Knut Franke (knut.franke*gmx.de)
    Copyright            : (C) 2006-2009 Tilman Benkert (thzs*gmx.net)
    Copyright            : (C) 2006-2007 by Ion Vasilief (ion_vasilief*yahoo.fr)
                           (replace * with @ in the email address)

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
#include "MyWidget.h"
#include "Folder.h"

#include <QMessageBox>
#include <QEvent>
#include <QCloseEvent>
#include <QString>
#include <QLocale>
#include <QIcon>
#include <QtDebug>

MyWidget::MyWidget(const QString& label, QWidget * parent, const QString name, Qt::WindowFlags f):
	QMdiSubWindow (parent, f)
{
	w_label = label;
	caption_policy = Both;
	askOnClose = true;
	w_status = Normal;
	setObjectName(QString(name));
}

void MyWidget::updateCaption()
{
	switch (caption_policy)
	{
		case Name:
                  setWindowTitle(name().c_str());
                  break;

		case Label:
                  if (!windowLabel().isEmpty())
                    setWindowTitle(windowLabel());
                  else
                    setWindowTitle(name().c_str());
                  break;

		case Both:
			if (!windowLabel().isEmpty())
                          setWindowTitle(QString(name().c_str()) + " - " + windowLabel());
			else
                          setWindowTitle(QString(name().c_str()));
			break;
	}
};

void MyWidget::closeEvent( QCloseEvent *e )
{
if (askOnClose)
    {
    switch( QMessageBox::information(this,tr("SciDAVis"),
					tr("Do you want to hide or delete") + "<p><b>'" + objectName() + "'</b> ?",
				      tr("Delete"), tr("Hide"), tr("Cancel"), 0,2))
		{
		case 0:
			emit closedWindow(this);
			e->accept();
		break;

		case 1:
			e->ignore();
			emit hiddenWindow(this);
		break;

		case 2:
			e->ignore();
		break;
		}
    }
else
    {
	emit closedWindow(this);
    e->accept();
    }
}

QString MyWidget::aspect()
{
	QString s = tr("Normal");
	switch (w_status)
	{
		case Hidden:
			return tr("Hidden");
			break;

		case Normal:
			break;

		case Minimized:
			return tr("Minimized");
			break;

		case Maximized:
			return tr("Maximized");
			break;
	}
	return s;
}

void MyWidget::changeEvent(QEvent *event)
{
	if (!isHidden() && event->type() == QEvent::WindowStateChange) {
		if (((QWindowStateChangeEvent *)event)->oldState() == windowState())
			return;
		if( windowState() & Qt::WindowMinimized )
			w_status = Minimized;
		else if ( windowState() & Qt::WindowMaximized )
			w_status = Maximized;
		else
			w_status = Normal;
		emit statusChanged (this);
	}
	QMdiSubWindow::changeEvent(event);
}

void MyWidget::contextMenuEvent(QContextMenuEvent *e)
{
	if (!this->widget()->geometry().contains(e->pos())) {
		emit showTitleBarMenu();
		e->accept();
	}
}

void MyWidget::setStatus(Status s)
{
	if (w_status == s)
		return;

	w_status = s;
	emit statusChanged (this);
}

void MyWidget::setHidden()
{
    w_status = Hidden;
    emit statusChanged (this);
    hide();
}

void MyWidget::setNormal()
{
	showNormal();
	w_status = Normal;
	emit statusChanged (this);
}

void MyWidget::setMinimized()
{
	showMinimized();
	w_status = Minimized;
	emit statusChanged (this);
}

void MyWidget::setMaximized()
{
	showMaximized();
	w_status = Maximized;
	emit statusChanged (this);
}


