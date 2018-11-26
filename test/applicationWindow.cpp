#include "ApplicationWindow.h"
#include "RenameWindowDialog.h"
#include "Folder.h"
#include <QToolBar>
#include <iostream>
using namespace std;

#include <UnitTest++/UnitTest++.h>
#include "utils.h"

SUITE(ApplicationWindow)
{
  TEST_FIXTURE(ApplicationWindow, lockToolbars)
    {
      lockToolbar(true);
      size_t toolbarCnt=0;
      for (auto i: children())
        if (auto tb=dynamic_cast<QToolBar*>(i))
          {
            CHECK(!tb->isMovable());
            toolbarCnt++;
          }
      CHECK(toolbarCnt>0);

      lockToolbar(false);
      for (auto i: children())
        if (auto tb=dynamic_cast<QToolBar*>(i))
          CHECK(tb->isMovable());
    }

  TEST_FIXTURE(ApplicationWindow, basicWindowTests)
    {
      auto t=newTable();
      CHECK(t->name()!="xxx");
      renameWindow(t,"xxx");
      CHECK_EQUAL("xxx", t->name());
      
      // test the renameWindowDialog
      auto rwd=new RenameWindowDialog(this);
      rwd->setWidget(t);
      renameWindow(t,"yyy");
      CHECK(t->name()!="xxx");
      rwd->accept(); // should change the window title back
      CHECK_EQUAL("xxx", t->name());
    }

  TEST_FIXTURE(ApplicationWindow, deleteSelectedItems)
    {
      auto t=newTable();
      bool found=false;
      QList<MyWidget*> windows = windowsList();
      for (auto i: windows)
        if (t==i) found=true;
      CHECK(found);

      addListViewItem(t);
      for (QTreeWidgetItemIterator item(&lv); *item; item++)
        if (dynamic_cast<WindowListItem*>(*item)->window()==t)
          (*item)->setSelected(true);
      t->askOnCloseEvent(false);
      ApplicationWindow::deleteSelectedItems();
      found=false;
      windows=windowsList();
      for (auto i: windows)
        if (t==i) found=false;
      CHECK(!found);
    }

  TEST_FIXTURE(ApplicationWindow, showWindowPopupMenu)
    {
      {
        // test null argument, should pop up the listview menu
        auto m1=showWindowPopupMenuImpl(nullptr);
        auto m2=showListViewPopupMenuImpl();
        CHECK(m1->actions().size());
        CHECK(m1->actions().size()==m2->actions().size());
        CHECK(m1->contentsRect()==m2->contentsRect());
      }

      // add a table, and check it pops up the window menu
      auto t=newTable();
      addListViewItem(t);

      // TODO add some more items like folder views etc to this test
      for (QTreeWidgetItemIterator item(&lv); *item; item++)
        if (auto wli=dynamic_cast<WindowListItem*>(*item))
          if (wli->window()==t)
            {
              auto m1=showWindowPopupMenuImpl(wli);
              auto m2=showWindowMenuImpl(t);
              CHECK(m1->actions().size());
              CHECK(m1->actions().size()==m2->actions().size());
              CHECK(m1->contentsRect()==m2->contentsRect());
            }

      // finally, check multi-selections
      for (QTreeWidgetItemIterator item(&lv); *item; item++)
        (*item)->setSelected(true);
      {
        auto m1=showWindowPopupMenuImpl(lv.topLevelItem(0));
        CHECK(m1);
        auto m2=showListViewSelectionMenuImpl();
        CHECK(m2);
        CHECK(m1->actions().size());
        CHECK(m1->actions().size()==m2->actions().size());
        CHECK(m1->contentsRect()==m2->contentsRect());
      }
      lv.clearSelection();

      // check behaviour on folders
  CHECK(lv.topLevelItem(0));
  addFolderListViewItem(&current_folder->addChild<Folder>("ImAFolder"));
  CHECK(lv.topLevelItem(0));
  for (QTreeWidgetItemIterator item(&lv); *item; item++)
    if (auto fli=dynamic_cast<FolderListItem*>(*item))
      {
          auto m1=showWindowPopupMenuImpl(fli);
          auto m2=showFolderPopupMenuImpl(fli, false);
          CHECK(m1->actions().size());
          CHECK(m1->actions().size()==m2->actions().size());
          CHECK(m1->contentsRect()==m2->contentsRect());
        }

    }
}
