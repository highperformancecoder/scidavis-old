#ifndef FUNCTIONCURVE_H
#define FUNCTIONCURVE_H

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

class FunctionCurve: public QwtPlotCurve
{
public:
	enum {RTTI = 1001};
	enum FunctionType{Normal = 0, Parametric = 1, Polar = 2};
	FunctionCurve(const FunctionType& t, const char *name=0);
	FunctionCurve(const char *name=0);

	double startRange(){return range_from;};
	double endRange(){return range_to;};
	void setRange(double from, double to);

	QStringList formulas(){return d_formulas;};
	void setFormulas(const QStringList& lst){d_formulas = lst;};

	//! Provided for convenience when dealing with normal functions
	void setFormula(const QString& s){d_formulas = QStringList() << s;};

	QString variable(){return d_variable;};
	void setVariable(const QString& s){d_variable = s;};

	FunctionType functionType(){return d_type;};
	void setFunctionType(const FunctionType& t){d_type = t;};

	void copy(FunctionCurve *f);
	virtual int rtti () const;

	//! Returns a string used when saving to a project file
	QString saveToString();

	//! Returns a string that can be displayed in a plot legend
	QString legend();
	
private:
	FunctionType d_type;
	QString d_variable;
	QStringList d_formulas;
	double range_from, range_to;
};

#endif 

