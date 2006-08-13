#ifndef FITDIALOG_H
#define FITDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class QPushButton;
class QLineEdit;
class QComboBox;
class QWidgetStack;
class QWidget;
class QTextEdit;
class QListBox;
class QCheckBox;
class QTable;
class QSpinBox;
class QLabel;
class QRadioButton;
class QLineEdit;
class Graph;
class ColorBox;
class Fitter;

class fitDialog : public QDialog
{
    Q_OBJECT

public:
    fitDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~fitDialog();

	void initFitPage();
	void initEditPage();
	void initAdvancedPage();

	QCheckBox* boxUseBuiltIn;
	QWidgetStack* tw;
    QPushButton* buttonOk;
	QPushButton* buttonCancel;
	QPushButton* buttonAdvanced;
	QPushButton* buttonClear;
	QPushButton* buttonPlugins;
	QPushButton* btnBack;
	QComboBox* boxCurve;
	QComboBox* boxSolver;
	QTable* boxParams;
	QLineEdit* boxFrom;
	QLineEdit* boxTo;
	QLineEdit* boxTolerance;
	QSpinBox *boxPoints, *generatePointsBox, *boxPrecision, *polynomOrderBox;
	QWidget *fitPage, *editPage, *advancedPage;
	QTextEdit *editBox, *explainBox, *boxFunction;
	QListBox *categoryBox, *funcBox;
	QLineEdit *boxName, *boxParam;
	QLabel *lblFunction, *lblPoints, *polynomOrderLabel;
	QPushButton *btnAddFunc, *btnDelFunc, *btnContinue, *btnApply;
	QPushButton *buttonEdit, *btnAddTxt, *btnAddName, *btnDeleteTables;
	ColorBox* boxColor;
	QComboBox *boxWeighting, *tableNamesBox, *colNamesBox;
	QRadioButton *generatePointsBtn, *samePointsBtn;
	QPushButton *btnParamTable, *btnCovMatrix;
	QLineEdit *covMatrixName, *paramTableName;
	QCheckBox *plotLabelBox, *logBox;
	Fitter *fitter;

protected:
	void closeEvent (QCloseEvent * e );

public slots:
	void accept();
	void insertFunctionsList(const QStringList& list);
	void clearList();
	void showFitPage();
	void showEditPage();
	void showAdvancedPage();
	void showFunctionsList(int category);
	void showParseFunctions();
	void showUserFunctions();
	void loadPlugins();
	void showExpression(int function);
	void addFunction();
	void addFunctionName();
	void setFunction(bool ok);
	void saveUserFunction();
	void removeUserFunction();
	void setBuiltInFunctionNames();
	void setBuiltInFunctions();
	bool containsUserFunctionName(const QString& s);
	void setGraph(Graph *g);
	void activateCurve(int index);
	void choosePluginsFolder();
	bool validInitialValues();
	void changeDataRange();
	void fitBuiltInFunction(const QString& function, const QStringList& initVal);

	void setSrcTables(QWidgetList* tables);
	void selectSrcTable(int tabnr);
	void enableWeightingParameters(int index);
	void showPointsBox(bool);
	void showParametersTable();
	void showCovarianceMatrix();

	//! Applies the user changes to the numerical format of the output results
	void applyChanges();
	void enableApplyChanges(int);

signals:
	void clearFunctionsList();
	void saveFunctionsList(const QStringList&);

private:
	Graph *graph;
	QStringList userFunctions, userFunctionNames, userFunctionParams;
	QStringList builtInFunctionNames, builtInFunctions;
	QStringList pluginFunctionNames, pluginFunctions, pluginFilesList, pluginParameters;
	QWidgetList *srcTables;
};

#endif // FITDIALOG_H
