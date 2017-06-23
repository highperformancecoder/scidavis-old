#include "unittests.h"
#include "ApplicationWindow.h"
#include <QTest>
#include <QToolBar>
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
