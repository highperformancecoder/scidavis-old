#include "lineDlg.h"
#include "colorButton.h"
#include "LineMarker.h"
#include "graph.h"
#include "application.h"

#include <qwt_plot.h>

#include <qspinbox.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qbuttongroup.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qcolordialog.h>
#include <qtabwidget.h>
#include <qhbox.h>
#include <qlineedit.h>

lineDialog::lineDialog(LineMarker *line, QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl ),
	  lm(line)
{
    setCaption( tr( "QtiPlot - Line options" ) );

	tw = new QTabWidget( this);
	options = new QWidget( tw);
    GroupBox1 = new QButtonGroup( 2,QGroupBox::Horizontal, QString::null,options,"GroupBox1" );

	new QLabel(tr( "Color" ), GroupBox1);
	colorBox = new ColorButton(GroupBox1);
	colorBox->setColor(lm->color());

	new QLabel(tr( "Line type" ),GroupBox1);
    styleBox = new QComboBox( FALSE, GroupBox1);
	styleBox->insertItem("_____");
	styleBox->insertItem("- - -");
	styleBox->insertItem(".....");
	styleBox->insertItem("_._._");
	styleBox->insertItem("_.._..");
	
	setLineStyle(lm->style());

	new QLabel(tr( "Line width" ),GroupBox1);
    widthBox = new QComboBox( FALSE, GroupBox1, "widthBox" );
	widthBox->insertItem( tr( "1" ) );
    widthBox->insertItem( tr( "2" ) );
    widthBox->insertItem( tr( "3" ) );
    widthBox->insertItem( tr( "4" ) );
    widthBox->insertItem( tr( "5" ) );
	widthBox->setEditable (TRUE);
	widthBox->setCurrentItem(0);
	widthBox->setEditText(QString::number(lm->width()));

	startBox = new QCheckBox(GroupBox1); 
    startBox->setText( tr( "Arrow at &start" ) );
	startBox->setChecked(lm->hasStartArrow());

	endBox = new QCheckBox(GroupBox1, "endBox" );
    endBox->setText( tr( "Arrow at &end" ) );
	endBox->setChecked(lm->hasEndArrow());

	QHBoxLayout* hl1 = new QHBoxLayout(options,5,5, "hl1");
    hl1->addWidget(GroupBox1);
	
	tw->insertTab(options, tr( "Opti&ons" ) );
	
	head = new QWidget( tw, "head" );
    QButtonGroup *GroupBox4 = new QButtonGroup( 2,QGroupBox::Horizontal,tr(""),head,"GroupBox4" );

	new QLabel(tr( "Length" ), GroupBox4, "TextLabel111",0);
	boxHeadLength = new QSpinBox( 0,100,1,GroupBox4, "boxHeadLength" );
	boxHeadLength->setValue(lm->headLength());

	new QLabel(tr( "Angle" ),GroupBox4, "TextLabel1112",0 );
	boxHeadAngle = new QSpinBox(0,85,5,GroupBox4, "boxHeadAngle" );
	boxHeadAngle->setValue(lm->headAngle());

	filledBox = new QCheckBox(GroupBox4, "filledBox" ); 
    filledBox->setText( tr( "&Filled" ) );
	filledBox->setChecked(lm->filledArrowHead());

	QHBoxLayout* hl3 = new QHBoxLayout(head,5,5, "hl3");
    hl3->addWidget(GroupBox4);

	tw->insertTab(head, tr( "Arrow &Head" ) );

	initGeometryTab();

	GroupBox2 = new QButtonGroup(4,QGroupBox::Horizontal, QString::null,this,"GroupBox2" );
	GroupBox2->setLineWidth (0);
	
	buttonDefault = new QPushButton( GroupBox2);
	buttonDefault->setText( tr( "Set &Default" ) );
	
	btnApply = new QPushButton(GroupBox2, "btnApply" );
    btnApply->setText( tr( "&Apply" ) );

	btnOk = new QPushButton(GroupBox2, "btnOk" );
    btnOk->setText( tr( "&Ok" ) );
	
	QVBoxLayout* hlayout = new QVBoxLayout(this,5,5, "hlayout");
    hlayout->addWidget(tw);
	hlayout->addWidget(GroupBox2);

	enableHeadTab();

	connect( colorBox, SIGNAL( clicked() ), this, SLOT(pickColor() ) );
	connect( btnOk, SIGNAL( clicked() ), this, SLOT(accept() ) );
	connect( btnApply, SIGNAL( clicked() ), this, SLOT(apply() ) );
	connect( tw, SIGNAL(currentChanged (QWidget *)), this, SLOT(enableButtonDefault(QWidget *)));
	connect( buttonDefault, SIGNAL(clicked()), this, SLOT(setDefaultValues()));
}

