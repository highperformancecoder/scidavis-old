#ifndef FOLDER_H
#define FOLDER_H

#include <qobject.h>
#include <qlistview.h>
#include <qiconview.h>

#include "widget.h"

class FolderListItem;
class Table;
class Matrix;
class MultiLayer;
class Note;

class QDragEnterEvent;
class QDragMoveEvent;
class QDragLeaveEvent;
class QDropEvent;
class QDragObject;

class Folder : public QObject
{
    Q_OBJECT

public:
    Folder( Folder *parent, const QString &name );
    ~Folder();

	QPtrList<myWidget> windowsList(){return lstWindows;};

    void addWindow( myWidget *w ){ lstWindows.append( w );};
	void removeWindow( myWidget *w ){ lstWindows.take( lstWindows.find(w) );};

	void setFolderName(const QString& s){fName = s;};
    QString folderName() { return fName;};

	//! The list of subfolder names, including first generation children only
	QStringList subfolders();

	//! The list of subfolders
	QPtrList<Folder> folders();
	
	//! Pointer to the subfolder called s
	Folder* findSubfolder(const QString& s, bool caseSensitive = TRUE, bool partialMatch = FALSE);

	//! Pointer to the first window matching the search criteria
	myWidget* findWindow(const QString& s, bool windowNames, bool labels, 
							 bool caseSensitive, bool partialMatch);

	//! Return first window with given name that inherits class cls; NULL on failure.
	//! If recursive is true, do a depth-first recursive search.
	myWidget *window(const QString &name, const char *cls="myWidget", bool recursive=false);
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

	//! Size of the folder as a string
	QString sizeToString();

	QString birthDate(){return birthdate;};
	void setBirthDate(const QString& s){birthdate = s;};

	QString modificationDate(){return modifDate;};
	void setModificationDate(const QString& s){modifDate = s;};

	//! Pointer to the corresponding QListViewItem in the main application
	FolderListItem * folderListItem(){return myFolderListItem;};
	void setFolderListItem(FolderListItem *it){myFolderListItem = it;};

protected:
    QString fName, birthdate, modifDate;
    QPtrList<myWidget> lstWindows;
	FolderListItem *myFolderListItem;
};

/*****************************************************************************
 *
 * Class WindowListItem
 *
 *****************************************************************************/

class WindowListItem : public QListViewItem
{
public:
    WindowListItem( QListView *parent, myWidget *w );

    myWidget *window() { return myWindow; };
	void cancelRename(int){return;};

protected:
    myWidget *myWindow;
};

/*****************************************************************************
 *
 * Class FolderListItem
 *
 *****************************************************************************/

class FolderListItem : public QListViewItem
{
public:
    FolderListItem( QListView *parent, Folder *f );
    FolderListItem( FolderListItem *parent, Folder *f );

	enum {RTTI = 1001};

	void setActive( bool o );
	void cancelRename(int){return;};

	virtual int rtti() const {return (int)RTTI;};

    Folder *folder() { return myFolder; };

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

class FolderListView : public QListView
{
    Q_OBJECT

public:
    FolderListView( QWidget *parent = 0, const char *name = 0 );

public slots:
	void adjustColumns();

protected:
	void startDrag();

    void contentsDropEvent( QDropEvent *e );
    void contentsMouseMoveEvent( QMouseEvent *e );
    void contentsMousePressEvent( QMouseEvent *e );
	void contentsMouseDoubleClickEvent( QMouseEvent* e );
	void keyPressEvent ( QKeyEvent * e );
    void contentsMouseReleaseEvent( QMouseEvent *){mousePressed = FALSE;};
	void enterEvent(QEvent *){mousePressed = FALSE;};

signals:
	void dragItems(QPtrList<QListViewItem> items);
	void dropItems(QListViewItem *dest);
	void renameItem(QListViewItem *item);
	void addFolderItem();
	void deleteSelection();

private:
	bool mousePressed;
	QPoint presspos;
};

#endif
