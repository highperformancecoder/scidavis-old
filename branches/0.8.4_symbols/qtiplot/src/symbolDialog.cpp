#include "symbolDialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qbuttongroup.h>
#include <qaccel.h>

symbolDialog::symbolDialog(CharSet charsSet, QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "symbolDialog" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setSizeGripEnabled( FALSE );

	GroupBox1 = new QButtonGroup(5, QGroupBox::Horizontal,tr(""), this,"GroupBox1" );
	GroupBox1->setFlat ( true );
	GroupBox1->setLineWidth ( 0 );
	GroupBox1->moveFocus (0);

	if (charsSet == symbolDialog::lowerGreek)
		initLowerGreekChars();
	else if (charsSet == symbolDialog::upperGreek)
		initUpperGreekChars();
	else if (charsSet == symbolDialog::mathSymbols)
		initMathSymbols();
	else
		initArrowSymbols();

	QHBoxLayout* hlayout = new QHBoxLayout(this, 0, 0, "hlayout2");
    hlayout->addWidget(GroupBox1);

    languageChange();

	connect (GroupBox1, SIGNAL(clicked(int)), this, SLOT(getChar(int)));

	QAccel *accel = new QAccel(this);
	accel->connectItem( accel->insertItem( Key_Return ),
                            this, SLOT(addCurrentChar()) );
}

void symbolDialog::initLowerGreekChars()
{
	int i, counter = 0;
	for ( i=0 ; i <= (0x3C9-0x3B1) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x3B1)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x3D1-0x3D1) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x3D1)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x3D5-0x3D5) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x3D5)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x3F1-0x3F0) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x3F0)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}

}

void symbolDialog::initUpperGreekChars()
{
	int i, counter = 0;
	for ( i=0 ; i <= (0x3A1-0x391) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x391)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x3A9-0x3A3) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x3A3)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
}


void symbolDialog::initMathSymbols()
{
	int i, counter = 0;
	for ( i=0 ; i <= (0x220D-0x2200) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x2200)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x2211-0x220F) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x220F)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x00B1-0x00B1) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x00B1)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x2213-0x2213) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x2213)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x221E - 0x2217) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x2217)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x2222-0x2222) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x2222)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x2230-0x2227) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x2227)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x223F-0x223F) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x223F)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x2245-0x2245) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x2245)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x2248-0x2248) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x2248)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x2255-0x2254) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x2254)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x2259-0x2259) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x2259)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x2267-0x225F) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x225F)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x226B-0x226A) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x226A)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x2289-0x2282) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x2282)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	// h bar
	for ( i=0 ; i <= (0x210F-0x210F) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x210F)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	// angstrom
	for ( i=0 ; i <= (0x212B-0x212B) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x212B)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
}

void symbolDialog::initArrowSymbols()
{
	int i, counter = 0;
	for ( i=0 ; i <= (0x219B-0x2190) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x2190)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x21A7-0x21A4) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x21A4)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x21D5-0x21CD) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x21CD)), GroupBox1, 0);
		btn->setMaximumWidth(30);
		btn->setFlat ( true );
		btn->setAutoDefault (false);
	}
	for ( i=0 ; i <= (0x21E9-0x21E6) ; i++,counter++ )
	{
		QPushButton *btn = new QPushButton(QString(QChar(i+0x21E6)), GroupBox1, 0);
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