void lineDialog::initGeometryTab()
{
	geometry = new QWidget( tw );

	QHBox *box1 = new QHBox (geometry);
	box1->setSpacing(5);
	box1->setMargin(5);
	
	new QLabel(tr( "Unit" ), box1);
	unitBox = new QComboBox(box1);
	unitBox->insertItem(tr("Scale Coordinates"));
	unitBox->insertItem(tr("Pixels"));

	QHBox *box2 = new QHBox (geometry);
	box2->setSpacing(5);

    QButtonGroup *GroupBox2 = new QButtonGroup( 2,QGroupBox::Horizontal,tr("Start Point"),box2);

	new QLabel(tr( "X" ), GroupBox2, "TextLabel11",0);
	xStartBox = new QLineEdit(GroupBox2);
	
	new QLabel(tr( "Y" ),GroupBox2, "TextLabel111",0 );
	yStartBox = new QLineEdit(GroupBox2);

	QButtonGroup *GroupBox3 = new QButtonGroup( 2,QGroupBox::Horizontal,tr("End Point"),box2);

	new QLabel(tr( "X" ), GroupBox3, "TextLabel11",0);
	xEndBox = new QLineEdit(GroupBox3);

	new QLabel(tr( "Y" ),GroupBox3, "TextLabel111",0 );
	yEndBox = new QLineEdit(GroupBox3);

	QVBoxLayout* hl2 = new QVBoxLayout(geometry, 5, 5);
    hl2->addWidget(box1);
	hl2->addWidget(box2);

	tw->insertTab(geometry, tr( "&Geometry" ) );

	connect( unitBox, SIGNAL( activated(int) ), this, SLOT(displayCoordinates(int) ) );
	displayCoordinates(0);
}

void lineDialog::displayCoordinates(int unit)
{
if (unit == ScaleCoordinates)
	{
	QwtDoublePoint sp = lm->startPointCoord();
	xStartBox->setText(QString::number(sp.x()));
	yStartBox->setText(QString::number(sp.y()));

	QwtDoublePoint ep = lm->endPointCoord();
	xEndBox->setText(QString::number(ep.x()));
	yEndBox->setText(QString::number(ep.y()));
	}
else
	{
	QPoint startPoint = lm->startPoint();
	QPoint endPoint = lm->endPoint();

	xStartBox->setText(QString::number(startPoint.x()));
	yStartBox->setText(QString::number(startPoint.y()));

	xEndBox->setText(QString::number(endPoint.x()));
	yEndBox->setText(QString::number(endPoint.y()));
	}
}

void lineDialog::setCoordinates(int unit)
{
if (unit == ScaleCoordinates)
	{
	lm->setStartPoint(xStartBox->text().replace(",", ".").toDouble(), 
					yStartBox->text().replace(",", ".").toDouble());
	lm->setEndPoint(xEndBox->text().replace(",", ".").toDouble(), 
					yEndBox->text().replace(",", ".").toDouble());
	}
else
	{
	lm->setStartPoint(QPoint(xStartBox->text().toInt(), yStartBox->text().toInt()));
	lm->setEndPoint(QPoint(xEndBox->text().toInt(), yEndBox->text().toInt()));
	}
}

void lineDialog::apply()
{
if (tw->currentPage()==(QWidget *)options)
	{
	lm->setStyle(Graph::getPenStyle(styleBox->currentItem()));
	lm->setColor(colorBox->color());
	lm->setWidth(widthBox->currentText().toInt());
	lm->drawEndArrow(endBox->isChecked());
	lm->drawStartArrow(startBox->isChecked());
	}
else if (tw->currentPage()==(QWidget *)head)
	{
	if (lm->headLength() != boxHeadLength->value())
		lm->setHeadLength( boxHeadLength->value() );

	if (lm->headAngle() != boxHeadAngle->value())
		lm->setHeadAngle( boxHeadAngle->value() );

	if (lm->filledArrowHead() != filledBox->isChecked())
		lm->fillArrowHead( filledBox->isChecked() );
	}
else if (tw->currentPage()==(QWidget *)geometry)
	setCoordinates(unitBox->currentItem());

QwtPlot *plot = lm->plot();
Graph *g = (Graph *)plot->parent();
plot->replot();
g->emitModified();

enableHeadTab();
}

void lineDialog::accept()
{
apply();
close();
}

void lineDialog::setLineStyle(Qt::PenStyle style)
{
if (style==Qt::SolidLine)
	styleBox->setCurrentItem(0);
else if (style==Qt::DashLine)
	styleBox->setCurrentItem(1);
else if (style==Qt::DotLine)
	styleBox->setCurrentItem(2);	
else if (style==Qt::DashDotLine)
	styleBox->setCurrentItem(3);		
else if (style==Qt::DashDotDotLine)
	styleBox->setCurrentItem(4);
}
	
void lineDialog::enableHeadTab()
{
if (startBox->isChecked() || endBox->isChecked())
	tw->setTabEnabled (head, true);
else
	tw->setTabEnabled (head, false);
}

void lineDialog::pickColor()
{
QColor c = QColorDialog::getColor(colorBox->color(), this);
if ( !c.isValid() || c == colorBox->color() )
	return;

colorBox->setColor ( c ) ;
}
	
void lineDialog::setDefaultValues()
{
ApplicationWindow *app = (ApplicationWindow *)this->parent();
if (!app)
	return;

app->setArrowDefaultSettings(widthBox->currentText().toInt(), colorBox->color(), 
							Graph::getPenStyle(styleBox->currentItem()),
							boxHeadLength->value(), boxHeadAngle->value(), filledBox->isChecked());
}

void lineDialog::enableButtonDefault(QWidget *w)
{
if (w == geometry)
	buttonDefault->setEnabled(false);
else
	buttonDefault->setEnabled(true);
}

lineDialog::~lineDialog()
{
}
