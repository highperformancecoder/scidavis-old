#include "sortDialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <q3buttongroup.h>
//Added by qt3to4:
#include <Q3VBoxLayout>

sortDialog::sortDialog( QWidget* parent, Qt::WFlags fl )
    : QDialog( parent, fl )
{
	setCaption(tr("QtiPlot - Sorting Options"));
    setMinimumSize( QSize( 310, 140 ) );
	setMaximumSize( QSize( 310, 140 ) );
    setMouseTracking( TRUE );
    setSizeGripEnabled( FALSE );
	
	GroupBox1 = new Q3ButtonGroup( 2,Qt::Horizontal, QString::null,this,"GroupBox1" );

	new QLabel( tr("Sort columns"), GroupBox1, "TextLabel1",0 );
	boxType = new QComboBox(GroupBox1, "boxShow" );
	
	new QLabel( tr("Order"), GroupBox1, "TextLabel2",0 );
	boxOrder = new QComboBox(GroupBox1, "boxOrder" );
	
	new QLabel(tr("Leading column"),GroupBox1, "TextLabel3",0);
	columnsList = new QComboBox(GroupBox1, "listBox" );
	columnsList->setEnabled(FALSE);	
	
	GroupBox2 = new Q3HButtonGroup(this,"GroupBox2" );
	GroupBox2->setFlat (TRUE);
	// TODO: Replace this when porting the dialog to Qt4
	//X GroupBox2->setLineWidth (0);
	
	buttonOk = new QPushButton(GroupBox2, "buttonOk" );
    buttonOk->setDefault( TRUE );
   
    buttonCancel = new QPushButton(GroupBox2, "buttonCancel" );    
    
	Q3VBoxLayout* vlayout = new Q3VBoxLayout(this,5,5, "vlayout");
    vlayout->addWidget(GroupBox1);
	vlayout->addWidget(GroupBox2);

    languageChange();
   
    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( boxType, SIGNAL( activated(int) ), this, SLOT(changeType(int)));
}

sortDialog::~sortDialog()
{
}


void sortDialog::languageChange()
{
    buttonOk->setText( tr("&OK") );
	buttonCancel->setText( tr("&Cancel") );
	
	boxType->insertItem(tr("Separately"));
	boxType->insertItem(tr("Together"));

	boxOrder->insertItem(tr("Ascending"));
	boxOrder->insertItem(tr("Descending"));
}

void sortDialog::accept()
{
emit sort(boxType->currentItem(),boxOrder->currentItem(),columnsList->currentText());
close();
}

void sortDialog::insertColumnsList(const QStringList& cols)
{
int i,n=cols.count();
for (i=0;i<n;i++)
	columnsList->insertItem(cols[i],i);

columnsList->setCurrentItem(0);
}

void sortDialog::changeType(int Type)
{
boxType->setCurrentItem(Type);
if(Type==1)
	columnsList->setEnabled(TRUE);
else
	columnsList->setEnabled(FALSE);	
}
