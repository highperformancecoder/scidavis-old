#include "Spectrogram.h"
#include <math.h>
#include <qpen.h>

#include <qwt_color_map.h>
#include <qwt_scale_widget.h>

Spectrogram::Spectrogram():
	QwtPlotSpectrogram(),
	d_matrix(0),
	color_axis(QwtPlot::yRight),
	color_map(Default)
{
}

Spectrogram::Spectrogram(Matrix *m):
	QwtPlotSpectrogram(QString(m->name())),
	d_matrix(m),
	color_axis(QwtPlot::yRight),
	color_map(Default)
{
setData(MatrixData(m));
double step = fabs(data().range().maxValue() - data().range().minValue())/5.0;

QwtValueList contourLevels;
for ( double level = data().range().minValue() + step; 
	level < data().range().maxValue(); level += step )
    contourLevels += level;

setContourLevels(contourLevels);
}

void Spectrogram::updateData(Matrix *m)
{
if (!m)
	return;

QwtPlot *plot = this->plot();
if (!plot)
	return;

setData(MatrixData(m));
setLevelsNumber(levels());

QwtScaleWidget *colorAxis = plot->axisWidget(color_axis);
if (colorAxis)
	colorAxis->setColorMap(data().range(), colorMap());

plot->setAxisScale(color_axis, data().range().minValue(), data().range().maxValue());
plot->replot();
}

void Spectrogram::setLevelsNumber(int levels)
{
double step = fabs(data().range().maxValue() - data().range().minValue())/(double)levels;

QwtValueList contourLevels;
for ( double level = data().range().minValue() + step; 
	level < data().range().maxValue(); level += step )
    contourLevels += level;

setContourLevels(contourLevels);	
}

bool Spectrogram::hasColorScale()
{
QwtPlot *plot = this->plot();
if (!plot)
	return false;

QwtScaleWidget *colorAxis = plot->axisWidget(color_axis);
return colorAxis->isColorBarEnabled();
}

void Spectrogram::showColorScale(int axis, bool on)
{
if (hasColorScale() == on && color_axis == axis)
	return;

QwtPlot *plot = this->plot();
if (!plot)
	return;

QwtScaleWidget *colorAxis = plot->axisWidget(color_axis);
colorAxis->setColorBarEnabled(false);

color_axis = axis;

// We must switch the main and the color scale axes and their scales
int xAxis = this->xAxis();
int yAxis = this->yAxis();
int oldMainAxis;
if (axis == QwtPlot::xBottom || axis == QwtPlot::xTop)
	{
	oldMainAxis = xAxis;
	xAxis = 5 - color_axis;
	}
else if (axis == QwtPlot::yLeft || axis == QwtPlot::yRight)
	{
	oldMainAxis = yAxis;
	yAxis = 1 - color_axis;
	}
// First we must switch axes
setAxis(xAxis, yAxis);

// Next we switch axes scales
QwtScaleDiv *scDiv = plot->axisScaleDiv(oldMainAxis);
if (axis == QwtPlot::xBottom || axis == QwtPlot::xTop)
	plot->setAxisScale(xAxis, scDiv->lBound(), scDiv->hBound());
else if (axis == QwtPlot::yLeft || color_axis == QwtPlot::yRight)
	plot->setAxisScale(yAxis, scDiv->lBound(), scDiv->hBound());

colorAxis = plot->axisWidget(color_axis);
plot->setAxisScale(color_axis, data().range().minValue(), data().range().maxValue());
colorAxis->setColorBarEnabled(on);
colorAxis->setColorMap(data().range(), colorMap());
if (!plot->axisEnabled(color_axis))
	plot->enableAxis(color_axis);
colorAxis->show();
plot->updateLayout();
}

int Spectrogram::colorBarWidth()
{
QwtPlot *plot = this->plot();
if (!plot)
	return 0;

QwtScaleWidget *colorAxis = plot->axisWidget(color_axis);
return colorAxis->colorBarWidth();
}

void Spectrogram::setColorBarWidth(int width)
{
QwtPlot *plot = this->plot();
if (!plot)
	return;

QwtScaleWidget *colorAxis = plot->axisWidget(color_axis);
colorAxis->setColorBarWidth(width);
}

Spectrogram* Spectrogram::copy()
{
Spectrogram *new_s = new Spectrogram(matrix());
new_s->setDisplayMode(QwtPlotSpectrogram::ImageMode, testDisplayMode(QwtPlotSpectrogram::ImageMode));
new_s->setDisplayMode(QwtPlotSpectrogram::ContourMode, testDisplayMode(QwtPlotSpectrogram::ContourMode));
new_s->setColorMap (colorMap());
new_s->setAxis(xAxis(), yAxis());
new_s->setDefaultContourPen(defaultContourPen());
new_s->setLevelsNumber(levels());
return new_s;
}

void Spectrogram::setGrayScale()
{
setColorMap(QwtLinearColorMap(Qt::black, Qt::white));
color_map = GrayScale;

QwtPlot *plot = this->plot();
if (!plot)
	return;

QwtScaleWidget *colorAxis = plot->axisWidget(color_axis);
if (colorAxis)
	colorAxis->setColorMap(data().range(), colorMap());
}

void Spectrogram::setDefaultColorMap()
{
QwtLinearColorMap colorMap(Qt::blue, Qt::red);
colorMap.addColorStop(0.25, Qt::cyan);
colorMap.addColorStop(0.5, Qt::green);
colorMap.addColorStop(0.75, Qt::yellow);
setColorMap(colorMap);

color_map = Default;

QwtPlot *plot = this->plot();
if (!plot)
	return;

QwtScaleWidget *colorAxis = plot->axisWidget(color_axis);
if (colorAxis)
	colorAxis->setColorMap(this->data().range(), this->colorMap());
}

double MatrixData::value(double x, double y) const
{       
int i = abs((int)floor((y_end - y)/dy - 1));
int j = abs((int)floor((x - x_start)/dx - 1));

if (d_m && i < n_rows && j < n_cols)
	return d_m[i][j];
else 
	return 0.0;
}

/*double FuntionData::value(double x, double y) const
{       
myParser parser;	
//parser.SetExpr(d_matrix->formula().ascii());
parser.SetExpr("sin(i*j)");
parser.DefineVar("i", &x);
parser.DefineVar("j", &y);
return parser.Eval();
}*/