#ifndef SCRIPTEDIT_H
#define SCRIPTEDIT_H

#include <qtextedit.h>
#include "Scripting.h"

class QAction;

class ScriptEdit: public QTextEdit
{
  Q_OBJECT
    
  public:
    ScriptEdit(ScriptingEnv *env, QWidget *parent=0, const char *name=0);
    ~ScriptEdit();
    
  public slots:
    void execute();
    void executeAll();
    void evaluate();
    void print();

  protected:
    QPopupMenu *createPopupMenu (const QPoint & pos);
    
  private:
    Script *myScript;
    QAction *actionDo, *actionDoAll, *actionEval, *actionPrint;

  private slots:
    void insertErrorMsg(const QString &message);
};

#endif
