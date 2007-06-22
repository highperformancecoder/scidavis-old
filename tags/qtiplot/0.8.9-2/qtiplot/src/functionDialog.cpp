#include "functionDialog.h"
#include "parser.h"
#include "application.h"
#include "FunctionCurve.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qbuttongroup.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qwidgetstack.h>
#include <qhbox.h>
#include <qtextedit.h>

FunctionDialog::FunctionDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
	if ( !name )
		setName( "FunctionDialog" );

	QHBox *hbox1=new QHBox(this, "hbox1");
	hbox1->setSpacing(5);

	textFunction_2 = new QLabel( hbox1, "textFunction_2" );
	boxType = new QComboBox( FALSE, hbox1, "boxType" );

	optionStack = new QWidgetStack( this, "optionStack" );
	optionStack->setFrameShape( QWidgetStack::GroupBoxPanel );
	optionStack->setFrameShadow( QWidgetStack::Plain );

	functionPage = new QWidget( optionStack, "functionPage" );

	GroupBox1 = new QButtonGroup( 2,QGroupBox::Horizontal, QString::null,functionPage,"GroupBox1" );
	GroupBox1->setLineWidth(0);
	GroupBox1->setFlat(TRUE);

	textFunction = new QLabel( GroupBox1);
	boxFunction = new QTextEdit(GroupBox1);
	boxFunction->setMinimumHeight(50);
	boxFunction->setMinimumWidth(350);

	textFrom = new QLabel( GroupBox1);
	boxFrom = new QLineEdit( GroupBox1 );
	boxFrom->setText("0");

	textTo = new QLabel( GroupBox1);
	boxTo = new QLineEdit( GroupBox1);
	boxTo->setText("1");

	textPoints = new QLabel( GroupBox1, "textPoints" );
	boxPoints = new QSpinBox(2,1000000,100, GroupBox1, "boxPoints" );
	boxPoints->setValue(100);

	QHBoxLayout* hlayout = new QHBoxLayout(functionPage,5,5, "hlayout");
	hlayout->addWidget(GroupBox1);

	optionStack->addWidget( functionPage, 0 );

	parametricPage = new QWidget( optionStack, "parametricPage" );

	GroupBox2 = new QButtonGroup( 2,QGroupBox::Horizontal,tr(""), parametricPage,"GroupBox2" );
	GroupBox2->setLineWidth(0);
	GroupBox2->setFlat(TRUE);

	textParameter = new QLabel( GroupBox2, "textParameter" );
	boxParameter = new QLineEdit( GroupBox2, "boxParameter" );
	boxParameter->setText("m");

	textParameterFrom = new QLabel( GroupBox2, "textParameterFrom" );
	boxParFrom = new QLineEdit( GroupBox2, "boxParFrom" );
	boxParFrom->setText("0");

	textParameterTo = new QLabel( GroupBox2, "textParameterTo" );
	boxParTo = new QLineEdit( GroupBox2, "boxParTo" );
	boxParTo->setText("1");

	textXPar = new QLabel( GroupBox2, "textXPar" );
	boxXFunction = new QComboBox(TRUE, GroupBox2, "boxXFunction" );

	textYPar = new QLabel( GroupBox2, "textYPar" );
	boxYFunction = new QComboBox(TRUE, GroupBox2, "boxYFunction" );

	textParPoints = new QLabel( GroupBox2, "textParPoints" );
	boxParPoints = new QSpinBox(2,1000000,100, GroupBox2, "boxParPoints" );
	boxParPoints->setValue(100);

	QHBoxLayout* hlayout2 = new QHBoxLayout(parametricPage, 5, 5, "hlayout");
	hlayout2->addWidget(GroupBox2);

	optionStack->addWidget( parametricPage, 1 );

	polarPage = new QWidget( optionStack, "polarPage" );

	GroupBox3 = new QButtonGroup( 2,QGroupBox::Horizontal, QString::null, polarPage,"GroupBox3" );
	GroupBox3->setLineWidth(0);
	GroupBox3->setFlat(TRUE);

	textPolarParameter = new QLabel( GroupBox3, "textPolarParameter" );
	boxPolarParameter = new QLineEdit( GroupBox3, "boxPolarParameter" );
	boxPolarParameter->setText ("t");

	textPolarFrom = new QLabel( GroupBox3, "textPolarFrom" );
	boxPolarFrom = new QLineEdit( GroupBox3, "boxPolarFrom" );
	boxPolarFrom->setText("0");

	textPolarTo = new QLabel( GroupBox3, "textPolarTo" );
	boxPolarTo = new QLineEdit( GroupBox3, "boxPolarTo" );
	boxPolarTo->setText("pi");

	textPolarRadius = new QLabel( GroupBox3, "textPolarRadius" );
	boxPolarRadius = new QComboBox(TRUE, GroupBox3, "boxPolarRadius" );

	textPolarTheta = new QLabel( GroupBox3, "textPolarTheta" );
	boxPolarTheta = new QComboBox(TRUE, GroupBox3, "boxPolarTheta" );

	textPolarPoints = new QLabel( GroupBox3, "textPolarPoints" );
	boxPolarPoints = new QSpinBox(2,1000000,100,GroupBox3, "boxPolarPoints" );
	boxPolarPoints->setValue(100);

	QHBoxLayout* hlayout3 = new QHBoxLayout(polarPage, 5, 5, "hlayout");
	hlayout3->addWidget(GroupBox3);

	optionStack->addWidget( polarPage, 2 );

	GroupBox4 = new QButtonGroup( 3, QGroupBox::Horizontal,tr(""), this, "buttonGroup3" );
	GroupBox4->setLineWidth(0);
	GroupBox4->setFlat(TRUE);

	buttonClear = new QPushButton( GroupBox4, "buttonClear" );
	buttonOk = new QPushButton( GroupBox4, "buttonOk" );
	buttonOk->setDefault(TRUE);

	buttonCancel = new QPushButton( GroupBox4, "buttonCancel" );

	QVBoxLayout* hlayout1 = new QVBoxLayout(this, 5, 5, "hlayout1");
	hlayout1->addWidget(hbox1);
	hlayout1->addWidget(optionStack);
	hlayout1->addWidget(GroupBox4);

	languageChange();
	connect( boxType, SIGNAL( activated(int) ), this, SLOT( raiseWidget(int) ) );
	connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( buttonClear, SIGNAL( clicked() ), this, SLOT(clearList() ) );
	setFocusProxy(boxFunction);

	curveID = -1;
	graph = 0;
}

