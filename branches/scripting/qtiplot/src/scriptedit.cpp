#include "scriptedit.h"
#include "note.h"

#include <qaction.h>
#include <qpopupmenu.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qsimplerichtext.h>

ScriptEdit::ScriptEdit(ScriptingEnv *env, QWidget *parent, const char *name)
  : QTextEdit(parent, name), scriptEnv(env)
{
  myScript = env->newScript("", this, name);
  connect(myScript, SIGNAL(error(const QString&,const QString&,int)), this, SLOT(insertErrorMsg(const QString&)));
  
  setWordWrap(QTextEdit::NoWrap);
  setTextFormat(PlainText);
  
  actionDo = new QAction(NULL,"&Do It",CTRL+Key_J, this, "doit");
  connect(actionDo, SIGNAL(activated()), this, SLOT(execute()));
  actionDoAll = new QAction(NULL, "Do &All", CTRL+SHIFT+Key_J, this, "doall");
  connect(actionDoAll, SIGNAL(activated()), this, SLOT(executeAll()));
  actionEval = new QAction(NULL , "&Evaluate Expression", CTRL+Key_Return, this, "evaluate");
  connect(actionEval, SIGNAL(activated()), this, SLOT(evaluate()));
  actionPrint = new QAction(NULL, "&Print ScratchPad", 0, this, "print");
  connect(actionPrint, SIGNAL(activated()), this, SLOT(print()));
  //CTRL+Key_I -> inspect (currently "Open image file". other shutcut?)
}

QPopupMenu *ScriptEdit::createPopupMenu (const QPoint & pos)
{
  QPopupMenu *menu = QTextEdit::createPopupMenu(pos);
  Q_CHECK_PTR(menu);

  actionPrint->addTo(menu);
  menu->insertSeparator();

  actionDo->addTo(menu);
  actionDoAll->addTo(menu);
  actionEval->addTo(menu);
  
  if (parent()->isA("Note"))
  {
    Note *sp = (Note*) parent();
    QAction *actionAutoexec = new QAction(0, tr("Auto&exec"), 0, sp);
    actionAutoexec->setToggleAction(true);
    actionAutoexec->setOn(sp->autoexec());
    connect(actionAutoexec, SIGNAL(toggled(bool)), sp, SLOT(setAutoexec(bool)));
    actionAutoexec->addTo(menu);
  }

  QPopupMenu *functionsMenu = new QPopupMenu(menu, "functionsMenu");
  functionsMenu->insertTearOffHandle();
  QStringList flist = scriptEnv->mathFunctions();
  for (int i=0; i<flist.size(); i++)
  {
    int id = functionsMenu->insertItem(flist[i], this, SLOT(insertFunction(int)));
    functionsMenu->setItemParameter(id, i);
  }
  menu->insertItem(tr("functions"),functionsMenu);
  
  return menu;
}

void ScriptEdit::insertErrorMsg(const QString &message)
{
  QString err = message;
  err.prepend("\n").replace("\n","\n#> ");
  removeSelection();
  insert(err,(uint)QTextEdit::CheckNewLines);
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
  myScript->exec();
}

void ScriptEdit::executeAll()
{
  QString fname = "<%1>";
  fname = fname.arg(name());
  myScript->setName(fname);
  myScript->setCode(text());
  myScript->exec();
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

