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
  connect(this, SIGNAL(returnPressed()), this, SLOT(updateIndentation()));
  
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
  actionExport = new QAction(NULL, "&Export", 0, this, "export");
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
  for (unsigned i=0; i<flist.size(); i++)
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

QString ScriptEdit::importASCII(const QString &filename)
{
QString filter = tr("Text") +" (*.txt *.TXT);;";
#ifdef SCRIPTING_PYTHON
	filter += tr("Python Source")+" (*.py);;";
#endif
filter += tr("All Files")+" (*)";
	
  QString f;
  if (filename.isEmpty())
    f = QFileDialog::getOpenFileName(QString::null,  filter, this, 0, tr("QtiPlot - Import Text From File"));
  else
    f = filename;
  if (f.isEmpty()) return QString::null;
  QFile file(f);
  if (!file.open(IO_ReadOnly))
  {
    QMessageBox::critical(this, tr("QtiPlot - Error Opening File"), tr("Could not open file \"%1\" for reading.").arg(f));
    return QString::null;
  }
  QTextStream s(&file);
  s.setEncoding(QTextStream::UnicodeUTF8);
  while (!s.atEnd())
    insert(s.readLine()+"\n");
  file.close();
  return f;
}

QString ScriptEdit::exportASCII()
{
QString filter = " *.txt;;";
#ifdef SCRIPTING_PYTHON
	filter += tr("Python Source")+" (*.py);;";
#endif
filter += tr("All Files")+" (*)";

QString selectedFilter;
QString fn = QFileDialog::getSaveFileName(parent()->name(), filter, this, 0,
			tr("Save Text to File"), &selectedFilter, false);
if ( !fn.isEmpty() )
	{
	QFileInfo fi(fn);
	QString baseName = fi.fileName();	
	if (!baseName.contains("."))
		{
		if (selectedFilter.contains(".txt"))
			fn.append(".txt");
		else if (selectedFilter.contains(".py"))
			fn.append(".py");
		}

	if ( QFile::exists(fn) &&
        QMessageBox::question(this, tr("QtiPlot -- Overwrite File? "),
            tr("A file called: <p><b>%1</b><p>already exists.\n"
                "Do you want to overwrite it?")
                .arg(fn), tr("&Yes"), tr("&No"),QString::null, 0, 1 ) )
		return QString::null;
	else
		{
		QFile f(fn);
		if ( !f.open( IO_WriteOnly ) )
			{
			QMessageBox::critical(0, tr("QtiPlot - File Save Error"),
			tr("Could not write to file: <br><h4> %1 </h4><p>Please verify that you have the right to write to this location!").arg(fn));
			return QString::null;
			}
		QTextStream t( &f );
		t.setEncoding(QTextStream::UnicodeUTF8);
		t << text();
		f.close();
		}
    }
return fn;
}

void ScriptEdit::updateIndentation()
{
  int para, index;
  getCursorPosition(&para, &index);
  if (para==0) return;
  QString prev = text(para-1);
  prev = prev.mid(0, prev.length()-1);
  int i;
  QString indent;
  for (i=0; prev[i].isSpace(); i++);
  indent = prev.mid(0, i);
  QString cur = text(para);
  cur = cur.mid(0, cur.length()-1);
  for (i=0; cur[i].isSpace(); i++);
  QString newtxt = indent + cur.mid(i, cur.length()-i);
  bool last = (para == paragraphs()-1);
  removeParagraph(para);
  insertParagraph(newtxt, para);
  if (last) removeParagraph(para+1);
  setCursorPosition(para, indent.length() + index);
}

