#ifndef FUNCTIONDIALOG_H
#define FUNCTIONDIALOG_H

#include "functionDialogui.h"
#include <qvariant.h>
#include <qdialog.h>

class Graph;
	
class fDialog : public functionDialogui
{
    Q_OBJECT

public:
    fDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~fDialog();

public slots:
	void accept();
	void acceptFunction();
	void acceptParametric();
	void acceptPolar();
	void setCurveToModify(Graph *g, int curve);
	void insertParamFunctionsList(const QStringList& xList, const QStringList& yList);
	void insertPolarFunctionsList(const QStringList& rList, const QStringList& tetaList);
	void clearList();
	void setGraph(Graph *g){graph = g;};

signals:
	void newFunctionPlot(int, QStringList &, const QString &, QValueList<double> &, int);
	void clearParamFunctionsList();
	void clearPolarFunctionsList();

private:
	Graph *graph;
	int curveID;
};

#endif // EXPORTDIALOG_H
