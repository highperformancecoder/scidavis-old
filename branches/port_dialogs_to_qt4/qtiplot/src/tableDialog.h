#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
//Added by qt3to4:
#include <QLabel>
#include <QCloseEvent>
#include "worksheet.h"

class QPushButton;
class QLineEdit;
class QCheckBox;
class Q3ButtonGroup;
class QComboBox;
class QLabel;
class Q3TextEdit;
class QSpinBox;

class tableDialog : public QDialog
{
    Q_OBJECT

public:
    tableDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~tableDialog();

    QPushButton* buttonCancel;
	QPushButton* buttonApply;
	QPushButton* buttonPrev;
	QPushButton* buttonNext;
    QLineEdit* colName;
    QCheckBox* enumerateAllBox;
    QPushButton* buttonOk;
 	Q3ButtonGroup* GroupBox2;
    QCheckBox* applyToRightCols;
    QComboBox* formatBox;
	QComboBox* displayBox;
    QComboBox* columnsBox;
    QSpinBox* colWidth, *precisionBox;
    QCheckBox* applyToAllBox;
	QLabel *labelNumeric, *labelFormat;
	Q3TextEdit *comments;

protected slots:
    virtual void languageChange();

public slots:
	void prevColumn();
	void nextColumn();
	void updateColumn(int);
	void changeColWidth(int width);
	void setWorksheet(Table * table);
	void showPrecisionBox(int item);
	void updateFormat();
	void updatePrecision(int prec);
	void setPlotDesignation(int i);
	void accept();
	void apply();
	void closeEvent( QCloseEvent *);
	void updateDisplay(int item);
	void enablePrecision(int f);

signals:
	void nameChanged(const QString&);
	void enumRightCols(bool);
	void changeWidth(const QString&, bool);

protected:
	Table * w;
};

#endif // TABLEDIALOG_H
