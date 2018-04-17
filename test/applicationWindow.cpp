#include "unittests.h"
#include "ApplicationWindow.h"
#include "RenameWindowDialog.h"
#include "Folder.h"
#include <QTest>
#include <QToolBar>
#include <iostream>
using namespace std;

void Unittests::lockToolbars()
{
  lockToolbar(true);
  size_t toolbarCnt=0;
  for (auto i: children())
    if (auto tb=dynamic_cast<QToolBar*>(i))
      {
        QVERIFY(!tb->isMovable());
        toolbarCnt++;
      }
  QVERIFY(toolbarCnt>0);

  lockToolbar(false);
  for (auto i: children())
    if (auto tb=dynamic_cast<QToolBar*>(i))
      QVERIFY(tb->isMovable());
 
}

void Unittests::basicWindowTests()
{
  auto t=newTable();
  QVERIFY(t->name()!="xxx");
  renameWindow(t,"xxx");
  QVERIFY(t->name()=="xxx");

  // test the renameWindowDialog
  auto rwd=new RenameWindowDialog(this);
  rwd->setWidget(t);
  renameWindow(t,"yyy");
  QVERIFY(t->name()!="xxx");
  rwd->accept(); // should change the window title back
  QVERIFY(t->name()=="xxx");

}

void Unittests::deleteSelectedItems()
{
  auto t=newTable();
  bool found=false;
  unique_ptr<QWidgetList> windows(windowsList());
  for (auto i: *windows)
    if (t==i) found=true;
  QVERIFY(found);

  addListViewItem(t);
  lv->selectAll(true);
  t->askOnCloseEvent(false);
  ApplicationWindow::deleteSelectedItems();
  found=false;
  windows.reset(windowsList());
  for (auto i: *windows)
    if (t==i) found=false;
  QVERIFY(!found);

}

void Unittests::showWindowPopupMenu()
{
  auto t=newTable();
  addListViewItem(t);

  // TODO add some more items like folder views etc to this test
  for (auto item=lv->firstChild(); item; item=item->nextSibling())
    if (dynamic_cast<WindowListItem*>(item)->window()==t)
      {
        auto m1=showWindowPopupMenuImpl(item);
        auto m2=showWindowMenuImpl(t);
        QVERIFY(m1->actions().size());
        QVERIFY(m1->actions().size()==m2->actions().size());
        QVERIFY(m1->contentsRect()==m2->contentsRect());
      }
  
}
