#include "ScriptingLangDialog.h"
#include "application.h"

#include <qlistbox.h>
#include <qpushbutton.h>
#include <qhbox.h>
#include <qlayout.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qobjectlist.h>

ScriptingLangDialog::ScriptingLangDialog(ScriptingEnv *env, ApplicationWindow *parent, const char *name, bool modal, WFlags fl )
  : QDialog(parent, name, modal, fl), scripted(env)
{
  setCaption(tr("QtiPlot - Select scripting language"));
 
  langList = new QListBox(this, "langList");

  QHBox *box1 = new QHBox(this, "box1");
  btnOK = new QPushButton(box1, "btnOK");
  btnOK->setText(tr("OK"));
  btnCancel = new QPushButton(box1, "btnCancel");
  btnCancel->setText(tr("Cancel"));

  QVBoxLayout *layout = new QVBoxLayout(this, 5, 5, "layout");
  layout->addWidget(langList);
  layout->addWidget(box1);

  connect(btnOK, SIGNAL(clicked()), this, SLOT(accept()));
  connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));
  
  updateLangList();
}

void ScriptingLangDialog::updateLangList()
{
  langList->clear();
  langList->insertStringList(ScriptingLangManager::languages());
  QListBoxItem *current = langList->findItem(scriptEnv->name());
  if (current)
    langList->setCurrentItem(current);
}

void ScriptingLangDialog::accept()
{
  ApplicationWindow *app = (ApplicationWindow*) parent();
  if (langList->currentText() == scriptEnv->name())
    close();
  ScriptingEnv *newEnv = ScriptingLangManager::newEnv(langList->currentText(), app);
  if (!newEnv || !newEnv->isInitialized())
  {
    QMessageBox::critical(this, tr("QtiPlot - Scripting Error"), tr("Scripting language \"%1\" failed to initialize.").arg(langList->currentText()));
    return;
  }

  QApplication::postEvent(app, new ScriptingChangeEvent(newEnv));
  QObjectList *receivers = app->queryList();
  for (QObjectListIt i(*receivers); !i.atLast(); ++i)
    QApplication::postEvent(i, new ScriptingChangeEvent(newEnv));
  delete receivers;
  close();
}

