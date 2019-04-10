/***************************************************************************
    File                 : Folder.cpp
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Ion Vasilief,
                           Tilman Benkert,
					  Knut Franke
    Email (use @ for *)  : ion_vasilief*yahoo.fr, thzs*gmx.net,
                           knut.franke*gmx.de
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
#include "Folder.h"
#ifdef SCRIPTING_PYTHON
#include "PythonScripting.h"
#include "ApplicationWindow.h"
#endif

#include <classdesc_epilogue.h>

#include <qobject.h>
#include <qdatetime.h>
#include <qpixmap.h>

#include <qevent.h>
#include <qpoint.h>
#include <qmessagebox.h>
#include <qstringlist.h>
#include <qapplication.h>
#include <qcursor.h>
//Added by qt3to4:
#include <QKeyEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QDrag>

Folder::Folder(const QString &name )
	: d_active_window(0)
{
	QObject::setObjectName(name);
	birthdate = QDateTime::currentDateTime ().toString(Qt::LocalDate);

	// FIXME: This doesn't work anymore in Qt4, need alternative method
	// lstWindows.setAutoDelete( true );
}

PyQtList<Folder> Folder::folders() const
{
	PyQtList<Folder> lst;
	foreach(QObject *f, children())
		lst.append((Folder*) f);
	return lst;
}

void Folder::addChild(Folder *f)
{
  QString name = f->objectName();
  int i = 1;
  while (subfolders().contains(name))
    name = f->objectName() + QString::number(i++);
  if (f->parent() == 0) { // adding newly constructed folder
    f->setParent(this);
    f->setObjectName(name);
#if SCRIPTING_PYTHON
    theApp().addFolderListViewItem(f);
#endif
    f->setFolderListItem(new FolderListItem(folderListItem(), f));
  } else { // moving existing folder
#if SCRIPTING_PYTHON
    theApp().moveFolder(f->folderListItem(), folderListItem());
#endif
 }
}

QStringList Folder::subfolders()
{
	QStringList list = QStringList();
	QObjectList folderList = children();
	if (!folderList.isEmpty()){
		QObject * f;
		foreach(f,folderList)
                  list << static_cast<Folder *>(f)->name().c_str();
	}
	return list;
}

QString Folder::path()
{
  QString s = ("/" + name() + "/").c_str();
  Folder *parentFolder = (Folder *)parent();
  while (parentFolder){
    s.prepend(("/" + parentFolder->name()).c_str());
    parentFolder = (Folder *)parentFolder->parent();
  }
  return s;
}

Folder* Folder::findSubfolder(const QString& s, bool caseSensitive, bool partialMatch)
{
  QObjectList folderList = children();
  if (!folderList.isEmpty()){
    QObject * f;

    foreach(f,folderList){
      QString name = static_cast<Folder *>(f)->name().c_str();
      if (partialMatch){
        if (caseSensitive && name.startsWith(s,Qt::CaseSensitive))
          return static_cast<Folder *>(f);
        else if (!caseSensitive && name.startsWith(s,Qt::CaseInsensitive))
          return static_cast<Folder *>(f);
      }
      else // partialMatch == false
        {
          if (caseSensitive && name == s)
            return static_cast<Folder *>(f);
          else if ( !caseSensitive && (name.toLower() == s.toLower()) )
            return static_cast<Folder *>(f);
        }
    }
  }
  return 0;
}

MyWidget& Folder::findWindow(const QString& s, bool windowNames, bool labels,
                             bool caseSensitive, bool partialMatch) const
{
  Qt::CaseSensitivity cs = Qt::CaseSensitive;
  if (!caseSensitive)
    cs = Qt::CaseInsensitive;
  for(auto w:lstWindows)
    {
      if (windowNames)
        {
          QString name = w->name().c_str();
          if (partialMatch && name.startsWith(s, cs))
            return *w;
          else if (caseSensitive && name == s)
            return *w;
          else
            {
              QString text = s;
              if (name == text.toLower())
                return *w;
            }
        }

      if (labels)
        {
          QString label = w->windowLabel();
          if (partialMatch && label.startsWith(s, cs))
            return *w;
          else if (caseSensitive && label == s)
            return *w;
          else
            {
              QString text = s;
              if (label == text.toLower())
                return *w;
            }
        }
    }
  throw NoSuchObject();
}

MyWidget *Folder::window(const QString &name, const char *cls, bool recursive)
{
	foreach (MyWidget *w, lstWindows)
          if (w->inherits(cls) && name == QString(w->name().c_str()).mid(0,QString(w->name().c_str()).indexOf("@")))
			return w;
	if (!recursive) return NULL;
	foreach (QObject *f, children())
	{
		MyWidget *w = ((Folder*)f)->window(name, cls, true);
		if (w) return w;
	}
	return NULL;
}

Folder& Folder::rootFolder()
{
	Folder *i = this;
	while(i->parent())
          {
            assert(i!=i->parent());
            i = (Folder*)i->parent();
          }
	return *i;
}

void Folder::save(const std::string& fileName)
{
#ifdef SCRIPTING_PYTHON
  theApp().saveFolder(this,fileName.c_str());
#else
  throw std::runtime_error("Folders can only be saved in a python build");
#endif
}

/*****************************************************************************
 *
 * Class FolderListItem
 *
 *****************************************************************************/

