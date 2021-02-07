#include "ApplicationWindowTest.h"
#include "RenameWindowDialog.h"
#include "Folder.h"
#include <QToolBar>
#include <iostream>

#include "utils.h"

TEST_F(ApplicationWindowTest, lockToolbars)
{
    lockToolbar(true);
    size_t toolbarCnt = 0;
    for (auto i : children())
        if (auto tb = dynamic_cast<QToolBar *>(i)) {
            EXPECT_TRUE(!tb->isMovable());
            toolbarCnt++;
        }
    EXPECT_TRUE(toolbarCnt > 0);

    lockToolbar(false);
    for (auto i : children())
        if (auto tb = dynamic_cast<QToolBar *>(i)) {
            EXPECT_TRUE(tb->isMovable());
        }
}

TEST_F(ApplicationWindowTest, basicWindowTests)
{
    auto t = newTable();
    EXPECT_TRUE(t->name() != "xxx");
    renameWindow(t, "xxx");
    EXPECT_EQ("xxx", t->name());

    // test the renameWindowDialog
    auto rwd = new RenameWindowDialog(this);
    rwd->setWidget(t);
    renameWindow(t, "yyy");
    EXPECT_TRUE(t->name() != "xxx");
    rwd->accept(); // should change the window title back
    EXPECT_EQ("xxx", t->name());
}

TEST_F(ApplicationWindowTest, deleteSelectedItems)
{
    auto t = newTable();
    bool found = false;
    QList<MyWidget *> windows = windowsList();
    for (auto i : windows)
        if (t == i)
            found = true;
    EXPECT_TRUE(found);

    addListViewItem(t);
    for (QTreeWidgetItemIterator item(&lv); *item; item++)
        if (dynamic_cast<WindowListItem *>(*item)->window() == t)
            (*item)->setSelected(true);
    t->askOnCloseEvent(false);
    ApplicationWindow::deleteSelectedItems();
    found = false;
    windows = windowsList();
    for (auto i : windows)
        if (t == i)
            found = false;
    EXPECT_TRUE(!found);
}

TEST_F(ApplicationWindowTest, showWindowPopupMenu)
{
    {
        // test null argument, should pop up the listview menu
        auto m1 = showWindowPopupMenuImpl(nullptr);
        auto m2 = showListViewPopupMenuImpl();
        EXPECT_TRUE(m1->actions().size());
        EXPECT_TRUE(m1->actions().size() == m2->actions().size());
        EXPECT_TRUE(m1->contentsRect() == m2->contentsRect());
    }

    // add a table, and check it pops up the window menu
    auto t = newTable();
    addListViewItem(t);

    // TODO add some more items like folder views etc to this test
    for (QTreeWidgetItemIterator item(&lv); *item; item++)
        if (auto wli = dynamic_cast<WindowListItem *>(*item))
            if (wli->window() == t) {
                auto m1 = showWindowPopupMenuImpl(wli);
                auto m2 = showWindowMenuImpl(t);
                EXPECT_TRUE(m1->actions().size());
                EXPECT_TRUE(m1->actions().size() == m2->actions().size());
                EXPECT_TRUE(m1->contentsRect() == m2->contentsRect());
            }

    // finally, check multi-selections
    for (QTreeWidgetItemIterator item(&lv); *item; item++)
        (*item)->setSelected(true);
    {
        auto m1 = showWindowPopupMenuImpl(lv.topLevelItem(0));
        EXPECT_TRUE(m1);
        auto m2 = showListViewSelectionMenuImpl();
        EXPECT_TRUE(m2);
        EXPECT_TRUE(m1->actions().size());
        EXPECT_TRUE(m1->actions().size() == m2->actions().size());
        EXPECT_TRUE(m1->contentsRect() == m2->contentsRect());
    }
    lv.clearSelection();

    // check behaviour on folders
    EXPECT_TRUE(lv.topLevelItem(0));
    addFolderListViewItem(&current_folder->addChild<Folder>("ImAFolder"));
    EXPECT_TRUE(lv.topLevelItem(0));
    for (QTreeWidgetItemIterator item(&lv); *item; item++)
        if (auto fli = dynamic_cast<FolderListItem *>(*item)) {
            auto m1 = showWindowPopupMenuImpl(fli);
            auto m2 = showFolderPopupMenuImpl(fli, false);
            EXPECT_TRUE(m1->actions().size());
            EXPECT_TRUE(m1->actions().size() == m2->actions().size());
            EXPECT_TRUE(m1->contentsRect() == m2->contentsRect());
        }
}
