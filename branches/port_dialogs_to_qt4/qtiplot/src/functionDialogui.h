#ifndef FUNCTIONDIALOGUI_H
#define FUNCTIONDIALOGUI_H

#include <qvariant.h>
#include <qdialog.h>
//Added by qt3to4:
#include <QLabel>

class QLabel;
class Q3WidgetStack;
class QWidget;
class QLineEdit;
class QComboBox;
class QPushButton;
class Q3ButtonGroup;
class QSpinBox;

class functionDialogui : public QDialog
{
    Q_OBJECT

public:
    functionDialogui( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~functionDialogui();

    QLabel* textFunction_2;
    Q3WidgetStack* optionStack;
    QWidget* functionPage;
    QLabel* textFunction;
    QLineEdit* boxFrom;
    QLabel* textFrom;
    QLabel* textTo;
    QLabel* textPoints;
    QLineEdit* boxTo;
    QComboBox* boxFunction;
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
    Q3ButtonGroup  *GroupBox1, *GroupBox2, *GroupBox3, *GroupBox4;
    QPushButton* buttonCancel;
    QPushButton* buttonOk;
	QSpinBox    *boxPoints, *boxParPoints, *boxPolarPoints;

protected slots:
    virtual void languageChange();

};

#endif // FUNCTIONDIALOGUI_H
