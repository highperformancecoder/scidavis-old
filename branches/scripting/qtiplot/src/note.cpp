#include "note.h"
#include "Scripting.h"
#include "scriptedit.h"

#include <qdatetime.h>
#include <qlayout.h>
#include <qapplication.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qsimplerichtext.h>

#include <math.h>

Note::Note(ScriptingEnv *env, const QString& label, QWidget* parent, const char* name, WFlags f)
				: myWidget(label, parent, name, f)
{
init(env);	
}

void Note::init(ScriptingEnv *env)
{
autoExec = false;
QDateTime dt = QDateTime::currentDateTime ();
setBirthDate(dt.toString(Qt::LocalDate));

te = new ScriptEdit(env, this, name());
QVBoxLayout* hlayout = new QVBoxLayout(this,0,0, "hlayout1");
hlayout->addWidget(te);

setGeometry(0, 0, 500, 200);
connect(te, SIGNAL(textChanged()), this, SLOT(modifiedNote()));
}

void Note::modifiedNote()
{
emit modifiedWindow(this);
}

QString Note::saveToString(const QString &info)
{
QString s= "<note>\n";
s+= QString(name()) + "\t" + birthDate() + "\n";
s+= info;
s+= "WindowLabel\t" + windowLabel() + "\t" + QString::number(captionPolicy()) + "\n";
s+= "AutoExec\t" + QString(autoExec ? "1" : "0") + "\n";
s+= "<content>\n"+te->text().stripWhiteSpace()+"\n</content>";
s+="\n</note>\n";
return s;
}

void Note::restore(const QStringList& data)
{
  QStringList::ConstIterator line = data.begin();
  QStringList fields;

  fields = QStringList::split("\t", *line, true);
  if (fields[0] == "AutoExec")
  {
    setAutoexec(fields[1] == "1");
    line++;
  }

  if (*line == "<content>") line++;
  while (line != data.end() && *line != "</content>")
    te->append((*line++)+"\n");
}

void Note::setAutoexec(bool exec)
{
  autoExec = exec;
  if (autoExec)
    te->setPaletteBackgroundColor(QColor(255,239,185));
  else
    te->unsetPalette();
}

void Note::execute()
{
  te->executeAll();
}

