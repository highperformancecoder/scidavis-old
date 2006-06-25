#include "configDialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qbuttongroup.h>
#include <qfont.h>
#include <qfontdialog.h> 
#include <qcolordialog.h>
#include <qwidgetstack.h>
#include <qwidget.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qstylefactory.h>
#include <qvbox.h>
#include <qregexp.h>
#include <qmessagebox.h>
#include <qtranslator.h>
#include <qapplication.h>
#include <qdir.h>
#include <qtabwidget.h>

#include "application.h"
#include "graph.h"
#include "colorButton.h"
#include "pixmaps.h"

configDialog::configDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
	ApplicationWindow *app = (ApplicationWindow *)parentWidget();
	plot3DColors = app->plot3DColors;
	plot3DTitleFont = app->plot3DTitleFont;
	plot3DNumbersFont = app->plot3DNumbersFont;
	plot3DAxesFont = app->plot3DAxesFont;
	textFont = app->tableTextFont;
	headerFont = app->tableHeaderFont;	
	appFont = app->appFont;
	axesFont = app->plotAxesFont;
	numbersFont = app->plotNumbersFont;
	legendFont = app->plotLegendFont;	
	titleFont = app->plotTitleFont;

    if ( !name )
		setName( "configDialog" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMouseTracking( TRUE );
    setSizeGripEnabled( FALSE );

	QHBox *box = new QHBox (this); 
	box->setSpacing (5);
	box->setMargin(5);
	
	itemsList = new QListBox (box);
	
	QVBox *vbox = new QVBox (box); 
	vbox->setSpacing (10);

	lblPageHeader = new QLabel (vbox);
	QFont fnt = this->font();
	fnt.setPointSize(fnt.pointSize() + 3);
	fnt.setBold(true);
	lblPageHeader->setFont(fnt);
	lblPageHeader->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
	lblPageHeader->setBackgroundColor(app->panelsColor);
	
	generalDialog = new QWidgetStack(vbox);
	
	initAppPage();
	initTablesPage();
	initPlotsPage();
	initPlots3DPage();
	
	generalDialog->addWidget(appTabWidget, 0);
	generalDialog->addWidget(tables, 1);
	generalDialog->addWidget(plotsTabWidget, 2);
	generalDialog->addWidget(plots3D, 3);

	GroupBox2 = new QButtonGroup( 3,QGroupBox::Horizontal, QString::null,this);
	GroupBox2->setFlat(TRUE);
	GroupBox2->setLineWidth(0);
	
	buttonApply = new QPushButton(GroupBox2);
	
	buttonOk = new QPushButton(GroupBox2);
    buttonOk->setDefault( TRUE );
   
    buttonCancel = new QPushButton(GroupBox2);
	
	QVBoxLayout* hlayout = new QVBoxLayout(this, 0, 0);
    hlayout->addWidget(box);
	hlayout->addWidget(GroupBox2);

    languageChange(); 
   
    // signals and slots connections
	connect( itemsList, SIGNAL(highlighted(int)), this, SLOT(update()));
	connect( itemsList, SIGNAL(highlighted(int)), this, SLOT(setCurrentPage(int)));
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( buttonApply, SIGNAL( clicked() ), this, SLOT( update() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( buttonBackground, SIGNAL( clicked() ), this, SLOT( pickBgColor() ) );
	connect( buttonText, SIGNAL( clicked() ), this, SLOT( pickTextColor() ) );
	connect( buttonHeader, SIGNAL( clicked() ), this, SLOT( pickHeaderColor() ) );
	connect( buttonTextFont, SIGNAL( clicked() ), this, SLOT( pickTextFont() ) );
	connect( buttonHeaderFont, SIGNAL( clicked() ), this, SLOT( pickHeaderFont() ) );

	setCurrentPage(0);
	}

void configDialog::setCurrentPage(int index)
{
generalDialog->raiseWidget(index);
lblPageHeader->setText(itemsList->currentText());
}

void configDialog::initTablesPage()
{
ApplicationWindow *app = (ApplicationWindow *)parentWidget();
tables = new QWidget( generalDialog, "tables" );

QHBox  *hbox1=new QHBox (tables, "hbox1"); 
hbox1->setSpacing(5);
	
lblSeparator = new QLabel(hbox1, "TextLabel1",0 );
boxSeparator = new QComboBox(true, hbox1, "boxSeparator" );

GroupBoxTableCol = new QButtonGroup(2,QGroupBox::Horizontal,tr("Colors"),tables,"GroupBoxTableCol" );

lblTableBackground = new QLabel(GroupBoxTableCol, "lblTableBackground",0 ); 
buttonBackground= new ColorButton(GroupBoxTableCol);
buttonBackground->setColor(app->tableBkgdColor);

lblTextColor = new QLabel(GroupBoxTableCol, "TextLabel69111",0 ); 
buttonText= new ColorButton(GroupBoxTableCol);
buttonText->setColor(app->tableTextColor);

lblHeaderColor = new QLabel(GroupBoxTableCol, "lblHeaderColor",0 ); 
buttonHeader= new ColorButton(GroupBoxTableCol);
buttonHeader->setColor(app->tableHeaderColor);
	
GroupBoxTableFonts = new QButtonGroup(2,QGroupBox::Horizontal,tr("Fonts"),tables,"GroupBoxTableFonts" );

buttonTextFont= new QPushButton(GroupBoxTableFonts, "buttonTextFont" );
buttonHeaderFont= new QPushButton(GroupBoxTableFonts, "buttonHeaderFont" );
	
QVBoxLayout* hlayout1 = new QVBoxLayout(tables, 0, 10, "hlayout1");
hlayout1->addWidget(hbox1);
hlayout1->addWidget(GroupBoxTableCol);
hlayout1->addWidget(GroupBoxTableFonts);
}

void configDialog::initPlotsPage()
{
ApplicationWindow *app = (ApplicationWindow *)parentWidget();
	
	plotsTabWidget = new QTabWidget(generalDialog);
	plots = new QWidget(plotsTabWidget);

	QHBox  *hbox1 = new QHBox (plots); 
	hbox1->setSpacing(5);

	QButtonGroup *GroupBoxOptions = new QButtonGroup( 2,QGroupBox::Horizontal, QString::null, hbox1);	
	boxAutoscaling = new QCheckBox(GroupBoxOptions,"boxAutoscaling");
	boxAutoscaling->setChecked(app->autoscale2DPlots);

	boxScaleFonts = new QCheckBox(GroupBoxOptions,"boxScaleFonts");
	boxScaleFonts->setChecked(app->autoScaleFonts);

    boxTitle= new QCheckBox(GroupBoxOptions);
	boxTitle->setChecked(app->titleOn);

	boxAllAxes= new QCheckBox(GroupBoxOptions);
	boxAllAxes->setChecked (app->allAxesOn);

	boxFrame= new QCheckBox(GroupBoxOptions);
	boxFrame->setChecked(app->canvasFrameOn);

	boxBackbones= new QCheckBox(GroupBoxOptions);
	boxBackbones->setChecked(app->drawBackbones);

	labelFrameWidth = new QLabel(GroupBoxOptions); 
	boxFrameWidth= new QSpinBox(1, 100, 1,GroupBoxOptions);	
	boxFrameWidth->setValue(app->canvasFrameWidth);
	if (!app->canvasFrameOn)
		{
		labelFrameWidth->hide();
		boxFrameWidth->hide();
		}

	lblLinewidth = new QLabel(GroupBoxOptions);  
	boxLinewidth= new QSpinBox(0, 100, 1,GroupBoxOptions);
	boxLinewidth->setValue(app->axesLineWidth);

	lblLegend = new QLabel(GroupBoxOptions); 
	boxLegend = new QComboBox(GroupBoxOptions);

	lblMargin = new QLabel(GroupBoxOptions); 
	boxMargin= new QSpinBox(0, 1000, 5, GroupBoxOptions);	
	boxMargin->setValue(app->defaultPlotMargin);

	boxResize = new QCheckBox(plots);
	boxResize->setChecked(!app->autoResizeLayers);
	if(boxResize->isChecked())
		boxScaleFonts->setEnabled(false);

	QVBoxLayout* hlayout2 = new QVBoxLayout(plots,5,5);
    hlayout2->addWidget(hbox1);
	hlayout2->addWidget(boxResize);

	plotsTabWidget->insertTab( plots, tr( "Options" ) );

	initCurvesPage();

	plotTicks = new QWidget(plotsTabWidget);

	QButtonGroup *GroupBox6 = new QButtonGroup(4, QGroupBox::Horizontal,QString::null,plotTicks);

	lblTicks = new QLabel(GroupBox6); 
	boxMajTicks = new QComboBox(GroupBox6);

	lblMajTicks = new QLabel(GroupBox6);  
	boxMajTicksLength = new QSpinBox(0, 100, 1,GroupBox6);
	boxMajTicksLength->setValue (app->majTicksLength);

	lblMinTicks = new QLabel(GroupBox6); 
	boxMinTicks = new QComboBox(GroupBox6);

	lblMinTicksLength = new QLabel(GroupBox6);  
	boxMinTicksLength= new QSpinBox(0, 100, 1,GroupBox6);
	boxMinTicksLength->setValue(app->minTicksLength);

	QHBoxLayout* hl = new QHBoxLayout(plotTicks,5,5);
	hl->addWidget(GroupBox6);

	plotsTabWidget->insertTab(plotTicks, tr( "Ticks" ) );

	plotFonts = new QWidget(plotsTabWidget);

	QButtonGroup *GroupBox2DFonts = new QButtonGroup(1,QGroupBox::Horizontal,QString::null,plotFonts);
	buttonTitleFont= new QPushButton(GroupBox2DFonts, "buttonTitleFont" );  
	buttonLegendFont= new QPushButton(GroupBox2DFonts, "buttonLegendFont" );	 
    buttonAxesFont= new QPushButton(GroupBox2DFonts, "buttonAxesFont" );   
	buttonNumbersFont= new QPushButton(GroupBox2DFonts, "buttonNumbersFont" );

	QHBoxLayout* hl2 = new QHBoxLayout(plotFonts,5,5);
	hl2->addWidget(GroupBox2DFonts);

	plotsTabWidget->insertTab(plotFonts, tr( "Fonts" ) );

	connect( boxResize, SIGNAL( clicked() ), this, SLOT( enableScaleFonts() ) );
	connect( boxFrame, SIGNAL( toggled(bool) ), this, SLOT( showFrameWidth(bool) ) );
	connect( buttonAxesFont, SIGNAL( clicked() ), this, SLOT( pickAxesFont() ) );
	connect( buttonNumbersFont, SIGNAL( clicked() ), this, SLOT( pickNumbersFont() ) );
	connect( buttonLegendFont, SIGNAL( clicked() ), this, SLOT( pickLegendFont() ) );
	connect( buttonTitleFont, SIGNAL( clicked() ), this, SLOT( pickTitleFont() ) );
}

void configDialog::enableScaleFonts()
{
if(boxResize->isChecked())
	boxScaleFonts->setEnabled(false);
else
	boxScaleFonts->setEnabled(true);
}

void configDialog::showFrameWidth(bool ok)
{
if (!ok)
	{
	boxFrameWidth->hide();
	labelFrameWidth->hide();
	}
else
	{
	boxFrameWidth->show();
	labelFrameWidth->show();
	}	
}

void configDialog::initPlots3DPage()
{
	ApplicationWindow *app = (ApplicationWindow *)parentWidget();
	plots3D = new QWidget( generalDialog, "plots3D" );

	QVBox  *box=new QVBox (plots3D, "box"); 
	box->setSpacing (5);

	QButtonGroup *GroupBox77 = new QButtonGroup(2,QGroupBox::Horizontal,QString::null, box, "GroupBox7" );
	lblResolution = new QLabel(GroupBox77, "lblResolution",0 ); 
	boxResolution = new QSpinBox(1, 100, 1, GroupBox77, "boxResolution" );
	boxResolution->setValue(app->plot3DResolution);

	boxShowLegend = new QCheckBox(GroupBox77, "boxShowLegend" );
	boxShowLegend->setChecked(app->showPlot3DLegend);

	boxShowProjection = new QCheckBox(GroupBox77, "boxShowProjection" );
	boxShowProjection->setChecked(app->showPlot3DProjection);

	boxSmoothMesh = new QCheckBox(GroupBox77, "boxSmoothMesh" );
	boxSmoothMesh->setChecked(app->smooth3DMesh);

	GroupBox3DCol = new QButtonGroup(4,QGroupBox::Horizontal,tr( "Colors" ),box, "GroupBox3DCol" );
	btnFromColor = new QPushButton( GroupBox3DCol, "btnFrom" );
	btnLabels = new QPushButton( GroupBox3DCol, "btnLabels" );
	btnMesh = new QPushButton( GroupBox3DCol, "btnMesh" );	
	btnGrid = new QPushButton( GroupBox3DCol, "btnGrid" );
	btnToColor = new QPushButton( GroupBox3DCol, "btnTo" );
	btnNumbers = new QPushButton( GroupBox3DCol, "btnNumbers" );
	btnAxes = new QPushButton( GroupBox3DCol, "btnAxes" );
	btnBackground3D = new QPushButton( GroupBox3DCol, "btnBackground" );

	GroupBox3DFonts = new QButtonGroup(3,QGroupBox::Horizontal,tr( "Fonts" ),box, "GroupBox3DFonts" );
	btnTitleFnt = new QPushButton( GroupBox3DFonts, "btnTitleFnt" );
	btnLabelsFnt = new QPushButton( GroupBox3DFonts, "btnLabelsFnt" );
	btnNumFnt = new QPushButton( GroupBox3DFonts, "btnNumFnt" );
   	
	QVBoxLayout* hlayout2 = new QVBoxLayout(plots3D, 0, 5, "hlayout2");
	hlayout2->addWidget(box);
	
	connect( btnAxes, SIGNAL( clicked() ), this, SLOT(pick3DAxesColor() ) );
	connect( btnLabels, SIGNAL( clicked() ), this, SLOT(pick3DLabelsColor() ) );
	connect( btnNumbers, SIGNAL( clicked() ), this, SLOT(pick3DNumbersColor() ) );
	connect( btnBackground3D, SIGNAL( clicked() ), this, SLOT(pick3DBackgroundColor()));
	connect( btnFromColor, SIGNAL( clicked() ), this, SLOT(pickDataMinColor() ) );
	connect( btnToColor, SIGNAL( clicked() ), this, SLOT(pickDataMaxColor() ) );
	connect( btnGrid, SIGNAL( clicked() ), this, SLOT(pickGridColor() ) );
	connect( btnMesh, SIGNAL( clicked() ), this, SLOT(pickMeshColor() ) );

	connect( btnNumFnt, SIGNAL( clicked() ), this, SLOT(pick3DNumbersFont() ) );
	connect( btnTitleFnt, SIGNAL( clicked() ), this, SLOT(pick3DTitleFont() ) );
	connect( btnLabelsFnt, SIGNAL( clicked() ), this, SLOT(pick3DAxesFont() ) );
}

void configDialog::initAppPage()
{
ApplicationWindow *app = (ApplicationWindow *)parentWidget();

appTabWidget = new QTabWidget(generalDialog);

application = new QWidget( appTabWidget);
QButtonGroup *GroupBoxApp = new QButtonGroup( 2,QGroupBox::Horizontal, QString::null, application);
    
	lblLanguage = new QLabel(GroupBoxApp); 
	boxLanguage = new QComboBox(GroupBoxApp);
	insertLanguagesList();

	lblStyle = new QLabel(GroupBoxApp, "lblStyle",0 ); 
	boxStyle = new QComboBox(GroupBoxApp,"style");
	QStringList styles = QStyleFactory::keys();
	styles.sort();
	boxStyle->insertStringList(styles);
	boxStyle->setCurrentText(app->appStyle);
	
	#ifdef Q_OS_MACX // Mac 
	 if (QApplication::macVersion() == Qt::MV_10_DOT_3)
			{
			lblStyle->hide();
			boxStyle->hide();
			}
	#endif
   
    lblFonts = new QLabel(GroupBoxApp, "lblFonts",0 ); 
	fontsBtn= new QPushButton(GroupBoxApp, "fontsBtn" );
	
	boxSave= new QCheckBox(GroupBoxApp, "boxSave" );
	boxSave->setChecked(app->autoSave);

	boxMinutes = new QSpinBox(1,100,1,GroupBoxApp);
	boxMinutes->setValue(app->autoSaveTime);
	boxMinutes->setEnabled(app->autoSave);

	boxSearchUpdates = new QCheckBox(GroupBoxApp);
	boxSearchUpdates->setChecked(app->autoSearchUpdates);

	QVBoxLayout* hlayout4 = new QVBoxLayout(application, 5, 5, "hlayout4");
    hlayout4->addWidget(GroupBoxApp);

	appTabWidget->insertTab(application, tr( "Application" ) );

	confirm = new QWidget(appTabWidget);
	
	GroupBoxConfirm = new QButtonGroup( 1,QGroupBox::Horizontal,tr("Prompt on closing"),confirm);
	boxFolders = new QCheckBox(GroupBoxConfirm);
	boxFolders->setChecked(app->confirmCloseFolder);
	
	boxTables = new QCheckBox(GroupBoxConfirm);
	boxTables->setChecked(app->confirmCloseTable);

	boxMatrixes = new QCheckBox(GroupBoxConfirm);
	boxMatrixes->setChecked(app->confirmCloseMatrix);

    boxPlots2D = new QCheckBox(GroupBoxConfirm);
	boxPlots2D->setChecked(app->confirmClosePlot2D);

	boxPlots3D = new QCheckBox(GroupBoxConfirm, "boxPlots3D" );
	boxPlots3D->setChecked(app->confirmClosePlot3D);

	boxNotes = new QCheckBox(GroupBoxConfirm, "boxNotes" );
	boxNotes->setChecked(app->confirmCloseNotes);

	QHBoxLayout* hlayout3 = new QHBoxLayout(confirm, 5, 5, "hlayout3");
    hlayout3->addWidget(GroupBoxConfirm);

	appTabWidget->insertTab(confirm, tr( "Confirmations" ) );

	appColors = new QWidget(appTabWidget);
	QButtonGroup *GroupBoxAppCol = new QButtonGroup(2,QGroupBox::Horizontal, QString::null,appColors);
 	
	lblWorkspace = new QLabel(GroupBoxAppCol, "lblWorkspace",0 ); 
	btnWorkspace = new ColorButton(GroupBoxAppCol);
	btnWorkspace->setColor(app->workspaceColor);

	lblPanels = new QLabel(GroupBoxAppCol, "lblPanels",0 ); 
	btnPanels = new ColorButton(GroupBoxAppCol);
	btnPanels->setColor(app->panelsColor);

	lblPanelsText = new QLabel(GroupBoxAppCol); 
	btnPanelsText = new ColorButton(GroupBoxAppCol);
	btnPanelsText->setColor(app->panelsTextColor);
	
	QVBoxLayout* hl = new QVBoxLayout(appColors, 5, 5);
	hl->addWidget(GroupBoxAppCol);

	appTabWidget->insertTab(appColors, tr( "Colors" ) );

	connect( boxLanguage, SIGNAL( activated(int) ), this, SLOT( switchToLanguage(int) ) );
	connect( fontsBtn, SIGNAL( clicked() ), this, SLOT( pickApplicationFont() ) );
	connect( boxSave, SIGNAL( toggled(bool) ), boxMinutes, SLOT( setEnabled(bool) ) );
	connect( btnWorkspace, SIGNAL( clicked() ), this, SLOT( pickWorkspaceColor() ) );
	connect( btnPanels, SIGNAL( clicked() ), this, SLOT( pickPanelsColor() ) );
	connect( btnPanelsText, SIGNAL( clicked() ), this, SLOT( pickPanelsTextColor() ) );
}

void configDialog::initCurvesPage()
{
ApplicationWindow *app = (ApplicationWindow *)parentWidget();

curves = new QWidget(plotsTabWidget);
QButtonGroup *GroupBox77 = new QButtonGroup( 2,QGroupBox::Horizontal,QString::null,curves,"GroupBox77" );
    
lblCurveStyle = new QLabel(GroupBox77, "lblCurveStyle",0 ); 	
boxCurveStyle = new QComboBox(GroupBox77,"boxCurveStyle");

lblLineWidth = new QLabel(GroupBox77, "lblLineWidth",0 ); 
boxCurveLineWidth = new QSpinBox(1,100,1,GroupBox77, "boxCurveLineWidth");
boxCurveLineWidth->setValue(app->defaultCurveLineWidth);

lblSymbSize = new QLabel(GroupBox77, "lblSymbSize",0 ); 
boxSymbolSize = new QSpinBox(1,100,1,GroupBox77, "boxSymbolSize");
boxSymbolSize->setValue(app->defaultSymbolSize);
	
QHBoxLayout* hlayout5 = new QHBoxLayout(curves, 5, 5, "hlayout5");
hlayout5->addWidget(GroupBox77);

plotsTabWidget->insertTab( curves, tr( "Curves" ) );
}

configDialog::~configDialog()
{
}

void configDialog::languageChange()
{
    setCaption( tr( "QtiPlot - Choose default settings" ) );
	ApplicationWindow *app = (ApplicationWindow *)parentWidget();

	itemsList->clear();
	itemsList->insertItem( QPixmap(general_xpm), tr( "General" ) );
    itemsList->insertItem( QPixmap(configTable_xpm), tr( "Tables" ) );
    itemsList->insertItem( QPixmap(config_curves_xpm), tr( "2D Plots" ) );
	itemsList->insertItem( QPixmap(logo_xpm), tr( "3D Plots" ) );
	itemsList->setSelected(0, true);
	
	//plots 2D page
	plotsTabWidget->setTabLabel (plots, tr("Options"));
	plotsTabWidget->setTabLabel (curves, tr("Curves"));
	plotsTabWidget->setTabLabel (plotTicks, tr("Ticks"));
	plotsTabWidget->setTabLabel (plotFonts, tr("Fonts"));

	boxResize->setText(tr("Do not &resize layers when window size changes"));
	lblMinTicksLength->setText(tr("Length"));  
	lblLinewidth->setText(tr("Axes linewidth" ));  
	lblMajTicks->setText(tr("Length" ));  
	lblTicks->setText(tr("Major Ticks" )); 
	lblMinTicks->setText(tr("Minor Ticks" ));

	lblMargin->setText(tr("Margin" )); 
	lblLegend->setText(tr("Legend frame" )); 
	labelFrameWidth->setText(tr("Frame width" )); 
	boxBackbones->setText(tr("Axes &backbones"));
	boxFrame->setText(tr("Canvas Fra&me"));
	boxAllAxes->setText(tr("Sho&w all axes"));
	boxTitle->setText(tr("Show &Title"));
	boxScaleFonts->setText(tr("Scale &Fonts"));
	boxAutoscaling->setText(tr("Auto&scaling"));
	boxLegend->clear();
	boxLegend->insertItem( tr( "None" ) );
	boxLegend->insertItem( tr( "Rectangle" ) );
    boxLegend->insertItem( tr( "Shadow" ) );
    boxLegend->insertItem( tr( "White out" ) );
    boxLegend->insertItem( tr( "Black out" ) );
	boxLegend->setCurrentItem(app->legendFrameStyle);

	boxMajTicks->clear();
	boxMajTicks->insertItem(tr("None"));
	boxMajTicks->insertItem(tr("Out"));
	boxMajTicks->insertItem(tr("In & Out"));
	boxMajTicks->insertItem(tr("In"));

	boxMinTicks->clear();
	boxMinTicks->insertItem(tr("None"));
	boxMinTicks->insertItem(tr("Out"));
	boxMinTicks->insertItem(tr("In & Out"));
	boxMinTicks->insertItem(tr("In"));

	boxMajTicks->setCurrentItem(app->majTicksStyle);
	boxMinTicks->setCurrentItem(app->minTicksStyle);

	//confirmations page
	GroupBoxConfirm->setTitle(tr("Prompt on closing"));
	boxFolders->setText(tr("Folders"));
	boxTables->setText(tr("Tables"));
	boxPlots3D->setText(tr("3D Plots"));
	boxPlots2D->setText(tr("2D Plots"));
	boxMatrixes->setText(tr("Matrixes"));
	boxNotes->setText(tr("&Notes"));

    buttonOk->setText( tr( "&OK" ) );
	buttonCancel->setText( tr( "&Close" ) );
	buttonApply->setText( tr( "&Apply" ) );
	buttonTextFont->setText( tr( "&Text Font" ) );
	buttonHeaderFont->setText( tr( "&Labels Font" ) );
	buttonAxesFont->setText( tr( "A&xes Legend" ) );
	buttonNumbersFont->setText( tr( "Axes &Numbers" ) );
	buttonLegendFont->setText( tr( "&Legend" ) );
	buttonTitleFont->setText( tr( "T&itle" ) );

	//application page
	appTabWidget->setTabLabel (application, tr("Application"));
	appTabWidget->setTabLabel (confirm, tr("Confirmations"));
	appTabWidget->setTabLabel (appColors, tr("Colors"));

	lblLanguage->setText(tr("Language"));
	lblStyle->setText(tr("Style")); 
	lblFonts->setText(tr("Fonts")); 
	fontsBtn->setText(tr("Choose &font"));
	lblWorkspace->setText(tr("Workspace")); 
	lblPanelsText->setText(tr("Panels text")); 
	lblPanels->setText(tr("Panels"));
	boxSave->setText(tr("Save every"));
	boxSearchUpdates->setText(tr("Check for new versions at startup"));
	boxMinutes->setSuffix(tr(" minutes"));

	//tables page
	GroupBoxTableCol->setTitle(tr("Colors"));
	lblSeparator->setText(tr("Default Column Separator"));
	boxSeparator->clear();
	boxSeparator->insertItem(tr("TAB"));
    boxSeparator->insertItem(tr("SPACE"));
	boxSeparator->insertItem(";" + tr("TAB"));
	boxSeparator->insertItem("," + tr("TAB"));
	boxSeparator->insertItem(";" + tr("SPACE"));
	boxSeparator->insertItem("," + tr("SPACE"));
    boxSeparator->insertItem(";");
    boxSeparator->insertItem(",");
	setColumnSeparator(app->separator);

	lblTableBackground->setText(tr( "Background" )); 
	lblTextColor->setText(tr( "Text" )); 
	lblHeaderColor->setText(tr("Labels")); 
	GroupBoxTableFonts->setTitle(tr("Fonts"));

	//curves page
	lblCurveStyle->setText(tr( "Default curve style" )); 	
	lblLineWidth->setText(tr( "Line width" )); 
	lblSymbSize->setText(tr( "Symbol size" )); 

	boxCurveStyle->clear();
	boxCurveStyle->insertItem( QPixmap(lPlot_xpm), tr( " Line" ) );
	boxCurveStyle->insertItem( QPixmap(pPlot_xpm), tr( " Scatter" ) );
	boxCurveStyle->insertItem( QPixmap(lpPlot_xpm), tr( " Line + Symbol" ) );
	boxCurveStyle->insertItem( QPixmap(dropLines_xpm), tr( " Vertical drop lines" ) );
	boxCurveStyle->insertItem( QPixmap(spline_xpm), tr( " Spline" ) );
	boxCurveStyle->insertItem( QPixmap(steps_xpm), tr( " Vertical steps" ) );
	boxCurveStyle->insertItem( QPixmap(area_xpm), tr( " Area" ) );
	boxCurveStyle->insertItem( QPixmap(vertBars_xpm), tr( " Vertical Bars" ) );
	boxCurveStyle->insertItem( QPixmap(hBars_xpm), tr( " Horizontal Bars" ) );

	int style = app->defaultCurveStyle;
	if (style == Graph::Line)
		boxCurveStyle->setCurrentItem(0);
	else if (style == Graph::Scatter)
		boxCurveStyle->setCurrentItem(1);
	else if (style == Graph::LineSymbols)
		boxCurveStyle->setCurrentItem(2);
	else if (style == Graph::VerticalDropLines)
		boxCurveStyle->setCurrentItem(3);
	else if (style == Graph::Spline)
		boxCurveStyle->setCurrentItem(4);
	else if (style == Graph::Steps)
		boxCurveStyle->setCurrentItem(5);
	else if (style == Graph::Area)
		boxCurveStyle->setCurrentItem(6);
	else if (style == Graph::VerticalBars)
		boxCurveStyle->setCurrentItem(7);
	else if (style == Graph::HorizontalBars)
		boxCurveStyle->setCurrentItem(8);

	//plots 3D
	lblResolution->setText(tr("Resolution")); 
	boxResolution->setSpecialValueText( "1 " + tr("(all data shown)") );
	boxShowLegend->setText(tr( "&Show Legend" ));
	boxShowProjection->setText(tr( "Show &Projection" ));
	btnFromColor->setText( tr( "&Data Max" ) );
	boxSmoothMesh->setText(tr( "Smoot&h Line" ));
	btnLabels->setText( tr( "Lab&els" ) );
	btnMesh->setText( tr( "Mesh &Line" ) );
	btnGrid->setText( tr( "&Grid" ) );
	btnToColor->setText( tr( "Data &Min" ) );
	btnNumbers->setText( tr( "&Numbers" ) );
	btnAxes->setText( tr( "A&xes" ) );
	btnBackground3D->setText( tr( "&Background" ) );
	GroupBox3DCol->setTitle(tr("Colors" ));
	GroupBox3DFonts->setTitle(tr("Fonts" ));
	btnTitleFnt->setText( tr( "&Title" ) );
	btnLabelsFnt->setText( tr( "&Axes Labels" ) );
	btnNumFnt->setText( tr( "&Numbers" ) );
}

void configDialog::accept()
{
update();	
close();
}

void configDialog::update()
{
ApplicationWindow *app = (ApplicationWindow *)parentWidget();
if (!app)
	return;

if (generalDialog->visibleWidget()==(QWidget*)tables)
	{
	QString sep = boxSeparator->currentText();
	sep.replace(tr("TAB"), "\t", false);
	sep.replace("\\t", "\t");
	sep.replace(tr("SPACE"), " ");
	sep.replace("\\s", " ");

	if (sep.contains(QRegExp("[0-9.eE+-]"))!=0)
		{
		QMessageBox::warning(0, tr("QtiPlot - Import options error"),
				tr("The separator must not contain the following characters: 0-9eE.+-"));
		return;
		}

	app->separator = sep;
	app->customizeTables(buttonBackground->color(), buttonText->color(), 
						 buttonHeader->color(), textFont, headerFont);
	}
else if (generalDialog->visibleWidget() == plotsTabWidget)
	{
	if (plotsTabWidget->currentPage() == plots)
		{		
		app->legendFrameStyle=boxLegend->currentItem();
		app->titleOn=boxTitle->isChecked();
		app->allAxesOn = boxAllAxes->isChecked();
		app->canvasFrameOn=boxFrame->isChecked();
		app->canvasFrameWidth = boxFrameWidth->value();
		app->drawBackbones = boxBackbones->isChecked();
		app->axesLineWidth = boxLinewidth->value();
		
		app->defaultPlotMargin = boxMargin->value();
		app->setGraphDefaultSettings(boxAutoscaling->isChecked(),boxScaleFonts->isChecked(),boxResize->isChecked());
		}
	else if (plotsTabWidget->currentPage() == curves)
		{
		app->defaultCurveStyle = curveStyle();
		app->defaultCurveLineWidth = boxCurveLineWidth->value();
		app->defaultSymbolSize = boxSymbolSize->value();
		}
	else if (plotsTabWidget->currentPage() == plotTicks)
		{
		app->majTicksLength = boxMajTicksLength->value();
		app->minTicksLength = boxMinTicksLength->value();
		app->majTicksStyle = boxMajTicks->currentItem();
		app->minTicksStyle = boxMinTicks->currentItem();
		}
	else if (plotsTabWidget->currentPage() == plotFonts)
		{
		app->plotAxesFont=axesFont;
		app->plotNumbersFont=numbersFont;
		app->plotLegendFont=legendFont;
		app->plotTitleFont=titleFont;
		}
	}
else if (generalDialog->visibleWidget()==(QWidget*)appTabWidget)
	{
	if (appTabWidget->currentPage() == application)
		{
		app->changeAppFont(appFont);
		setFont(appFont);
		app->changeAppStyle(boxStyle->currentText());
		app->autoSearchUpdates = boxSearchUpdates->isChecked();
		app->setSaveSettings(boxSave->isChecked(), boxMinutes->value());
		}
	else if (appTabWidget->currentPage() == confirm)
		{
		app->confirmCloseFolder = boxFolders->isChecked();
		app->updateConfirmOptions(boxTables->isChecked(), boxMatrixes->isChecked(),
							  boxPlots2D->isChecked(), boxPlots3D->isChecked(),
							  boxNotes->isChecked());
		}
	else if (appTabWidget->currentPage() == appColors)
		app->setAppColors(btnWorkspace->color(), btnPanels->color(), btnPanelsText->color());
	}
else if (generalDialog->visibleWidget()==(QWidget*)plots3D)
	{
	app->plot3DColors = plot3DColors;
	app->showPlot3DLegend = boxShowLegend->isChecked();
	app->showPlot3DProjection = boxShowProjection->isChecked();
	app->plot3DResolution = boxResolution->value();
	app->plot3DTitleFont = plot3DTitleFont;
	app->plot3DNumbersFont = plot3DNumbersFont;
	app->plot3DAxesFont = plot3DAxesFont;

	if (app->smooth3DMesh != boxSmoothMesh->isChecked())
		{
		app->smooth3DMesh = boxSmoothMesh->isChecked();
		app->setPlot3DOptions();
		}
	}
app->saveSettings();
}

int configDialog::curveStyle()
{
int style = 0;
switch (boxCurveStyle->currentItem())
	{
	case 0:
		style = Graph::Line;
	break;
	case 1:
		style = Graph::Scatter;
	break;
	case 2:
		style = Graph::LineSymbols;
	break;
	case 3:
		style = Graph::VerticalDropLines;
	break;
	case 4:
		style = Graph::Spline;
	break;
	case 5:
		style = Graph::Steps;
	break;
	case 6:
		style = Graph::Area;
	break;
	case 7:
		style = Graph::VerticalBars;
	break;
	case 8:
		style = Graph::HorizontalBars;
	break;
	}
return style;
}

void configDialog::pickBgColor()
{
QColor c = QColorDialog::getColor(buttonBackground->color(), this);
if ( !c.isValid() || c == buttonBackground->color())
		return;

buttonBackground->setColor(c);
}

void configDialog::pickTextColor()
{
QColor c = QColorDialog::getColor(buttonText->color(), this);
if ( !c.isValid() || c == buttonText->color())
		return;

buttonText->setColor(c);
}

void configDialog::pickHeaderColor()
{
QColor c = QColorDialog::getColor(buttonHeader->color(), this);
if ( !c.isValid() || c == buttonHeader->color())
		return;

buttonHeader->setColor(c);
}

void configDialog::pickTextFont()
{
bool ok;
QFont font = QFontDialog::getFont(&ok,textFont,this);
    if ( ok ) {
        textFont = font;
    } else {
     return;
    }
}

void configDialog::pickHeaderFont()
{
bool ok;
QFont font = QFontDialog::getFont(&ok,headerFont,this);
    if ( ok ) {
        headerFont = font;
    } else {
     return;
    }
}

void configDialog::pickLegendFont()
{
bool ok;
QFont font = QFontDialog::getFont(&ok,legendFont,this);
    if ( ok ) {
        legendFont = font;
    } else {
     return;
    }
}

void configDialog::pickAxesFont()
{
bool ok;
QFont font = QFontDialog::getFont(&ok,axesFont,this);
    if ( ok ) {
        axesFont = font;
    } else {
     return;
    }
}

void configDialog::pickNumbersFont()
{
bool ok;
QFont font = QFontDialog::getFont(&ok,numbersFont,this);
    if ( ok ) {
        numbersFont = font;
    } else {
     return;
    }
}

void configDialog::pickTitleFont()
{
bool ok;
QFont font = QFontDialog::getFont(&ok,titleFont,this);
    if ( ok ) 
		titleFont = font;
    else 
     	return;
}

void configDialog::pickApplicationFont()
{
bool ok;
QFont font = QFontDialog::getFont(&ok,appFont,this);
if ( ok ) 
	appFont = font;
else 
    return;
}

void configDialog::pickPanelsTextColor()
{
QColor c = QColorDialog::getColor(btnPanelsText->color(), this);
if ( !c.isValid() || c == btnPanelsText->color())
		return;

btnPanelsText->setColor(c);
}

void configDialog::pickPanelsColor()
{
QColor c = QColorDialog::getColor(btnPanels->color(), this);
if ( !c.isValid() || c == btnPanels->color())
		return;

btnPanels->setColor(c);
}

void configDialog::pickWorkspaceColor()
{
QColor c = QColorDialog::getColor(btnWorkspace->color(), this);
if ( !c.isValid() || c == btnWorkspace->color())
		return;

btnWorkspace->setColor(c);
}

void configDialog::pickDataMaxColor()
{
QColor c = QColorDialog::getColor(QColor(plot3DColors[0]), this );
if ( !c.isValid() )
		return;

plot3DColors[0] = c.name();
}

void configDialog::pickDataMinColor()
{
QColor c = QColorDialog::getColor(QColor(plot3DColors[4]), this );
if ( !c.isValid() )
		return;

plot3DColors[4] = c.name();
}

void configDialog::pick3DBackgroundColor()
{
QColor c = QColorDialog::getColor(QColor(plot3DColors[7]), this );
if ( !c.isValid() )
		return;

plot3DColors[7] = c.name();
}

void configDialog::pickMeshColor()
{
QColor c = QColorDialog::getColor(QColor(plot3DColors[2]), this );
if ( !c.isValid() )
		return;

plot3DColors[2] = c.name();
}

void configDialog::pickGridColor()
{
QColor c = QColorDialog::getColor(QColor(plot3DColors[3]), this );
if ( !c.isValid() )
		return;

plot3DColors[3] = c.name();
}

void configDialog::pick3DAxesColor()
{
QColor c = QColorDialog::getColor(QColor(plot3DColors[6]), this );
if ( !c.isValid() )
		return;

plot3DColors[6] = c.name();
}

void configDialog::pick3DNumbersColor()
{
QColor c = QColorDialog::getColor(QColor(plot3DColors[5]), this );
if ( !c.isValid() )
		return;

plot3DColors[5] = c.name();
}

void configDialog::pick3DLabelsColor()
{
QColor c = QColorDialog::getColor(QColor(plot3DColors[1]), this );
if ( !c.isValid() )
		return;

plot3DColors[1] = c.name();
}

void configDialog::pick3DTitleFont()
{
bool ok;
QFont font = QFontDialog::getFont(&ok, plot3DTitleFont,this);
if ( ok ) 
	plot3DTitleFont = font;
else 
	return;
}

void configDialog::pick3DNumbersFont()
{
bool ok;
QFont font = QFontDialog::getFont(&ok, plot3DNumbersFont,this);
if ( ok ) 
	plot3DNumbersFont = font;
else 
	return;
}

void configDialog::pick3DAxesFont()
{
bool ok;
QFont font = QFontDialog::getFont(&ok, plot3DAxesFont,this);
if ( ok ) 
	plot3DAxesFont = font;
else 
	return;
}

void configDialog::setColumnSeparator(const QString& sep)
{
if (sep=="\t")
	boxSeparator->setCurrentItem(0);
else if (sep==" ")
	boxSeparator->setCurrentItem(1);
else if (sep==";\t")
	boxSeparator->setCurrentItem(2);
else if (sep==",\t")
	boxSeparator->setCurrentItem(3);
else if (sep=="; ")
	boxSeparator->setCurrentItem(4);
else if (sep==", ")
	boxSeparator->setCurrentItem(5);
else if (sep==";")
	boxSeparator->setCurrentItem(6);
else if (sep==",")
	boxSeparator->setCurrentItem(7);
else
	{
	QString separator = sep;
	boxSeparator->setCurrentText(separator.replace(" ","\\s").replace("\t","\\t"));
	}
}

void configDialog::switchToLanguage(int param)
{
ApplicationWindow *app = (ApplicationWindow *)parentWidget();
app->switchToLanguage(param);
languageChange();
}

void configDialog::insertLanguagesList()
{
ApplicationWindow *app = (ApplicationWindow *)parentWidget();
QString qmPath = qApp->applicationDirPath() + "/translations";
QDir dir(qmPath);
QStringList locales = app->locales;
QStringList languages;
int lang = 0;
for (int i=0; i < (int)locales.size(); i++)
	{
	if (locales[i] == "en")
		languages.push_back("English");
	else
		{
		QTranslator translator;
		translator.load("qtiplot_"+locales[i], qmPath);

		QTranslatorMessage message = translator.findMessage("ApplicationWindow", "English");
		QString language = message.translation();
		if (!language.isEmpty())
			languages.push_back(language);
		else
			languages.push_back(locales[i]);
		}

	if (locales[i] == app->appLanguage)
		lang = i;
	}
boxLanguage->insertStringList(languages);
boxLanguage->setCurrentItem(lang);
}
