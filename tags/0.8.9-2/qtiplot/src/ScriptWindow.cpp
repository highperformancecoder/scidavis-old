#include "ScriptWindow.h"
#include "scriptedit.h"
#include "pixmaps.h"

#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qaction.h>
#include <qfile.h>
#include <qmessagebox.h>

ScriptWindow::ScriptWindow(ScriptingEnv *env)
		:QMainWindow (0, "ScriptWindow", 0)
{
initMenu();

fileName = QString::null;

te = new ScriptEdit(env, this, name());
te->setContext(this);
setCentralWidget(te);

initActions();

setIcon(QPixmap(logo_xpm));
setCaption(tr("QtiPlot - Script Window"));
setFocusProxy(te);
setFocusPolicy(QWidget::StrongFocus);
resize(QSize(500, 300));
}

void ScriptWindow::initMenu()
{
file = new QPopupMenu( this );
menuBar()->insertItem(tr("&File"), file);

edit = new QPopupMenu(this);
menuBar()->insertItem(tr("&Edit"), edit);
	
run = new QPopupMenu(this);
menuBar()->insertItem(tr("E&xecute"), run);

menuBar()->insertItem(tr("&Hide"), this, SLOT(close()));	
}

void ScriptWindow::initActions()
{
actionNew = new QAction(QPixmap(new_xpm), tr("&New"), tr("Ctrl+N"), this);
connect(actionNew, SIGNAL(activated()), this, SLOT(newScript()));
actionNew->addTo(file);

actionOpen = new QAction(QPixmap(fileopen_xpm), tr("&Open..."), tr("Ctrl+O"), this);
connect(actionOpen, SIGNAL(activated()), this, SLOT(open()));
actionOpen->addTo(file);

actionSave = new QAction(QPixmap(filesave_xpm), tr("&Save"), tr("Ctrl+S"), this);
connect(actionSave, SIGNAL(activated()), this, SLOT(save()));
actionSave->addTo(file);

actionSaveAs = new QAction(NULL, tr("Save &As..."), QString::null, this);
connect(actionSaveAs, SIGNAL(activated()), this, SLOT(saveAs()));
actionSaveAs->addTo(file);

actionPrint = new QAction(QPixmap(fileprint_xpm), tr("&Print"), tr("Ctrl+P"), this, "print");
connect(actionPrint, SIGNAL(activated()), te, SLOT(print()));
actionPrint->addTo(file);

actionUndo = new QAction(QPixmap(undo_xpm), tr("&Undo"), tr("Ctrl+Z"), this);
connect(actionUndo, SIGNAL(activated()), te, SLOT(undo()));	
actionUndo->addTo(edit);
actionUndo->setEnabled(false);

actionRedo = new QAction(QPixmap(redo_xpm), tr("&Redo"), tr("Ctrl+Y"), this);
connect(actionRedo, SIGNAL(activated()), te, SLOT(redo()));	
actionRedo->addTo(edit);
actionRedo->setEnabled(false);
edit->insertSeparator();

actionCut = new QAction(QPixmap(cut_xpm), tr("&Cut"), tr("Ctrl+x"), this);
connect(actionCut, SIGNAL(activated()), te, SLOT(cut()));	
actionCut->addTo(edit);
actionCut->setEnabled(false);

actionCopy = new QAction(QPixmap(copy_xpm), tr("&Copy"), tr("Ctrl+C"), this);
connect(actionCopy, SIGNAL(activated()), te, SLOT(copy()));	
actionCopy->addTo(edit);
actionCopy->setEnabled(false);

actionPaste = new QAction(QPixmap(paste_xpm), tr("&Paste"), tr("Ctrl+V"), this);
connect(actionPaste, SIGNAL(activated()), te, SLOT(paste()));	
actionPaste->addTo(edit);

actionDelete = new QAction(NULL, tr("&Delete"), tr("Del"), this);
connect(actionDelete, SIGNAL(activated()), te, SLOT(del()));	
actionDelete->addTo(edit);
actionDelete->setEnabled(false);

actionExecute = new QAction(NULL, tr("E&xecute"),tr("CTRL+J"), this, "execute");
connect(actionExecute, SIGNAL(activated()), te, SLOT(execute()));
actionExecute->addTo(run);

actionExecuteAll = new QAction(NULL, tr("Execute &All"), tr("CTRL+SHIFT+J"), this, "executeAll");
connect(actionExecuteAll, SIGNAL(activated()), te, SLOT(executeAll()));
actionExecuteAll->addTo(run);

actionEval = new QAction(NULL , tr("&Evaluate Expression"), tr("CTRL+Return"), this, "evaluate");
connect(actionEval, SIGNAL(activated()), te, SLOT(evaluate()));
actionEval->addTo(run);

connect(te, SIGNAL(copyAvailable(bool)), actionDelete, SLOT(setEnabled(bool)));
connect(te, SIGNAL(copyAvailable(bool)), actionCut, SLOT(setEnabled(bool)));
connect(te, SIGNAL(copyAvailable(bool)), actionCopy, SLOT(setEnabled(bool)));
connect(te, SIGNAL(undoAvailable(bool)), actionUndo, SLOT(setEnabled(bool)));
connect(te, SIGNAL(redoAvailable(bool)), actionRedo, SLOT(setEnabled(bool)));
}

