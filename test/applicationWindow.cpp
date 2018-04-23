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
  for (QTreeWidgetItemIterator item(lv); *item; item++)
    if (dynamic_cast<WindowListItem*>(*item)->window()==t)
      (*item)->setSelected(true);
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
  // test null argument, should pop up the listview menu
  {
    auto m1=showWindowPopupMenuImpl(nullptr);
    auto m2=showListViewPopupMenuImpl();
    QVERIFY(m1->actions().size());
    QVERIFY(m1->actions().size()==m2->actions().size());
    QVERIFY(m1->contentsRect()==m2->contentsRect());
  }

  // add a table, and check it pops up the window menu
  auto t=newTable();
  addListViewItem(t);

  // TODO add some more items like folder views etc to this test
  for (QTreeWidgetItemIterator item(lv); *item; item++)
    if (auto wli=dynamic_cast<WindowListItem*>(*item))
      if (wli->window()==t)
        {
          auto m1=showWindowPopupMenuImpl(wli);
          auto m2=showWindowMenuImpl(t);
          QVERIFY(m1->actions().size());
          QVERIFY(m1->actions().size()==m2->actions().size());
          QVERIFY(m1->contentsRect()==m2->contentsRect());
        }

  // finally, check multi-selections
  for (QTreeWidgetItemIterator item(lv); *item; item++)
    (*item)->setSelected(true);
  {
    auto m1=showWindowPopupMenuImpl(lv->topLevelItem(0));
    QVERIFY(m1);
    auto m2=showListViewSelectionMenuImpl();
    QVERIFY(m2);
    QVERIFY(m1->actions().size());
    QVERIFY(m1->actions().size()==m2->actions().size());
    QVERIFY(m1->contentsRect()==m2->contentsRect());
  }
  lv->clearSelection();

  // check behaviour on folders
  QVERIFY(lv->topLevelItem(0));
  addFolderListViewItem(new Folder(current_folder, "ImAFolder"));
  QVERIFY(lv->topLevelItem(0));
  for (QTreeWidgetItemIterator item(lv); *item; item++)
    if (auto fli=dynamic_cast<FolderListItem*>(*item))
      {
          auto m1=showWindowPopupMenuImpl(fli);
          auto m2=showFolderPopupMenuImpl(fli, false);
          QVERIFY(m1->actions().size());
          QVERIFY(m1->actions().size()==m2->actions().size());
          QVERIFY(m1->contentsRect()==m2->contentsRect());
        }

}
