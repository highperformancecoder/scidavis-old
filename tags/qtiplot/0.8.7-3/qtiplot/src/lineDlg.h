#ifndef LINEDIALOG_H
#define LINEDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class QCheckBox;
class QComboBox;
class QButtonGroup;
class QPushButton;
class QTabWidget;
class QWidget;
class QSpinBox;
class QLineEdit;
class ColorButton;
class LineMarker;

class lineDialog : public QDialog
{ 
    Q_OBJECT

public:
    lineDialog(LineMarker *line, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0);
    ~lineDialog();

	void initGeometryTab();
	void enableHeadTab();
	void setCoordinates(int unit);

    QButtonGroup* GroupBox1, *GroupBox2;
    ColorButton* colorBox;
    QComboBox* styleBox;
    QComboBox* widthBox;
	QComboBox* unitBox;
    QPushButton* btnOk;
    QPushButton* btnApply;
	QPushButton* buttonDefault;

    QCheckBox* endBox;
    QCheckBox* startBox,  *filledBox;
	QTabWidget* tw;
	QWidget *options, *geometry, *head;
	QLineEdit *xStartBox, *yStartBox, *xEndBox, *yEndBox;
	QSpinBox *boxHeadAngle, *boxHeadLength;

public slots:
	void enableButtonDefault(QWidget *w);
	void setDefaultValues();
	void displayCoordinates(int unit);
	void setLineStyle(Qt::PenStyle style);
	void pickColor();
	void accept();
	void apply();

private:
	LineMarker *lm;
};

#endif // LINEDIALOG_H
