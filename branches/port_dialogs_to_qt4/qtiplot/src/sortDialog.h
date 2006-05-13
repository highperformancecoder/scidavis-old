#ifndef SORTDIALOG_H
#define SORTDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
#include <q3valuelist.h> 

class Q3ButtonGroup;
class QPushButton;
class QCheckBox;
class QLineEdit;
class QComboBox;
	
class sortDialog : public QDialog
{
    Q_OBJECT

public:
    sortDialog( QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~sortDialog();

    QPushButton* buttonOk;
	QPushButton* buttonCancel;
	QPushButton* buttonHelp;
    Q3ButtonGroup* GroupBox1, *GroupBox2;
	QComboBox* boxType;
	QComboBox* boxOrder;
	QComboBox *columnsList;
	

public slots:
    virtual void languageChange();
	void accept();
	void insertColumnsList(const QStringList& cols);
	void changeType(int index);

signals:
	void sort(int, int, const QString&);

};

#endif // EXPORTDIALOG_H
