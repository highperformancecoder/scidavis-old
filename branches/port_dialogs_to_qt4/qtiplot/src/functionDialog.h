#ifndef FUNCTIONDIALOG_H
#define FUNCTIONDIALOG_H

#include "functionDialogui.h"
#include <qvariant.h>
#include <qdialog.h>
//Added by qt3to4:
#include <Q3ValueList>

class Graph;
	
class fDialog : public functionDialogui
{
    Q_OBJECT

public:
    fDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~fDialog();

public slots:
	void accept();
	void acceptFunction();
	void acceptParametric();
	void acceptPolar();
	void setCurveToModify(const QString& s, int curve);
	void insertFunctionsList(const QStringList& list);
	void insertParamFunctionsList(const QStringList& xList, const QStringList& yList);
	void insertPolarFunctionsList(const QStringList& rList, const QStringList& tetaList);
	void clearList();
	void setGraph(Graph *g){graph = g;};

signals:
	void newFunctionPlot(QString& ,QStringList &,QStringList &,Q3ValueList<double> &,Q3ValueList<int> &);
	void clearFunctionsList();
	void clearParamFunctionsList();
	void clearPolarFunctionsList();

private:
	Graph *graph;
	int curveID;
};

#endif // EXPORTDIALOG_H
