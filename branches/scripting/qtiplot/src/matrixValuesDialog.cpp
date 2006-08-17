#include "matrixValuesDialog.h"
#include "Scripting.h"
#include "scriptedit.h"
#include "matrix.h"

#include <qcombobox.h>
#include <qspinbox.h>
#include <qtextedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qvbox.h>
#include <qhbox.h>
#include <qbuttongroup.h>

matrixValuesDialog::matrixValuesDialog( ScriptingEnv *env, QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl ), scripted(env)
{
    if ( !name )
		setName( "matrixValuesDialog" );

    setCaption( tr( "QtiPlot - Set Matrix Values" ) );
    setFocusPolicy( QDialog::StrongFocus );
	
	QHBox *hbox1=new QHBox (this, "hbox1"); 
	hbox1->setSpacing (5);
	
	QVBox *box1=new QVBox (hbox1, "box2"); 
	box1->setSpacing (5);

	explain = new QTextEdit(box1, "explain" );
	explain->setReadOnly (true);
	
	QVBox *box2=new QVBox (hbox1, "box2"); 
	box2->setMargin(5);
	box2->setFrameStyle (QFrame::Box);

	QButtonGroup *GroupBox1 = new QButtonGroup(4,QGroupBox::Horizontal, "",box2, "GroupBox0" );
	GroupBox1->setLineWidth(0);
	GroupBox1->setFlat(true);
	
	QLabel *TextLabel1 = new QLabel(GroupBox1, "TextLabel1" );
    TextLabel1->setText( tr( "For row (i)" ) );
	
	startRow = new QSpinBox(1, 1000000, 1, GroupBox1, "startRow" );
	startRow->setValue(1);

    QLabel *TextLabel2 = new QLabel(GroupBox1, "TextLabel2" );
    TextLabel2->setText( tr( "to" ) );

    endRow =  new QSpinBox(1, 1000000, 1, GroupBox1, "endRow" );
	
	QLabel *TextLabel3 = new QLabel(GroupBox1, "TextLabel3" );
    TextLabel3->setText( tr( "For col (j)" ) );
	
	startCol = new QSpinBox(1, 1000000, 1, GroupBox1, "startCol" );
	startCol->setValue(1);

    QLabel *TextLabel4 = new QLabel(GroupBox1, "TextLabel2" );
    TextLabel4->setText( tr( "to" ) );

    endCol = new QSpinBox(1, 1000000, 1, GroupBox1, "endCol" );

	QHBox *hbox5=new QHBox (box2, "hbox5"); 
	hbox5->setSpacing (5);
	hbox5->setMargin(5);

    functions = new QComboBox( FALSE, hbox5, "functions" );
	
	PushButton3 = new QPushButton(hbox5, "PushButton3" );
    PushButton3->setText( tr( "Add function" ) ); 

	btnAddCell = new QPushButton(hbox5, "btnAddCell" );
    btnAddCell->setText( tr( "Add Cell" ) );

	QHBox *hbox4=new QHBox (this, "hbox4"); 
	hbox4->setSpacing (5);
	
	QLabel *TextLabel5 = new QLabel(hbox4, "TextLabel2" );
    TextLabel5->setText( tr( "Cell(i,j)=" ) );

	commandes = new ScriptEdit( scriptEnv, hbox4, "commandes" );
    commandes->setGeometry( QRect(10, 100, 260, 70) );
	commandes->setFocus();
	
	QVBox *box3=new QVBox (hbox4,"box3"); 
	box3->setSpacing (5);
	
	btnOk = new QPushButton(box3, "btnOk" );
    btnOk->setText( tr( "OK" ) );

	btnApply = new QPushButton(box3, "btnApply" );
    btnApply->setText( tr( "Apply" ) );

    btnCancel = new QPushButton( box3, "btnCancel" );
    btnCancel->setText( tr( "Cancel" ) );
	
	QVBoxLayout* layout = new QVBoxLayout(this,5,5, "hlayout3");
    layout->addWidget(hbox1);
	layout->addWidget(hbox5);
	layout->addWidget(hbox4);

setFunctions();
insertExplain(0);

connect(btnAddCell, SIGNAL(clicked()),this, SLOT(addCell()));
connect(PushButton3, SIGNAL(clicked()),this, SLOT(insertFunction()));
connect(btnOk, SIGNAL(clicked()),this, SLOT(accept()));
connect(btnApply, SIGNAL(clicked()),this, SLOT(apply()));
connect(btnCancel, SIGNAL(clicked()),this, SLOT(close()));
connect(functions, SIGNAL(activated(int)),this, SLOT(insertExplain(int)));
}

QSize matrixValuesDialog::sizeHint() const 
{
return QSize( 400, 190 );
}

void matrixValuesDialog::customEvent(QCustomEvent *e)
{
  if (e->type() == SCRIPTING_CHANGE_EVENT)
    scriptingChangeEvent((ScriptingChangeEvent*)e);
}

void matrixValuesDialog::accept()
{
if (apply())
	close();
}

bool matrixValuesDialog::apply()
{
  QString formula = commandes->text();
  QString oldFormula = matrix->formula();

  matrix->setFormula(formula);
  if (matrix->calculate(startRow->value()-1, endRow->value()-1, startCol->value()-1, endCol->value()-1))
    return true;
  matrix->setFormula(oldFormula);
  return false;
}

void matrixValuesDialog::setMatrix(Matrix* m)
{
  matrix = m;
  commandes->setText(m->formula());
  endCol->setValue(m->numCols());
  endRow->setValue(m->numRows());
  commandes->setContext(m);
}

void matrixValuesDialog::setFunctions()
{
functions->insertStringList(scriptEnv->mathFunctions(), -1);
}

void matrixValuesDialog::insertExplain(int index)
{
explain->setText(scriptEnv->mathFunctionDoc(functions->text(index)));
}

void matrixValuesDialog::insertFunction()
{
  commandes->insertFunction(functions->currentText());
}

void matrixValuesDialog::addCell()
{
commandes->insert("cell(i, j)");
}

matrixValuesDialog::~matrixValuesDialog()
{
}
