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

fDialog::fDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : functionDialogui( parent, name, modal, fl )
{
    if ( !name )
	setName( "fDialog" );

	connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( buttonClear, SIGNAL( clicked() ), this, SLOT(clearList() ) );
	setFocusProxy(boxFunction);

	curveID = -1;
	graph = 0;
}

void fDialog::setCurveToModify(Graph *g, int curve)
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

void fDialog::clearList()
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

void fDialog::acceptFunction()
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
void fDialog::acceptParametric()
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

void fDialog::acceptPolar()
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

void fDialog::accept()
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

void fDialog::insertParamFunctionsList(const QStringList& xList, const QStringList& yList)
{
boxXFunction->insertStringList (xList, 1);
boxYFunction->insertStringList (yList, 1);
}

void fDialog::insertPolarFunctionsList(const QStringList& rList, const QStringList& tetaList)
{
boxPolarRadius->insertStringList (rList, 1);
boxPolarTheta->insertStringList (tetaList, 1);	
}

fDialog::~fDialog()
{
}
