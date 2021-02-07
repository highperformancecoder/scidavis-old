#include "ApplicationWindowTest.h"
#include "MultiLayer.h"
#include <map>
#include <string>
#include <fstream>

#include "utils.h"

struct MenuActions : std::map<QString, const QMenu *>
{
    void parseQObject(QObject *q)
    {
        for (auto i : q->children()) {
            if (auto m = dynamic_cast<QMenu *>(i))
                (*this)[m->title()] = m;
            parseQObject(i);
        }
    }
};

TEST_F(ApplicationWindowTest, defaultMenus)
{
    for (auto &l : locales) {
        switchToLanguage(l);
        // std::ofstream needs a const char* for filename argument
        // qt4 without qt3support does not implicitly convert from QString to const char*
        std::ofstream of(("defaultMenus_" + l + ".menudat").toUtf8().constData());
        MenuActions menuActions;
        menuActions.parseQObject(this);
        for (auto &i : menuActions)
            for (auto j : i.second->actions())
                of << i.first << ":" << j->text() << "|" << j->iconText() << "|" << j->statusTip()
                   << "|" << j->toolTip() << "|" << j->isVisible() << "|" << j->whatsThis()
                   << std::endl;
    }
}

TEST_F(ApplicationWindowTest, appWithGraphMenus)
{
    std::unique_ptr<ApplicationWindow> appWithGraph(open("testProject.sciprj"));
    // find graph
    QList<MyWidget *> windows = appWithGraph->windowsList();
    for (auto &i : windows)
        if (auto ml = dynamic_cast<MultiLayer *>(i))
            appWithGraph->showCurveContextMenuImpl(ml->activeGraph()->curveKey(0));
    appWithGraph->showMarkerPopupMenuImpl();
    for (auto &l : appWithGraph->locales) {
        appWithGraph->switchToLanguage(l);
        // alternative way of getting const char*
        std::ofstream of(l.prepend("appWithGraph").append(".menudat").toStdString().c_str());
        MenuActions menuActions;
        menuActions.parseQObject(appWithGraph.get());
        for (auto &i : menuActions)
            for (auto j : i.second->actions())
                of << i.first << ":" << j->text() << "|" << j->iconText() << "|" << j->statusTip()
                   << "|" << j->toolTip() << "|" << j->isVisible() << "|" << j->whatsThis()
                   << std::endl;
    }
}
