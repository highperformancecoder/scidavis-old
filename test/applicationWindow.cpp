#include "unittests.h"
#include "ApplicationWindow.h"
#include "RenameWindowDialog.h"
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
