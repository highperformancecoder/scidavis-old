#include "symbolDialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <q3buttongroup.h>
#include <q3accel.h>
//Added by qt3to4:
#include <Q3HBoxLayout>

symbolDialog::symbolDialog(CharSet charsSet, QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "symbolDialog" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setSizeGripEnabled( FALSE );

	GroupBox1 = new Q3ButtonGroup(5, Qt::Horizontal,tr(""), this,"GroupBox1" );
	GroupBox1->setFlat ( true );
// FIXME: replace this with Qt4 equivalent
//X	GroupBox1->setLineWidth ( 0 );
// FIXME: replace this with Qt4 equivalent
//X	GroupBox1->moveFocus (0);

	if (!charsSet)
		initMinGreekChars();
	else
		initMajGreekChars();

	Q3HBoxLayout* hlayout = new Q3HBoxLayout(this, 0, 0, "hlayout2");
    hlayout->addWidget(GroupBox1);

    languageChange();

	connect (GroupBox1, SIGNAL(clicked(int)), this, SLOT(getChar(int)));

	Q3Accel *accel = new Q3Accel(this);
	accel->connectItem( accel->insertItem( Qt::Key_Return ),
                            this, SLOT(addCurrentChar()) );
}

void symbolDialog::initMinGreekChars()
{
for (int i=0;i<25;i++)
	{
	QPushButton *btn = new QPushButton(QChar(i+0x3B1), GroupBox1, 0);
	btn->setMaximumWidth(30);
	btn->setFlat ( true );
	btn->setAutoDefault (false);
	}
}

void symbolDialog::initMajGreekChars()
{
new QPushButton(QChar(0x393), GroupBox1, 0);
new QPushButton(QChar(0x394), GroupBox1, 0);
new QPushButton(QChar(0x398), GroupBox1, 0);
new QPushButton(QChar(0x39B), GroupBox1, 0);
new QPushButton(QChar(0x39E), GroupBox1, 0);
new QPushButton(QChar(0x3A0), GroupBox1, 0);
new QPushButton(QChar(0x3A3), GroupBox1, 0);
new QPushButton(QChar(0x3A6), GroupBox1, 0);
new QPushButton(QChar(0x3A8), GroupBox1, 0);
new QPushButton(QChar(0x3A9), GroupBox1, 0);

for (int i=0;i<GroupBox1->count();i++)
	{
	QPushButton *btn = (QPushButton *) GroupBox1->find (i);
	btn->setMaximumWidth(30);
	btn->setFlat ( true );
	btn->setAutoDefault (false);
	}
}

void symbolDialog::addCurrentChar()
{
for (int i=0; i<GroupBox1->count(); i++)
	{
	QPushButton *btn = (QPushButton *) GroupBox1->find (i);
	if (btn && btn->hasFocus())
		{
		emit addLetter(btn->text());
		return;
		}
	}
}

void symbolDialog::getChar(int btnIndex)
{
QPushButton *btn = (QPushButton *) GroupBox1->find ( btnIndex );
emit addLetter(btn->text());
}


void symbolDialog::languageChange()
{
    setCaption( tr( "QtiPlot - Choose Symbol" ) );
}


symbolDialog::~symbolDialog()
{
}