void FunctionDialog::raiseWidget(int index)
{
	if (index)
		buttonClear->setText( tr( "Clear list" ) );
	else
		buttonClear->setText( tr( "Clear Function" ) );

	optionStack->raiseWidget(index);
}

void FunctionDialog::languageChange()
{
	setCaption( tr( "QtiPlot - Add function curve" ) );
	textFunction_2->setText( tr( "Curve type " ) );
	textFunction->setText( tr( "f(x)= " ) );
	textFrom->setText( tr( "From x= " ) );
	textTo->setText( tr( "To x= " ) );
	textPoints->setText( tr( "Points" ) );
	buttonClear->setText( tr( "Clear Function" ) );
	textParameter->setText( tr( "Parameter" ) );
	textParPoints->setText( tr( "Points" ) );
	textParameterTo->setText( tr( "To" ) );
	textYPar->setText( tr( "y = " ) );
	textXPar->setText( tr( "x = " ) );
	textParameterFrom->setText( tr( "From" ) );
	textPolarPoints->setText( tr( "Points" ) );
	textPolarParameter->setText( tr( "Parameter" ) );
	textPolarFrom->setText( tr( "From" ) );
	textPolarTo->setText( tr( "To" ) );
	textPolarRadius->setText( tr( "R =" ) );
	textPolarTheta->setText( tr( "Theta =" ) );
	boxType->insertItem( tr( "Function" ) );
	boxType->insertItem( tr( "Parametric plot" ) );
	boxType->insertItem( tr( "Polar plot" ) );
	buttonCancel->setText( tr( "Close" ) );
	buttonOk->setText( tr( "Ok" ) );
}

void FunctionDialog::setCurveToModify(Graph *g, int curve)
{
	if (!g)
		return;

	graph = g;

	FunctionCurve *c = (FunctionCurve *)graph->curve(curve);
	if (!c)
		return;

	curveID = curve;

	QStringList formulas = c->formulas();
	if (c->functionType() == FunctionCurve::Normal)
	{
		boxFunction->setText(formulas[0]);
		boxFrom->setText(QString::number(c->startRange(), 'g', 15));
		boxTo->setText(QString::number(c->endRange(), 'g', 15));
		boxPoints->setValue(c->dataSize());
	}
	else if (c->functionType() == FunctionCurve::Polar)
	{
		optionStack->raiseWidget(2);
		boxType->setCurrentItem(2);

		boxPolarRadius->setCurrentText(formulas[0]);
		boxPolarTheta->setCurrentText(formulas[1]);
		boxPolarParameter->setText(c->variable());
		boxPolarFrom->setText(QString::number(c->startRange(), 'g', 15));
		boxPolarTo->setText(QString::number(c->endRange(), 'g', 15));
		boxPolarPoints->setValue(c->dataSize());
	}
	else if (c->functionType() == FunctionCurve::Parametric)
	{
		boxType->setCurrentItem(1);
		optionStack->raiseWidget(1);

		boxXFunction->setCurrentText(formulas[0]);
		boxYFunction->setCurrentText(formulas[1]);
		boxParameter->setText(c->variable());
		boxParFrom->setText(QString::number(c->startRange(), 'g', 15));
		boxParTo->setText(QString::number(c->endRange(), 'g', 15));
		boxParPoints->setValue(c->dataSize());
	}
}

