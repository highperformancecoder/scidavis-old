#ifndef FUNCTIONDIALOGUI_H
#define FUNCTIONDIALOGUI_H

#include <qvariant.h>
#include <qdialog.h>

class QLabel;
class QWidgetStack;
class QWidget;
class QLineEdit;
class QComboBox;
class QPushButton;
class QButtonGroup;
class QSpinBox;
class QTextEdit;

class functionDialogui : public QDialog
{
    Q_OBJECT

public:
    functionDialogui( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );

    QLabel* textFunction_2;
    QWidgetStack* optionStack;
    QWidget* functionPage;
    QLabel* textFunction;
    QLineEdit* boxFrom;
    QLabel* textFrom;
    QLabel* textTo;
    QLabel* textPoints;
    QLineEdit* boxTo;
    QTextEdit* boxFunction;
    QPushButton* buttonClear;
    QWidget* parametricPage;
    QLabel* textParameter;
    QLineEdit* boxParameter;
    QLabel* textParPoints;
    QLineEdit* boxParFrom;
    QLineEdit* boxParTo;
    QLabel* textParameterTo;
    QLabel* textYPar;
    QLabel* textXPar;
    QComboBox* boxXFunction;
    QComboBox* boxYFunction;
    QLabel* textParameterFrom;
    QWidget* polarPage;
    QLabel* textPolarPoints;
    QLabel* textPolarParameter;
    QLabel* textPolarFrom;
    QLabel* textPolarTo;
    QLabel* textPolarRadius;
    QLineEdit* boxPolarParameter;
    QLineEdit* boxPolarFrom;
    QLineEdit* boxPolarTo;
    QComboBox* boxPolarRadius;
    QLabel* textPolarTheta;
    QComboBox* boxPolarTheta;
    QComboBox* boxType;
    QButtonGroup  *GroupBox1, *GroupBox2, *GroupBox3, *GroupBox4;
    QPushButton* buttonCancel;
    QPushButton* buttonOk;
	QSpinBox *boxPoints, *boxParPoints, *boxPolarPoints;

protected slots:
    void languageChange();
	void raiseWidget(int index);
};

#endif // FUNCTIONDIALOGUI_H
