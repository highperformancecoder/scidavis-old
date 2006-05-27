#include "valuesDialog.h"
#include "worksheet.h"
#include "scriptedit.h"

#include <qcombobox.h>
#include <qspinbox.h>
#include <qtextedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qvbox.h>
#include <qhbox.h>
#include <qbuttongroup.h>

setColValuesDialog::setColValuesDialog( ScriptingEnv *env, QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
  scriptEnv = env;
    if ( !name )
	setName( "setColValuesDialog" );
    setCaption( tr( "QtiPlot - Set column values" ) );
    setFocusPolicy( QDialog::StrongFocus );
	
	QHBox *hbox1=new QHBox (this, "hbox1"); 
	hbox1->setSpacing (5);
	
	QVBox *box1=new QVBox (hbox1, "box2"); 
	box1->setSpacing (5);

	explain = new QTextEdit(box1, "explain" );
	explain->setReadOnly (true);
	explain->setPaletteBackgroundColor(QColor(197, 197, 197));
	
	colNameLabel = new QLabel(box1, "colNameLabel" );

	QVBox *box2=new QVBox (hbox1, "box2"); 
	box2->setMargin(5);
	box2->setFrameStyle (QFrame::Box);

	QHBox *hbox2=new QHBox (box2, "hbox2"); 
	hbox2->setMargin(5);
	hbox2->setSpacing (5);
	
	QLabel *TextLabel1 = new QLabel(hbox2, "TextLabel1" );
    TextLabel1->setText( tr( "For row (i)" ) );
	
	start = new QSpinBox(hbox2, "start" );
   
    QLabel *TextLabel2 = new QLabel(hbox2, "TextLabel2" );
    TextLabel2->setText( tr( "to" ) );

    end = new QSpinBox(hbox2, "end" );

    start->setMinValue(1);
    end->setMinValue(1);
    if (sizeof(int)==2)
	 { // 16 bit signed integer
	 start->setMaxValue(0x7fff);
	 end->setMaxValue(0x7fff);
	 }
    else
	 { // 32 bit signed integer
	 start->setMaxValue(0x7fffffff);
	 end->setMaxValue(0x7fffffff);
	 }
  
	QButtonGroup *GroupBox0 = new QButtonGroup(2,QGroupBox::Horizontal,tr( "" ),box2, "GroupBox0" );
	GroupBox0->setLineWidth(0);
	GroupBox0->setFlat(true);

    functions = new QComboBox( FALSE, GroupBox0, "functions" );
	
	PushButton3 = new QPushButton(GroupBox0, "PushButton3" );
    PushButton3->setText( tr( "Add function" ) );
    
    boxColumn = new QComboBox( FALSE, GroupBox0, "boxColumn" );
   
    PushButton4 = new QPushButton(GroupBox0, "PushButton4" );
    PushButton4->setText( tr( "Add column" ) );

	QHBox *hbox6=new QHBox (GroupBox0, "hbox6"); 
	hbox6->setSpacing (5);

	buttonPrev = new QPushButton( hbox6, "buttonPrev" );
	buttonPrev->setText("&<<");

	buttonNext = new QPushButton( hbox6, "buttonNext" );
	buttonNext->setText("&>>");

	addCellButton = new QPushButton(GroupBox0, "addCellButton" );
    addCellButton->setText( tr( "Add cell" ) );

	QHBox *hbox3=new QHBox (this, "hbox3"); 
	hbox3->setSpacing (5);
	
	commandes = new ScriptEdit( env, hbox3, "commandes" );
    commandes->setGeometry( QRect(10, 100, 260, 70) );
	commandes->setFocus();
	
	QVBox *box3=new QVBox (hbox3,"box3"); 
	box3->setSpacing (5);
	
    btnOk = new QPushButton(box3, "btnOk" );
    btnOk->setText( tr( "OK" ) );

	btnApply = new QPushButton(box3, "btnApply" );
    btnApply->setText( tr( "Apply" ) );

    btnCancel = new QPushButton( box3, "btnCancel" );
    btnCancel->setText( tr( "Cancel" ) );
	
	QVBoxLayout* layout = new QVBoxLayout(this,5,5, "hlayout3");
    layout->addWidget(hbox1);
	layout->addWidget(hbox3);

setFunctions();
insertExplain(0);

connect(PushButton3, SIGNAL(clicked()),this, SLOT(insertFunction()));
connect(PushButton4, SIGNAL(clicked()),this, SLOT(insertCol()));
connect(addCellButton, SIGNAL(clicked()),this, SLOT(insertCell()));
connect(btnOk, SIGNAL(clicked()),this, SLOT(accept()));
connect(btnApply, SIGNAL(clicked()),this, SLOT(apply()));
connect(btnCancel, SIGNAL(clicked()),this, SLOT(close()));
connect(functions, SIGNAL(activated(int)),this, SLOT(insertExplain(int)));
connect(buttonPrev, SIGNAL(clicked()), this, SLOT(prevColumn()));
connect(buttonNext, SIGNAL(clicked()), this, SLOT(nextColumn()));
}

void setColValuesDialog::prevColumn()
{
int sc = table->selectedColumn();
updateColumn(--sc);
}

void setColValuesDialog::nextColumn()
{
int sc = table->selectedColumn();
updateColumn(++sc);
}

void setColValuesDialog::updateColumn(int sc)
{
if (!sc)
	buttonPrev->setEnabled(false);
else
	buttonPrev->setEnabled(true);

if (sc >= table->tableCols() - 1)
	buttonNext->setEnabled(false);
else
	buttonNext->setEnabled(true);

table->setSelectedCol(sc);
colNameLabel->setText("col(\""+table->colLabel(sc)+"\")= ");

QStringList com = table->getCommandes();
commandes->setText(com[sc]);
commandes->moveCursor ( QTextEdit::MoveEnd, true );
}

QSize setColValuesDialog::sizeHint() const 
{
return QSize( 400, 190 );
}

void setColValuesDialog::accept()
{
if (apply())
	close();
}

bool setColValuesDialog::apply()
{
  int col = table->selectedColumn();
  QString formula = commandes->text();
  QString oldFormula = table->getCommandes()[col];

  table->setCommand(col,formula);
  if(table->calculate(col,start->value()-1,end->value()-1))
    return true;
  table->setCommand(col,oldFormula);
  return false;
}

void setColValuesDialog::setFunctions()
{
functions->insertStringList(scriptEnv->mathFunctions(), -1);
}

void setColValuesDialog::insertExplain(int index)
{
explain->setText(scriptEnv->mathFunctionDoc(functions->text(index)));
}

void setColValuesDialog::insertFunction()
{
  commandes->insertFunction(functions->currentText());
}

void setColValuesDialog::insertCol()
{
commandes->insert(boxColumn->currentText());
}

void setColValuesDialog::insertCell()
{
QString f=boxColumn->currentText().remove(")")+", i)";
commandes->insert(f);
}

void setColValuesDialog::setTable(Table* w)
{
table=w;
QStringList colNames=w->colNames();
int cols = w->tableCols();
for (int i=0; i<cols; i++)
	boxColumn->insertItem("col(\""+colNames[i]+"\")",i); 

start->setValue(1);
end->setValue(w->tableRows());
updateColumn(w->selectedColumn());
}

setColValuesDialog::~setColValuesDialog()
{
}
