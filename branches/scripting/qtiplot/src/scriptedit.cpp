#include "scriptedit.h"
#include "note.h"

#include <qaction.h>
#include <qpopupmenu.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qsimplerichtext.h>
#include <qmessagebox.h>
#include <qfiledialog.h>

ScriptEdit::ScriptEdit(ScriptingEnv *env, QWidget *parent, const char *name)
  : QTextEdit(parent, name), scripted(env)
{
  myScript = scriptEnv->newScript("", this, name);
  connect(myScript, SIGNAL(error(const QString&,const QString&,int)), this, SLOT(insertErrorMsg(const QString&)));
  connect(myScript, SIGNAL(print(const QString&)), this, SLOT(scriptPrint(const QString&)));
  
  setWordWrap(QTextEdit::NoWrap);
  setTextFormat(PlainText);
  
  actionExecute = new QAction(NULL,"E&xecute",CTRL+Key_J, this, "execute");
  connect(actionExecute, SIGNAL(activated()), this, SLOT(execute()));
  actionExecuteAll = new QAction(NULL, "Execute &All", CTRL+SHIFT+Key_J, this, "executeAll");
  connect(actionExecuteAll, SIGNAL(activated()), this, SLOT(executeAll()));
  actionEval = new QAction(NULL , "&Evaluate Expression", CTRL+Key_Return, this, "evaluate");
  connect(actionEval, SIGNAL(activated()), this, SLOT(evaluate()));
  actionPrint = new QAction(NULL, "&Print", 0, this, "print");
  connect(actionPrint, SIGNAL(activated()), this, SLOT(print()));
  actionImport = new QAction(NULL, "&Import", 0, this, "import");
  connect(actionImport, SIGNAL(activated()), this, SLOT(importASCII()));
  actionExport = new QAction(NULL, "Exp&ort", 0, this, "export");
  connect(actionExport, SIGNAL(activated()), this, SLOT(exportASCII()));
  //TODO: CTRL+Key_I -> inspect (currently "Open image file". other shortcut?)

  functionsMenu = new QPopupMenu(this, "functionsMenu");
  Q_CHECK_PTR(functionsMenu);
}

void ScriptEdit::customEvent(QCustomEvent *e)
{
  if (e->type() == SCRIPTING_CHANGE_EVENT)
  {
    scriptingChangeEvent((ScriptingChangeEvent*)e);
    delete myScript;
    myScript = scriptEnv->newScript("", this, name());
    connect(myScript, SIGNAL(error(const QString&,const QString&,int)), this, SLOT(insertErrorMsg(const QString&)));
    connect(myScript, SIGNAL(print(const QString&)), this, SLOT(scriptPrint(const QString&)));
  }
}

QPopupMenu *ScriptEdit::createPopupMenu (const QPoint & pos)
{
  QPopupMenu *menu = QTextEdit::createPopupMenu(pos);
  Q_CHECK_PTR(menu);

  actionPrint->addTo(menu);
  actionImport->addTo(menu);
  actionExport->addTo(menu);
  menu->insertSeparator();

  actionExecute->addTo(menu);
  actionExecuteAll->addTo(menu);
  actionEval->addTo(menu);
  
  if (parent()->isA("Note"))
  {
    Note *sp = (Note*) parent();
    QAction *actionAutoexec = new QAction(0, tr("Auto&exec"), 0, menu);
    actionAutoexec->setToggleAction(true);
    actionAutoexec->setOn(sp->autoexec());
    connect(actionAutoexec, SIGNAL(toggled(bool)), sp, SLOT(setAutoexec(bool)));
    actionAutoexec->addTo(menu);
  }

  functionsMenu->clear();
  functionsMenu->insertTearOffHandle();
  QStringList flist = scriptEnv->mathFunctions();
  for (int i=0; i<flist.size(); i++)
  {
    int id = functionsMenu->insertItem(flist[i], this, SLOT(insertFunction(int)));
    functionsMenu->setItemParameter(id, i);
    functionsMenu->setWhatsThis(id, scriptEnv->mathFunctionDoc(flist[i]));
  }
  menu->insertItem(tr("&Functions"),functionsMenu);
  
  return menu;
}

void ScriptEdit::insertErrorMsg(const QString &message)
{
  QString err = message;
  err.prepend("\n").replace("\n","\n#> ");
  int paraFrom, indexFrom, paraTo, indexTo;
  getSelection(&paraFrom, &indexFrom, &paraTo, &indexTo);
  removeSelection();
  setCursorPosition(paraTo, indexTo);
  insert(err,(uint)QTextEdit::CheckNewLines);
}

void ScriptEdit::scriptPrint(const QString &text)
{
  if(firstOutput) {
    int paraFrom, indexFrom, paraTo, indexTo;
    getSelection(&paraFrom, &indexFrom, &paraTo, &indexTo);
    removeSelection();
    setCursorPosition(paraTo, indexTo);
    insert("\n",(uint)QTextEdit::CheckNewLines);
    firstOutput = false;
  }
  insert(text,(uint)QTextEdit::CheckNewLines);
}

