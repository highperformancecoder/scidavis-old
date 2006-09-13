#ifndef VALUESDIALOG_H
#define VALUESDIALOG_H

#include "Scripting.h"

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QComboBox;
class QTextEdit;
class QSpinBox;
class QPushButton;
class QLabel;
class Table;
class ScriptingEnv;
class ScriptEdit;
	
class setColValuesDialog : public QDialog, public scripted
{ 
    Q_OBJECT

public:
    setColValuesDialog( ScriptingEnv *env, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~setColValuesDialog();
	
	QSize sizeHint() const ;
	void customEvent( QCustomEvent* e);

    QComboBox* functions;
    QComboBox* boxColumn;
    QPushButton* PushButton3; 
    QPushButton* PushButton4;
    QPushButton* btnOk;
    QPushButton* btnCancel;
    ScriptEdit* commandes;
    QTextEdit* explain;
	QSpinBox* start, *end;
	QPushButton *buttonPrev, *buttonNext, *addCellButton, *btnApply;
	QLabel *colNameLabel;

public slots:
	void accept();
	bool apply();
	void prevColumn();
	void nextColumn();
	void setFunctions();
	void insertFunction();
	void insertCol();
	void insertCell();
	void insertExplain(int index);
	void setTable(Table* w);
	void updateColumn(int sc);

private:
	Table* table;
};

#endif //
