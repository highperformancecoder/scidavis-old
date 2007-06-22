#ifndef SCRIPTING_LANG_DIALOG_H
#define SCRIPTING_LANG_DIALOG_H

#include "Scripting.h"

#include <qdialog.h>

class ApplicationWindow;
class QListBox;
class QPushButton;

class ScriptingLangDialog: public QDialog, public scripted
{
  Q_OBJECT
 
  public:
    ScriptingLangDialog(ScriptingEnv *env, ApplicationWindow *parent, const char *name, bool modal, WFlags fl);

  public slots:
    void updateLangList();
    void accept();

  private:
    QListBox *langList;
    QPushButton *btnOK, *btnCancel;
};

#endif

