#ifndef configDialog_H
#define configDialog_H

#include <qvariant.h>
#include <qdialog.h>
#include <qhbox.h> 
#include <qlayout.h>
#include <qlistbox.h>

class QButtonGroup;
class QPushButton;
class QWidgetStack;
class QWidget;
class QCheckBox;
class QComboBox;
class QSpinBox;
class QListBox;
class QLabel;
class QGroupBox;
class QTabWidget;
class ColorButton;
class QRadioButton;

class configDialog : public QDialog
{
    Q_OBJECT

public:
    configDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~configDialog();

	void initPlotsPage();
	void initAppPage();
	void initCurvesPage();
	void initPlots3DPage();
	void initTablesPage();
	void initFittingPage();

	QTabWidget *plotsTabWidget, *appTabWidget;

	QPushButton *btnBackground3D, *btnMesh, *btnAxes, *btnLabels, *btnNumbers;
	QPushButton *btnFromColor, *btnToColor, *btnGrid;
	QPushButton	*btnTitleFnt, *btnLabelsFnt, *btnNumFnt;

	ColorButton *buttonBackground, *buttonText, *buttonHeader;
    QPushButton *buttonOk, *buttonCancel, *buttonApply;
	QPushButton* buttonTextFont, *buttonHeaderFont;
    QButtonGroup* GroupBox1, *GroupBox2, *GroupBox3;
    QWidgetStack* generalDialog;
	QWidget *appColors, *tables, *plots, *plotTicks, *plotFonts, *confirm; 
	QWidget *application, *curves, *plots3D, *fitPage;
	QPushButton* buttonAxesFont, *buttonNumbersFont, *buttonLegendFont, *buttonTitleFont, *fontsBtn;
	QCheckBox *boxSearchUpdates, *logBox, *plotLabelBox;
	QCheckBox *boxTitle, *boxFrame, *boxPlots3D, *boxPlots2D, *boxTables, *boxNotes, *boxFolders;
	QCheckBox *boxSave, *boxBackbones, *boxAllAxes, *boxShowLegend, *boxSmoothMesh;
	QCheckBox *boxAutoscaling, *boxShowProjection, *boxMatrixes, *boxScaleFonts, *boxResize;
	QComboBox *boxMajTicks, *boxStyle, *boxCurveStyle, *boxSeparator, *boxLanguage, *boxMinTicks;
	QSpinBox *boxMinutes, *boxLinewidth, *boxFrameWidth, *boxResolution, *boxMargin, *boxPrecision;
	QSpinBox *boxCurveLineWidth, *boxSymbolSize, *boxMajTicksLength, *boxMinTicksLength, *generatePointsBox;
	ColorButton *btnWorkspace, *btnPanels, *btnPanelsText;
	QListBox *itemsList;
	QLabel *labelFrameWidth, *lblLanguage, *lblWorkspace, *lblPanels, *lblPageHeader;
	QLabel *lblPanelsText, *lblFonts, *lblStyle, *lblMinTicksLength;
	QGroupBox *GroupBoxConfirm;
	QGroupBox *GroupBoxTableFonts, *GroupBoxTableCol;
	QLabel *lblSeparator, *lblTableBackground, *lblTextColor, *lblHeaderColor;
	QLabel *lblSymbSize, *lblLineWidth, *lblCurveStyle, *lblResolution, *lblPrecision;
	QGroupBox *GroupBox3DFonts, *GroupBox3DCol;
	QGroupBox *GroupBoxFittingCurve, *GroupBoxFitParameters;
	QLabel *lblMargin, *lblTicks, *lblMajTicks, *lblLinewidth, *lblMinTicks, *lblPoints;
	QRadioButton *samePointsBtn, *generatePointsBtn;

public slots:
    virtual void languageChange();
	void insertLanguagesList();

	void accept();
	void update();

	void setCurrentPage(int index);

	void setColumnSeparator(const QString& sep);
	//table colors
	void pickBgColor();
	void pickTextColor();
	void pickHeaderColor();

	//table fonts
	void pickTextFont();
	void pickHeaderFont();

	//graph fonts
	void pickAxesFont();
	void pickNumbersFont();
	void pickLegendFont();
	void pickTitleFont();

	void enableScaleFonts();
	void showFrameWidth(bool ok);

	//application
	void pickApplicationFont();
	void pickPanelsTextColor();
	void pickPanelsColor();
	void pickWorkspaceColor();
	
	//2D curves
	int curveStyle();

	void pickDataMaxColor();
	void pickDataMinColor();
	void pick3DBackgroundColor();
	void pickMeshColor();
	void pickGridColor();
	void pick3DAxesColor();
	void pick3DNumbersColor();
	void pick3DLabelsColor();
	void pick3DTitleFont();
	void pick3DNumbersFont();
	void pick3DAxesFont();

	//Fitting
	void showPointsBox(bool);

	void switchToLanguage(int param);

private:
	QFont textFont, headerFont, axesFont, numbersFont, legendFont, titleFont, appFont;
	QFont plot3DTitleFont, plot3DNumbersFont, plot3DAxesFont;
	QStringList plot3DColors;
};

#endif // configDialog_H
