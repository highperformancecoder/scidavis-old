#ifndef INTDIALOG_H
#define INTDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class Q3ButtonGroup;
class QPushButton;
class QCheckBox;
class QLineEdit;
class QComboBox;
class QSpinBox;
class Graph;
	
class intDialog : public QDialog
{
    Q_OBJECT

public:
    intDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~intDialog();

    QPushButton* buttonOk;
	QPushButton* buttonCancel;
	QPushButton* buttonHelp;
    Q3ButtonGroup* GroupBox1, *GroupBox2;
    QCheckBox* boxShowFormula;
	QComboBox* boxName;
	QSpinBox* boxOrder;
	QSpinBox* boxSteps;
	QLineEdit* boxStart;
	QLineEdit* boxEnd;
	QLineEdit* boxTol;	

public slots:
    virtual void languageChange();
	void accept();
	void setGraph(Graph *g);
	void activateCurve(int index);
	void changeCurve(int index);
	void help();
	void changeDataRange();

signals:
	void integrate(int,int,int,double,double,double);

private:
	Graph *graph;
};

#endif 