FolderListItem::FolderListItem( QTreeWidget *parent, Folder *f )
    : QTreeWidgetItem( parent, FolderListItem::FolderType )
{
    myFolder = f;

    setText( 0, f->name().c_str() );
	setExpanded( true );
	setActive( true );
	setFlags(flags() | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
}

FolderListItem::FolderListItem( FolderListItem *parent, Folder *f )
    : QTreeWidgetItem( parent, FolderListItem::FolderType )
{
    myFolder = f;

    setText( 0, f->name().c_str() );
	setExpanded( true );
	setActive( true );
}

void FolderListItem::setActive( bool o )
{
    if ( o )
		setIcon(0, QIcon(QPixmap(":/folder_open.xpm")) );
    else
		setIcon(0, QIcon(QPixmap(":/folder_closed.xpm")) );

	setSelected(o);
}

bool FolderListItem::isChildOf(FolderListItem *src)
{
FolderListItem *parent = (FolderListItem *)this->parent();
while (parent)
	{
	if (parent == src)
		return true;

	parent = (FolderListItem *)parent->parent();
	}
return false;
}

int FolderListItem::depth()
{
	int c = 0;
	FolderListItem *parent = (FolderListItem *)this->parent();
	while (parent)
		{
		c++;
		parent = (FolderListItem *)parent->parent();
		}
	return c;
}

void FolderListItem::setData( int column, int role, const QVariant& value)
{
	if ((role == Qt::EditRole) && (column == 0) && (value != this->text(0)))
	{// data should be accepted or rejected on slot connected to this signal
		emit this->folderListView()->itemRenamed(this, column, value.toString());
	} else {
		QTreeWidgetItem::setData( column, role, value);
	}
}
/*****************************************************************************
 *
 * Class FolderListView
 *
 *****************************************************************************/

FolderListView::FolderListView(const QString& name )
	: mousePressed( false )
{
    setObjectName(name.toLocal8Bit());
    setAcceptDrops( true );
    viewport()->setAcceptDrops( true );
    setDropIndicatorShown(true);
}

void FolderListView::startDrag()
{
QTreeWidgetItem *item = currentItem();
if (!item)
	return;

if (item == topLevelItem(0) && item->treeWidget()->rootIsDecorated())
	return;//it's the project folder so we don't want the user to move it

QPixmap pix;
if (item->type() == FolderListItem::FolderType)
	pix = QPixmap(":/folder_closed.xpm");
else
	pix = item->icon(0).pixmap(QSize());

QDrag *drag = new QDrag(viewport());
drag->setPixmap(pix);
drag->setHotSpot(QPoint(pix.width()/2, pix.height()/2 ) );

QList<QTreeWidgetItem *> lst;
QTreeWidgetItemIterator it(this);
while (*it)
	{
	if ((*it)->isSelected())
		lst.append(*it);
	it++;
	}

emit dragItems(lst);
	drag->setMimeData(mimeData(lst));
	drag->exec();
}

void FolderListView::dropEvent( QDropEvent *e )
{
QTreeWidgetItem *dest = itemAt( e->pos() );
if ( dest && dest->type() == FolderListItem::FolderType)
{
	if (dropIndicatorPosition() != QAbstractItemView::OnItem)
	{
		e->ignore();
	}
	else
	{
		emit dropItems(dest);
		e->accept();
		this->setState(QAbstractItemView::NoState); // hack to clear DraggingState
    }
    mousePressed = false;
}
else
	e->ignore();
}

void FolderListView::keyPressEvent ( QKeyEvent * e )
{
if (isRenaming())
	{
	e->ignore();
	return;
	}
QTreeWidgetItem *item = currentItem();
if (!item) {
	QTreeWidget::keyPressEvent ( e );
	return;
}

if (item->type() == FolderListItem::FolderType &&
	(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return))
	{
	emit itemDoubleClicked(item,0);
	e->accept();
	}
else if (e->key() == Qt::Key_F2)
	{
	if (item)
		emit renameItem(item,0);
	e->accept();
	}
else if(e->key() == Qt::Key_A && e->modifiers() == Qt::ControlModifier)
	{
	selectAll();
	e->accept();
	}
else if(e->key() == Qt::Key_F7)
	{
	emit addFolderItem();
	e->accept();
	}
else if(e->key() == Qt::Key_F8)
	{
	emit deleteSelection();
	e->accept();
	}
else
	QTreeWidget::keyPressEvent ( e );
}

void FolderListView::mouseDoubleClickEvent( QMouseEvent* e )
{
	if (isRenaming())
		{
		e->ignore();
		return;
		}

	QTreeWidget::mouseDoubleClickEvent( e );
}

void FolderListView::mousePressEvent( QMouseEvent* e )
{
	QTreeWidget::mousePressEvent(e);
	if (e->button() == Qt::LeftButton)
	{
		presspos = e->pos();
		mousePressed = true;
	}
}

void FolderListView::mouseMoveEvent( QMouseEvent* e )
{
if ( mousePressed && ( presspos - e->pos() ).manhattanLength() > QApplication::startDragDistance() )
	{
	mousePressed = false;
	QTreeWidgetItem *item = itemAt( presspos );
	if ( item )
		startDrag();
    }
}

void FolderListView::adjustColumns()
{
for (int i=0; i < columnCount (); i++)
	resizeColumnToContents(i);
}

/*****************************************************************************
 *
 * Class WindowListItem
 *
 *****************************************************************************/

WindowListItem::WindowListItem( QTreeWidget *parent, MyWidget *w )
    : QTreeWidgetItem( parent, WindowListItem::WindowType )
{
    myWindow = w;
	this->treeWidget()->setCurrentItem(this,0, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

	setFlags((flags() | Qt::ItemIsDragEnabled) & ~Qt::ItemIsDropEnabled);
}

void WindowListItem::setData( int column, int role, const QVariant& value)
{
	if (role == Qt::EditRole)
	{// data should be accepted or rejected on slot connected to this signal
		if (column == 0)
			emit this->folderListView()->itemRenamed(this, column, value.toString());
	} else {
		QTreeWidgetItem::setData( column, role, value);
	}
}
