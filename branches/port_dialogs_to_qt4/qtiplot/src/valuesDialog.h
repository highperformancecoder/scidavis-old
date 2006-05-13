#ifndef VALUESDIALOG_H
#define VALUESDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3HBoxLayout>
#include <QLabel>
#include <Q3GridLayout>
class Q3VBoxLayout; 
class Q3HBoxLayout; 
class Q3GridLayout; 
class QComboBox;
class Q3TextEdit;
class QLineEdit;
class QPushButton;
class QLabel;
class Table;
	
class setColValuesDialog : public QDialog
{ 
    Q_OBJECT

public:
    setColValuesDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~setColValuesDialog();
	
	QSize sizeHint() const ;

    QComboBox* functions;
    QComboBox* boxColumn;
    QPushButton* PushButton3; 
    QPushButton* PushButton4;
    QPushButton* btnOk;
    QPushButton* btnCancel;
    Q3TextEdit* commandes;
    Q3TextEdit* explain;
	QLineEdit* start, *end;
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