void ScriptEdit::insertFunction(const QString &fname)
{
  if (hasSelectedText())
    insert(fname+"("+selectedText()+")");
  else {
    insert(fname + "()");
    int index,para;
    getCursorPosition(&para, &index);
    setCursorPosition(para, index-1);
  }
}

void ScriptEdit::insertFunction(int nr)
{
  insertFunction(scriptEnv->mathFunctions()[nr]);
}

void ScriptEdit::execute()
{
  int paraFrom, indexFrom, paraTo, indexTo;
  QString fname = "<%1:%2>";
  fname = fname.arg(name());
  if (!hasSelectedText())
  {
    moveCursor(QTextEdit::MoveLineStart,false);
    moveCursor(QTextEdit::MoveLineEnd,true);
  }
  getSelection(&paraFrom, &indexFrom, &paraTo, &indexTo);
  fname = fname.arg(paraFrom+1);
  myScript->setName(fname);
  myScript->setCode(selectedText());
  firstOutput=true;
  myScript->exec();
  firstOutput=false;
}

void ScriptEdit::executeAll()
{
  QString fname = "<%1>";
  fname = fname.arg(name());
  myScript->setName(fname);
  myScript->setCode(text());
  moveCursor(QTextEdit::MoveEnd,false);
  firstOutput=true;
  myScript->exec();
  firstOutput=false;
}

void ScriptEdit::evaluate()
{
  int paraFrom, indexFrom, paraTo, indexTo;
  QString fname = "<%1:%2>";
  fname = fname.arg(name());
  if (!hasSelectedText())
  {
    moveCursor(QTextEdit::MoveLineStart,false);
    moveCursor(QTextEdit::MoveLineEnd,true);
  }
  getSelection(&paraFrom, &indexFrom, &paraTo, &indexTo);
  fname = fname.arg(paraFrom+1);
  myScript->setName(fname);
  myScript->setCode(selectedText());
  firstOutput=true;
  myScript->setEmitErrors(false);
  QVariant res = myScript->eval();
  myScript->setEmitErrors(true);
  if (res.isValid() && res.canCast(QVariant::String))
  {
    insert("\n#> "+res.toString()+"\n", (uint)QTextEdit::CheckNewLines);
    removeSelection();
  } else { // statement or invalid
    if (myScript->exec())
    {
      removeSelection();
      insert("\n", (uint)QTextEdit::CheckNewLines);
      removeSelection();
    }
  }
  firstOutput=false;
}

ScriptEdit::~ScriptEdit()
{
}

void ScriptEdit::print()
{
QPrinter printer;
printer.setColorMode (QPrinter::GrayScale);
if (printer.setup()) 
	{
    printer.setFullPage( TRUE );
    QPainter painter;
    if ( !painter.begin(&printer ) )
         return;

	QPaintDeviceMetrics metrics( painter.device() );
	int dpiy = metrics.logicalDpiY();
	int margin = (int) ( (1/2.54)*dpiy ); // 1 cm margins
	QRect body( margin, margin, metrics.width() - 2*margin, metrics.height() - 2*margin );
	QSimpleRichText richText(QStyleSheet::convertFromPlainText(text()), QFont(), 
				context(), styleSheet(), mimeSourceFactory(), body.height());
	richText.setWidth( &painter, body.width() );
  	QRect view( body );
	int page = 1;
	do {
	    richText.draw( &painter, body.left(), body.top(), view, colorGroup() );
	    view.moveBy( 0, body.height() );
	    painter.translate( 0 , -body.height() );
	    painter.drawText( view.right() - painter.fontMetrics().width( QString::number( page ) ),
			view.bottom() + painter.fontMetrics().ascent() + 5, QString::number( page ) );
	    if ( view.top()  >= richText.height() )
			break;
	    printer.newPage();
	    page++;
		} 
	while (TRUE);
	}
}

void ScriptEdit::importASCII(const QString &filename)
{
  QString f;
  if (filename.isEmpty())
    f = QFileDialog::getOpenFileName(QString::null, "Text (*.txt *.TXT);; Python Source (*.py)", this, 0, tr("QtiPlot - Import Text into Note"));
  else
    f = filename;
  if (f.isEmpty()) return;
  QFile file(f);
  if (!file.open(IO_ReadOnly))
  {
    QMessageBox::critical(this, tr("QtiPlot - Error Opening File"), tr("Could not open file \"%1\" for reading.").arg(f));
    return;
  }
  QTextStream s(&file);
  while (!s.atEnd())
    insert(s.readLine()+"\n");
  file.close();
}

void ScriptEdit::exportASCII(const QString &filename)
{
  QString f;
  if (filename.isEmpty())
    f = QFileDialog::getSaveFileName(QString::null, "Text (*.txt *.TXT);; Python Source (*.py)", this, 0, tr("QtiPlot - Export Note to Text File"));
  else
    f = filename;
  if (f.isEmpty()) return;
  QFile file(f);
  if (!file.open(IO_WriteOnly))
  {
    QMessageBox::critical(this, tr("QtiPlot - Error Opening File"), tr("Could not open file \"%1\" for writing.").arg(f));
    return;
  }
  QTextStream s(&file);
  s << text();
  file.close();
}