void FunctionDialog::clearList()
{
	int type=boxType->currentItem();
	switch (type)
	{
		case 0:
			boxFunction->clear();
			break;

		case 1:
			boxXFunction->clear();
			boxYFunction->clear();
			emit clearParamFunctionsList();
			break;

		case 2:
			boxPolarTheta->clear();
			boxPolarRadius->clear();
			emit clearPolarFunctionsList();
			break;
	}
}

void FunctionDialog::acceptFunction()
{
	QString from=boxFrom->text().lower();
	QString to=boxTo->text().lower();
	QString points=boxPoints->text().lower();

	double start, end;
	try
	{
		myParser parser;
		parser.SetExpr(from.ascii());
		start=parser.Eval();
	}
	catch(mu::ParserError &e)
	{
		QMessageBox::critical(0, tr("QtiPlot - Start limit error"), e.GetMsg());
		boxFrom->setFocus();
		return;
	}	
	try
	{
		myParser parser;
		parser.SetExpr(to.ascii());
		end=parser.Eval();
	}
	catch(mu::ParserError &e)
	{
		QMessageBox::critical(0, tr("QtiPlot - End limit error"), e.GetMsg());
		boxTo->setFocus();
		return;
	}

	if (start>=end)
	{
		QMessageBox::critical(0, tr("QtiPlot - Input error"),
				tr("Please enter x limits that satisfy: from < end!"));
		boxTo->setFocus();
		return;
	}

	double x;
	QString formula=boxFunction->text().remove("\n");
	bool error=FALSE;

	try
	{
		myParser parser;	
		parser.DefineVar("x", &x);	
		parser.SetExpr(formula.ascii());

		x=start;
		parser.Eval();
		x=end;
		parser.Eval();
	}
	catch(mu::ParserError &e)
	{
		QMessageBox::critical(0, tr("QtiPlot - Input function error"), e.GetMsg());
		boxFunction->setFocus();
		error=TRUE;	
	}	

	// Collecting all the information
	int type = boxType->currentItem();
	QStringList formulas;
	QValueList<double> ranges;
	formulas+=formula;
	ranges+=start;
	ranges+=end;
	if (!error)
	{
		if (!graph)
			emit newFunctionPlot(type, formulas, "x", ranges, boxPoints->value());
		else
		{
			if (curveID >= 0)
				graph->modifyFunctionCurve(curveID, type, formulas, "x", ranges, boxPoints->value());
			else
				graph->addFunctionCurve(type,formulas, "x", ranges, boxPoints->value());

			ApplicationWindow *app = (ApplicationWindow *)this->parent();
			app->updateFunctionLists(type,formulas);
		}
	}		

}
void FunctionDialog::acceptParametric()
{
	QString from=boxParFrom->text().lower();
	QString to=boxParTo->text().lower();
	QString points=boxParPoints->text().lower();

	double start, end;
	try
	{
		myParser parser;
		parser.SetExpr(from.ascii());
		start=parser.Eval();
	}
	catch(mu::ParserError &e)
	{
		QMessageBox::critical(0, tr("QtiPlot - Start limit error"), e.GetMsg());
		boxParFrom->setFocus();
		return;
	}	

	try
	{
		myParser parser;
		parser.SetExpr(to.ascii());
		end=parser.Eval();
	}
	catch(mu::ParserError &e)
	{
		QMessageBox::critical(0, tr("QtiPlot - End limit error"), e.GetMsg());
		boxParTo->setFocus();
		return;
	}	

	if (start>=end)
	{
		QMessageBox::critical(0, tr("QtiPlot - Input error"),
				tr("Please enter parameter limits that satisfy: from < end!"));
		boxParTo->setFocus();
		return;
	}

	double parameter;
	QString xformula=boxXFunction->currentText();
	QString yformula=boxYFunction->currentText();	
	bool error=FALSE;

	try
	{
		myParser parser;				
		parser.DefineVar((boxParameter->text()).ascii(), &parameter);	
		parser.SetExpr(xformula.ascii());

		parameter=start;
		parser.Eval();
		parameter=end;
		parser.Eval();
	}
	catch(mu::ParserError &e)
	{
		QMessageBox::critical(0, tr("QtiPlot - Input function error"), e.GetMsg());
		boxXFunction->setFocus();
		error=TRUE;	
	}	
	try
	{
		myParser parser;				
		parser.DefineVar((boxParameter->text()).ascii(), &parameter);	
		parser.SetExpr(yformula.ascii());

		parameter=start;
		parser.Eval();
		parameter=end;
		parser.Eval();
	}
	catch(mu::ParserError &e)
	{
		QMessageBox::critical(0, tr("QtiPlot - Input function error"), e.GetMsg());
		boxYFunction->setFocus();
		error=TRUE;	
	}
	// Collecting all the information
	int type = boxType->currentItem();
	QStringList formulas;
	QValueList<double> ranges;
	formulas+=xformula;
	formulas+=yformula;
	ranges+=start;
	ranges+=end;
	if (!error)
	{
		if (!graph)
			emit newFunctionPlot(type, formulas, boxParameter->text(),ranges, boxParPoints->value());
		else
		{
			if (curveID >= 0)
				graph->modifyFunctionCurve(curveID, type, formulas, boxParameter->text(),ranges, boxParPoints->value());
			else
				graph->addFunctionCurve(type,formulas, boxParameter->text(),ranges, boxParPoints->value());

			ApplicationWindow *app = (ApplicationWindow *)this->parent();
			app->updateFunctionLists(type,formulas);
		}
	}
}

