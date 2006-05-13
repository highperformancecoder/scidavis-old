#include "errDlg.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <q3groupbox.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <q3hbox.h>
#include <q3vbox.h>
#include <q3buttongroup.h>
//Added by qt3to4:
#include <Q3HBoxLayout>

errDialog::errDialog( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "errDialog" );

    setFocusPolicy( Qt::StrongFocus );
    setSizeGripEnabled( true );

	Q3VBox *vbox1=new Q3VBox (this,"vbox1");
	vbox1->setSpacing (5);
	
	Q3HBox *hbox1=new Q3HBox (vbox1,"hbox1");
    TextLabel1 = new QLabel(hbox1, "TextLabel1" );
    nameLabel = new QComboBox( FALSE,hbox1, "nameLabel" );

    GroupBox1 = new Q3ButtonGroup(2,Qt::Horizontal,tr(""), vbox1 , "GroupBox1" );
	GroupBox1->setRadioButtonExclusive ( TRUE );
	
	columnBox = new QRadioButton( GroupBox1, "columnBox" );
    columnBox->setChecked( TRUE );
	
	colNamesBox=new QComboBox( FALSE, GroupBox1, "colNamesBox" );
		
    percentBox = new QRadioButton( GroupBox1, "percentBox" );
    percentBox->setChecked( FALSE );

    valueBox = new QLineEdit( GroupBox1, "valueBox" );
    valueBox->setAlignment( Qt::AlignHCenter );
	valueBox->setEnabled(false);

	standardBox = new QRadioButton( GroupBox1, "standardBox" );

	GroupBox3 = new Q3ButtonGroup(2,Qt::Horizontal,tr(""), vbox1 , "GroupBox3" );
	GroupBox3->setRadioButtonExclusive ( TRUE );
	
    xErrBox = new QRadioButton(GroupBox3, "xErrBox" );

    yErrBox = new QRadioButton(GroupBox3, "yErrBox" );
    yErrBox->setChecked( TRUE );

    GroupBox2 = new Q3GroupBox(3, Qt::Vertical,tr(""), this , "GroupBox2" );
	// FIXME: replace next line with Qt4 equivalent
//X	GroupBox2->setLineWidth(0);
	GroupBox2->setFlat(TRUE);
	
	buttonAdd = new QPushButton(GroupBox2 , "buttonAdd" );
    buttonAdd->setDefault( TRUE );

    buttonCancel = new QPushButton( GroupBox2, "buttonCancel" );
	
	Q3HBoxLayout* hlayout1 = new Q3HBoxLayout(this,5,5, "hlayout1");
	hlayout1->addWidget(vbox1);
    hlayout1->addWidget(GroupBox2);
	
    languageChange();

  // signals and slots connections
	connect( buttonAdd, SIGNAL( clicked() ), this, SLOT( add() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( percentBox, SIGNAL( toggled(bool) ), valueBox, SLOT( setEnabled(bool) ) );
	connect( columnBox, SIGNAL( toggled(bool) ), colNamesBox, SLOT( setEnabled(bool) ) );
}

void errDialog::setCurveNames(const QStringList& names)
{
nameLabel->insertStringList (names,-1);
}

void errDialog::setExistingColumns(const QStringList& columns)
{
colNamesBox->insertStringList (columns,-1);	
}

void errDialog::add()
{
int direction=-1;
if (xErrBox->isChecked()) direction=0; 
else direction=1;
	
if (columnBox->isChecked())
	emit options(nameLabel->currentText(), colNamesBox->currentText(), direction);	
else
	{
	int type;
	if (percentBox->isChecked()) type=0; 
		else type=1;

	emit options(nameLabel->currentText(),type, valueBox->text(), direction);
	}
}

errDialog::~errDialog()
{
}

void errDialog::languageChange()
{
    setCaption( tr( "QtiPlot - Error Bars" ) );
    xErrBox->setText( tr( "&X Error Bars" ) );
	buttonAdd->setText( tr( "&Add" ) );
    TextLabel1->setText( tr( "Add Error Bars to" ) );
    GroupBox1->setTitle( tr( "Source of errors" ) );
    percentBox->setText( tr( "Percent of data (%)" ) );
    valueBox->setText( tr( "5" ) );
    standardBox->setText( tr( "Standard Deviation of Data" ) );
    yErrBox->setText( tr( "&Y Error Bars" ) );
    buttonCancel->setText( tr( "&Close" ) );
	columnBox->setText("Existing column");
}