void ScriptWindow::languageChange()
{
setCaption(tr("QtiPlot - Script Window"));

menuBar()->clear();
menuBar()->insertItem(tr("&File"), file);
menuBar()->insertItem(tr("&Edit"), edit);	
menuBar()->insertItem(tr("E&xecute"), run);
menuBar()->insertItem(tr("&Hide"), this, SLOT(close()));

actionNew->setMenuText(tr("&New"));
actionNew->setAccel(tr("Ctrl+N"));

actionOpen->setMenuText(tr("&Open..."));
actionOpen->setAccel(tr("Ctrl+O"));

actionSave->setMenuText(tr("&Save"));
actionSave->setAccel(tr("Ctrl+S"));

actionSaveAs->setMenuText(tr("Save &As..."));

actionPrint->setMenuText(tr("&Print"));
actionPrint->setAccel(tr("Ctrl+P"));

actionUndo->setMenuText(tr("&Undo"));
actionUndo->setAccel(tr("Ctrl+Z"));

actionRedo->setMenuText(tr("&Redo"));
actionRedo->setAccel(tr("Ctrl+Y"));

actionCut->setMenuText(tr("&Cut"));
actionCut->setAccel(tr("Ctrl+x"));

actionCopy->setMenuText(tr("&Copy"));
actionCopy->setAccel(tr("Ctrl+C"));

actionPaste->setMenuText(tr("&Paste"));
actionPaste->setAccel(tr("Ctrl+V"));

actionDelete->setMenuText(tr("&Delete")); 
actionDelete->setAccel(tr("Del"));

actionExecute->setMenuText(tr("E&xecute"));
actionExecute->setAccel(tr("CTRL+J"));

actionExecuteAll->setMenuText(tr("Execute &All"));
actionExecuteAll->setAccel(tr("CTRL+SHIFT+J"));

actionEval->setMenuText(tr("&Evaluate Expression"));
actionEval->setAccel(tr("CTRL+Return"));
}

void ScriptWindow::newScript()
{
fileName = QString::null;
te->clear();
}

void ScriptWindow::open()
{
QString fn = te->importASCII();
if (!fn.isEmpty())
	fileName = fn;
}

void ScriptWindow::saveAs()
{
QString fn = te->exportASCII();
if (!fn.isEmpty())
	fileName = fn;
}

void ScriptWindow::save()
{
if (!fileName.isEmpty())
	{
	QFile f(fileName);
	if ( !f.open( IO_WriteOnly ) )
		{
		QMessageBox::critical(0, tr("QtiPlot - File Save Error"),
		tr("Could not write to file: <br><h4> %1 </h4><p>Please verify that you have the right to write to this location!").arg(fileName));
		return;
		}
	QTextStream t( &f );
	t.setEncoding(QTextStream::UnicodeUTF8);
	t << te->text();
	f.close();
	}
else
	saveAs();
}

void ScriptWindow::closeEvent ( QCloseEvent * e )
{
emit setVisible(false);
hide();
}
