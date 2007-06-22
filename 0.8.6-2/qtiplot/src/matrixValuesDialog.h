#ifndef MVALUESDIALOG_H
#define MVALUESDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class QComboBox;
class QTextEdit;
class ScriptEdit;
class QSpinBox;
class QPushButton;
class Matrix;
class ScriptingEnv;
	
class matrixValuesDialog : public QDialog
{ 
    Q_OBJECT

public:
    matrixValuesDialog( ScriptingEnv *env, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~matrixValuesDialog();
	
	QSize sizeHint() const ;

    QComboBox* functions;
    QPushButton* PushButton3; 
    QPushButton* btnOk, *btnAddCell;
    QPushButton* btnCancel;
    ScriptEdit* commandes;
    QTextEdit* explain;
	QSpinBox *startRow, *endRow, *startCol, *endCol;
	QPushButton *btnApply;

public slots:
	void accept();
	bool apply();
	void setFunctions();
	void addCell();
	void insertFunction();
	void insertExplain(int index);
	void setMatrix(Matrix *m);

private:
	Matrix *matrix;
	ScriptingEnv *scriptEnv;
};

#endif //
