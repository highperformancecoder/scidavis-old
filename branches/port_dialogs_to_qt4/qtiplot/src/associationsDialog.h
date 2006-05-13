#ifndef associationsDialog_H
#define associationsDialog_H

#include <qvariant.h>
#include <qdialog.h>
#include <qwidget.h>
//Added by qt3to4:
#include <QLabel>
#include <QEvent>

class QLabel;
class Q3ListBox;
class QPushButton;
class Q3Table;
class Q3TableItem;
class QStringList;
class Table;
class Graph;

class associationsDialog : public QDialog
{ 
    Q_OBJECT

public:
    associationsDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~associationsDialog();


	QLabel* tableCaptionLabel;
	Q3Table *table;
	QPushButton *btnOK, *btnCancel, *btnApply;
    Q3ListBox* associations;

public slots:
	Table * findTable(int index);
	void initTablesList(QWidgetList* lst, int curve);
	void updateTable(int index);
	void updateColumnTypes();
	void uncheckCol(int col);
	void updatePlotAssociation(int row, int col);
	void updateCurve();
	void accept();
	QString plotAssociation(const QString& text);
	void setGraph(Graph *g);

	bool eventFilter(QObject *object, QEvent *e);

private:
	QWidgetList* tables;
	Table *active_table;
	Graph *graph;
	QStringList plotAssociationsList;
};

#endif // associationsDialog_H
