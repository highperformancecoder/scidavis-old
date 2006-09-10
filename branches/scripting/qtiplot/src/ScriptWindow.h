#ifndef SCRIPTWINDOW_H
#define SCRIPTWINDOW_H

#include "scriptedit.h"

#include <qmainwindow.h>
class ScriptingEnv;
class QAction;

class ScriptWindow: public QMainWindow
{
    Q_OBJECT

public:
	ScriptWindow(ScriptingEnv *env);

public slots:
	void newScript();
	void open();
	void save();
	void saveAs();
	void languageChange();

signals:
	void setVisible(bool);

private:
	void closeEvent ( QCloseEvent * e );
	void initMenu();
	void initActions();
	ScriptEdit *te;

	QString fileName;

	QPopupMenu *file, *edit, *run;
	QAction *actionNew, *actionUndo, *actionRedo, *actionCut, *actionCopy, *actionPaste, *actionDelete;
	QAction *actionExecute, *actionExecuteAll, *actionEval, *actionPrint, *actionOpen;
	QAction *actionSave, *actionSaveAs;
};

#endif
