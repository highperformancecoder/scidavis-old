#ifndef SCRIPTEDIT_H
#define SCRIPTEDIT_H

#include <qtextedit.h>
#include "Scripting.h"

class QAction;
class QPopupMenu;

class ScriptEdit: public QTextEdit, public scripted
{
  Q_OBJECT
    
  public:
    ScriptEdit(ScriptingEnv *env, QWidget *parent=0, const char *name=0);
    ~ScriptEdit();
    
    void customEvent(QCustomEvent*);
    
  public slots:
    void execute();
    void executeAll();
    void evaluate();
    void print();
    void insertFunction(const QString &);
    void insertFunction(int);
    void setContext(QObject *context) { myScript->setContext(context); }
    void scriptPrint(const QString&);

  protected:
    QPopupMenu *createPopupMenu (const QPoint & pos);
    
  private:
    Script *myScript;
    QAction *actionExecute, *actionExecuteAll, *actionEval, *actionPrint;
    QPopupMenu *functionsMenu;
    bool firstOutput;

  private slots:
    void insertErrorMsg(const QString &message);
};

#endif