void FunctionDialog::acceptPolar()
{
	QString from=boxPolarFrom->text().lower();
	QString to=boxPolarTo->text().lower();
	QString points=boxPolarPoints->text().lower();

	double start, end;
	try
	{
		myParser parser;
		parser.SetExpr(from.ascii());
		start=parser.Eval();
	}
	catch(mu::ParserError &e)
	{
		QMessageBox::critical(0, tr("QtiPlot - Start limit error"), e.GetMsg());
		boxPolarFrom->setFocus();
		return;
	}

	try
	{
		myParser parser;	
		parser.SetExpr(to.ascii());
		end=parser.Eval();
	}
	catch(mu::ParserError &e)
	{
		QMessageBox::critical(0, tr("QtiPlot - End limit error"), e.GetMsg());
		boxPolarTo->setFocus();
		return;
	}

	if (start>=end)
	{
		QMessageBox::critical(0, tr("QtiPlot - Input error"),
				tr("Please enter parameter limits that satisfy: from < end!"));
		boxPolarTo->setFocus();
		return;
	}

	double parameter;
	QString rformula=boxPolarRadius->currentText();
	QString tformula=boxPolarTheta->currentText();	
	bool error=FALSE;

	try
	{
		myParser parser;				
		parser.DefineVar((boxPolarParameter->text()).ascii(), &parameter);	
		parser.SetExpr(rformula.ascii());

		parameter=start;
		parser.Eval();
		parameter=end;
		parser.Eval();
	}
	catch(mu::ParserError &e)
	{
		QMessageBox::critical(0, tr("QtiPlot - Input function error"), e.GetMsg());
		boxPolarRadius->setFocus();
		error=TRUE;	
	}	
	try
	{
		myParser parser;				
		parser.DefineVar((boxPolarParameter->text()).ascii(), &parameter);	
		parser.SetExpr(tformula.ascii());

		parameter=start;
		parser.Eval();
		parameter=end;
		parser.Eval();
	}
	catch(mu::ParserError &e)
	{
		QMessageBox::critical(0, tr("QtiPlot - Input function error"), e.GetMsg());
		boxPolarTheta->setFocus();
		error=TRUE;	
	}
	// Collecting all the information
	int type = boxType->currentItem();
	QStringList formulas;
	QValueList<double> ranges;
	formulas+=rformula;
	formulas+=tformula;
	ranges+=start;
	ranges+=end;
	if (!error)
	{
		ApplicationWindow *app = (ApplicationWindow *)this->parent();
		app->updateFunctionLists(type,formulas);

		if (!graph)
			emit newFunctionPlot(type, formulas, boxParameter->text(),ranges, boxPolarPoints->value());
		else
		{
			if (curveID >= 0)
				graph->modifyFunctionCurve(curveID, type, formulas, boxPolarParameter->text(),ranges, boxPolarPoints->value());
			else
				graph->addFunctionCurve(type, formulas, boxPolarParameter->text(),ranges, boxPolarPoints->value());
		}
	}
}

void FunctionDialog::accept()
{
	int type=boxType->currentItem();
	switch (type)
	{
		case 0:
			acceptFunction();
			break;

		case 1:
			acceptParametric();
			break;

		case 2:
			acceptPolar();
			break;
	}
}

void FunctionDialog::insertParamFunctionsList(const QStringList& xList, const QStringList& yList)
{
	boxXFunction->insertStringList (xList, 1);
	boxYFunction->insertStringList (yList, 1);
}

void FunctionDialog::insertPolarFunctionsList(const QStringList& rList, const QStringList& tetaList)
{
	boxPolarRadius->insertStringList (rList, 1);
	boxPolarTheta->insertStringList (tetaList, 1);	
}

FunctionDialog::~FunctionDialog()
{
}
