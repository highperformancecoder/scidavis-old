/***************************************************************************
    File                 : Folder.h
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Ion Vasilief, Tilman Benkert
    Email (use @ for *)  : ion_vasilief*yahoo.fr, thzs*gmx.net
    Description          : Folder for the project explorer

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
#ifndef FOLDER_H
#define FOLDER_H

#include <qobject.h>
#include <QTreeWidget>
#include <QTreeWidgetItem>
//Added by qt3to4:
#include <QDragEnterEvent>
#include <QMouseEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QDropEvent>

#include "MyWidget.h"

class FolderListItem;
class Table;
class Matrix;
class MultiLayer;
class Note;

class QDragEnterEvent;
class QDragMoveEvent;
class QDragLeaveEvent;
class QDropEvent;

//! Folder for the project explorer
class Folder : public QObject
{
    Q_OBJECT

public:
    Folder( Folder *parent, const QString &name );

	QList<MyWidget *> windowsList(){return lstWindows;};

    void addWindow( MyWidget *w ) {
		w->setFolder(this);
		lstWindows.append( w );
	};
	void removeWindow( MyWidget *w ){
		w->setFolder(0);
		lstWindows.removeAll(w);
		if (w==d_active_window) d_active_window=0;
	};

	int windowCount(bool recursive = false) const {
	    int result = lstWindows.size();
	    if (recursive)
		foreach (Folder *folder, folders())
		    result += folder->windowCount(true);
	    return result;
	};

	//! The list of subfolder names, including first generation children only
	QStringList subfolders();

	//! The list of subfolders
	QList<Folder*> folders() const;

	//! Pointer to the subfolder called s
	Folder* findSubfolder(const QString& s, bool caseSensitive = true, bool partialMatch = false);

	//! Pointer to the first window matching the search criteria
	MyWidget* findWindow(const QString& s, bool windowNames, bool labels,
							 bool caseSensitive, bool partialMatch);

	//! get a window by name
	  /**
	   * Returns the first window with given name that inherits class cls;
	   * NULL on failure. If recursive is true, do a depth-first recursive
	   * search.
	   */
	MyWidget *window(const QString &name, const char *cls="myWidget", bool recursive=false);
	//! Return table named name or NULL
	Table *table(const QString &name, bool recursive=false) { return (Table*) window(name, "Table", recursive); }
	//! Return matrix named name or NULL
	Matrix *matrix(const QString &name, bool recursive=false) { return (Matrix*) window(name, "Matrix", recursive); }
	//! Return graph named name or NULL
	MultiLayer *graph(const QString &name, bool recursive=false) { return (MultiLayer*) window(name, "MultiLayer", recursive); }
	//! Return note named name or NULL
	Note *note(const QString &name, bool recursive=false) { return (Note*) window(name, "Note", recursive); }

	//! The complete path of the folder in the project tree
	QString path();

	//! The root of the hierarchy this folder belongs to.
	Folder* rootFolder();

	QString birthDate(){return birthdate;};
	void setBirthDate(const QString& s){birthdate = s;};

	QString modificationDate(){return modifDate;};
	void setModificationDate(const QString& s){modifDate = s;};

	//! Pointer to the corresponding QListViewItem in the main application
	FolderListItem * folderListItem(){return myFolderListItem;};
	void setFolderListItem(FolderListItem *it){myFolderListItem = it;};

    MyWidget *activeWindow(){return d_active_window;};
    void setActiveWindow(MyWidget *w){d_active_window = w;};

	// TODO: move to Aspect
	QString name(){return objectName();}
	void setName(const QString& s){setObjectName(s);}
protected:
    QString birthdate, modifDate;
    QList<MyWidget *> lstWindows;
	FolderListItem *myFolderListItem;

	//! Pointer to the active window in the folder
	MyWidget *d_active_window;
};

/*****************************************************************************
 *
 * Class WindowListItem
 *
 *****************************************************************************/
//! Windows list item class
class WindowListItem : public QTreeWidgetItem
{
public:
    WindowListItem( QTreeWidget *parent, MyWidget *w );
    static const int WindowType = QTreeWidgetItem::UserType+2;

    MyWidget *window() { return myWindow; };

protected:
    MyWidget *myWindow;
};

/*****************************************************************************
 *
 * Class FolderListItem
 *
 *****************************************************************************/
//! Folders list item class
class FolderListItem : public QTreeWidgetItem
{
public:
    FolderListItem( QTreeWidget *parent, Folder *f );
    FolderListItem( FolderListItem *parent, Folder *f );

	static const int FolderType = QTreeWidgetItem::UserType+1;

	void setActive( bool o );

    Folder *folder() { return myFolder; };
	int depth();

	//! Checks weather the folder item is a grandchild of the source folder
	/**
	 * \param src source folder item
	 */
	bool isChildOf(FolderListItem *src);

protected:
    Folder *myFolder;
};

/*****************************************************************************
 *
 * Class FolderListView
 *
 *****************************************************************************/
//! Folder list view class
class FolderListView : public QTreeWidget
{
    Q_OBJECT

public:
	FolderListView(QWidget *parent = 0, const QString name = QString() );

public slots:
	void adjustColumns();
	bool isRenaming() { return state() == QAbstractItemView::EditingState; };

protected:
	void startDrag();

    void contentsDropEvent( QDropEvent *e );
    void contentsMouseMoveEvent( QMouseEvent *e );
    void contentsMousePressEvent( QMouseEvent *e );
	void contentsMouseDoubleClickEvent( QMouseEvent* e );
	void keyPressEvent ( QKeyEvent * e );
    void contentsMouseReleaseEvent( QMouseEvent *){mousePressed = false;};
	void enterEvent(QEvent *){mousePressed = false;};

signals:
	void dragItems(QList<QTreeWidgetItem *> items);
	void dropItems(QTreeWidgetItem *dest);
	void renameItem(QTreeWidgetItem *item);
	void addFolderItem();
	void deleteSelection();

private:
	bool mousePressed;
	QPoint presspos;
};

#endif
