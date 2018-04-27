#include "unittests.h"
#include <map>
#include <string>
#include <fstream>
using namespace std;

ostream& operator<<(ostream& o, const QString& s)
{return o<<s.toStdString();}

struct MenuActions: map<QString,const QMenu*>
{
  void parseQObject(QObject* q)
  {
    for (auto i: q->children())
      {
        if (auto m=dynamic_cast<QMenu*>(i))
            (*this)[m->title()]=m;
        parseQObject(i);
      }
  }
};

void Unittests::menus()
{
  // this is poisoned by other running tests, so instantiate a fresh
  // ApplicationWindow
  ApplicationWindow app;
  for (auto& l: app.locales)
    {
      app.switchToLanguage(l);
      ofstream of("defaultMenus_"+l+".menudat");
      MenuActions menuActions;
      menuActions.parseQObject(&app);
      for (auto& i: menuActions)
        for (auto j: i.second->actions())
          of << i.first <<":"<<j->text()<<"|"<<
            j->iconText()<<"|"<<j->statusTip()<<"|"<<
            j->toolTip()<<"|"<<j->isVisible()<<"|"<<
            j->whatsThis()<<endl;
    }
}
