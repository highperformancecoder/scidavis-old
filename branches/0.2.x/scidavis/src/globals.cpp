/***************************************************************************
    File                 : globals.cpp
    Description          : Definition of global constants and enums
    --------------------------------------------------------------------
    Copyright            : (C) 2006-2008 Tilman Benkert (thzs*gmx.net)
    Copyright            : (C) 2006-2007 Ion Vasilief (ion_vasilief*yahoo.fr)
                           (replace * with @ in the email addresses) 

 ***************************************************************************/

#include "globals.h"
#include <QMessageBox>
#include <QIcon>
#include <QObject>
#include <QMetaObject>
#include <QMetaEnum>

//  Don't forget to change the Doxyfile when changing these!
const int SciDAVis::scidavis_version = 0x000200;

const char * SciDAVis::extra_version = "-beta1";

const char * SciDAVis::copyright_string = "\
=== Credits ===\n\
\n\
--- Developers ---\n\
\n\
The following people have written parts of the SciDAVis source code, ranging from a few lines to large chunks.\n\
In alphabetical order.\n\
\n\
Tilman Benkert[1], Knut Franke\n\
\n\
--- Documentation ---\n\
\n\
The following people have written parts of the manual and/or other documentation.\n\
In alphabetical order.\n\
\n\
Knut Franke, Roger Gadiou\n\
\n\
--- Translations ---\n\
\n\
The following people have contributed translations or parts thereof.\n\
In alphabetical order.\n\
\n\
Markus Bongard, Tobias Burnus, Rémy Claverie, f0ma, Jose Antonio Lorenzo Fernandez, Tilman Benkert[1],\n\
Daniel Klaer, Peter Landgren, Tomomasa Ohkubo, Mikhail Shevyakov, Mauricio Troviano\n\
\n\
--- Packagers ---\n\
\n\
The following people have made installing SciDAVis easier by providing specialized binary packages.\n\
In alphabetical order.\n\
\n\
Burkhard Bunk (Debian), Quentin Denis (SUSE), Eric Tanguy (Fedora),\n\
Mauricio Troviano (Windows installer), Yu-Hung Lien (Intel-Mac binary)\n\
\n\
--- QtiPlot ---\n\
\n\
SciDAVis uses code from QtiPlot, which consisted (at the time of the fork, i.e. QtiPlot 0.9-rc2) of code by the following people:\n\
\n\
Shen Chen, Borries Demeler, José Antonio Lorenzo Fernández, Knut Franke, Vasileios Gkanis, Gudjon Gudjonsson, Tilman Benkert[1], \n\
Alex Kargovsky, Michael Mac-Vicar, Tomomasa Ohkubo, Aaron Van Tassle, Branimir Vasilic, Ion Vasilief, Vincent Wagelaar\n\
\n\
The SciDAVis manual is based on the QtiPlot manual, written by (in alphabetical order):\n\
\n\
Knut Franke, Roger Gadiou, Ion Vasilief\n\
\n\
footnotes:\n\
[1] birth name: Tilman Hoener zu Siederdissen\n\
\n\
=== Special Thanks ===\n\
\n\
We also want to acknowledge the people having helped us indirectly by contributing to the following\n\
fine pieces of software. In no particular order.\n\
\n\
Qt (http://doc.trolltech.com/4.3/credits.html),\n\
Qwt (http://qwt.sourceforge.net/#credits),\n\
Qwtplot3D (http://qwtplot3d.sourceforge.net/),\n\
muParser (http://muparser.sourceforge.net/),\n\
Python (http://www.python.org/),\n\
liborigin (http://sourceforge.net/projects/liborigin/),\n\
Vim (http://www.vim.org/thanks.php/),\n\\n\
webgen (http://webgen.rubyforge.org/),\n\
Doxygen (http://www.doxygen.org/),\n\
Subversion (http://subversion.tigris.org/),\n\
GSL (http://www.gnu.org/software/gsl/)\n\
\n\
... and many more we just forgot to mention.\n";

const char * SciDAVis::release_date = " XXXX-XX-XX";

int SciDAVis::version()
{
	return scidavis_version;
}

QString SciDAVis::versionString()
{
	return "SciDAVis " + 
			QString::number((scidavis_version & 0xFF0000) >> 16)+"."+ 
			QString::number((scidavis_version & 0x00FF00) >> 8)+"."+
			QString::number(scidavis_version & 0x0000FF);
}
			
QString SciDAVis::extraVersion()
{
	return	QString(extra_version);
}


void SciDAVis::about()
{
	QString text = "<h2>"+ versionString() + extraVersion() + "</h2>";
	text += "<h3>" + QObject::tr("Released") + ": " + QString(SciDAVis::release_date) + "</h3>";
	text +=	"<h3>" + QString(SciDAVis::copyright_string).replace("\n", "<br>") + "</h3>";

	QMessageBox *mb = new QMessageBox();
	mb->setAttribute(Qt::WA_DeleteOnClose);
	mb->setWindowTitle(QObject::tr("About SciDAVis"));
	mb->setWindowIcon(QIcon(":/appicon"));
	mb->setIconPixmap(QPixmap(":/appicon"));
	mb->setText(text);
	mb->exec();
}

QString SciDAVis::copyrightString()
{
	return copyright_string;
}

QString SciDAVis::releaseDateString()
{
	return release_date;
}

QString SciDAVis::enumValueToString(int key, const QString& enum_name)
{
	int index = staticMetaObject.indexOfEnumerator(enum_name.toAscii());
	if(index == -1) return QString("invalid");
	QMetaEnum meta_enum = staticMetaObject.enumerator(index);
	return QString(meta_enum.valueToKey(key));
}

int SciDAVis::enumStringToValue(const QString& string, const QString& enum_name)
{
	int index = staticMetaObject.indexOfEnumerator(enum_name.toAscii());
	if(index == -1) return -1;
	QMetaEnum meta_enum = staticMetaObject.enumerator(index);
	return meta_enum.keyToValue(string.toAscii());
}

