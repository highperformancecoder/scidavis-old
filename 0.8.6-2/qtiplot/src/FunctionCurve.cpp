#include "FunctionCurve.h"
#include <qpainter.h>

FunctionCurve::FunctionCurve(const char *name):
    QwtPlotCurve(name)
{
}

FunctionCurve::FunctionCurve(const FunctionType& t, const char *name):
    QwtPlotCurve(name),
	d_type(t)
{
}

void FunctionCurve::setRange(double from, double to)
{
range_from = from;
range_to = to;
}

void FunctionCurve::copy(FunctionCurve* f)
{
d_type = f->functionType();
d_variable = f->variable();
d_formulas = f->formulas();
range_from = f->startRange();
range_to = f->endRange();
}