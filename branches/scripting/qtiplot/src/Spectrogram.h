#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include "matrix.h"
#include <qwt_raster_data.h>
#include <qwt_plot.h>
#include <qwt_plot_spectrogram.h>

class MatrixData;

class Spectrogram: public QwtPlotSpectrogram
{
public:
	Spectrogram();
    Spectrogram(Matrix *m);

	enum ColorMapPolicy{GrayScale, Default, Custom};

	Spectrogram* copy();
	Matrix * matrix(){return d_matrix;};

	int levels(){return (int)contourLevels().size() + 1;};
	void setLevelsNumber(int levels);

	bool hasColorScale();
	int colorScaleAxis(){return color_axis;};
	void showColorScale(int axis, bool on = true);

	int colorBarWidth();
	void setColorBarWidth(int width);

	void setGrayScale();
	void setDefaultColorMap();

	void updateData(Matrix *m);

	ColorMapPolicy colorMapPolicy(){return color_map;};

protected:
	//! Pointer to the source data matrix
	Matrix *d_matrix;

	//! Axis used to display the color scale
	int color_axis;

	//! Flags
	ColorMapPolicy color_map;
};


class MatrixData: public QwtRasterData
{
public:
    MatrixData(Matrix *m):
        QwtRasterData(m->boundingRect()),
		d_matrix(m)
    {
	n_rows = d_matrix->numRows();
	n_cols = d_matrix->numCols();

	d_m = new double* [n_rows];
	for ( int l = 0; l < n_rows; ++l) 
		d_m[l] = new double [n_cols];

	for (int i = 0; i < n_rows; i++)
         for (int j = 0; j < n_cols; j++)
           d_m[i][j] = d_matrix->text(i, j).replace(",", ".").toDouble();

	m->range(&min_z, &max_z);

	x_start = d_matrix->xStart();
	dx = (d_matrix->xEnd()- x_start)/(double)n_cols;

	y_end = d_matrix->yEnd();
	dy = (y_end- d_matrix->yStart())/(double)n_rows;
    }

	~MatrixData()
	{
	for (int i = 0; i < n_rows; i++) 
		delete [] d_m[i];
	
	delete [] d_m;
	};

    virtual QwtRasterData *copy() const
    {
        return new MatrixData(d_matrix);
    }

    virtual QwtDoubleInterval range() const
    {
        return QwtDoubleInterval(min_z, max_z);
    }

	virtual QSize rasterHint (const QwtDoubleRect &) const
	{
		return QSize(n_cols, n_rows);
	}

    virtual double value(double x, double y) const;

private:
	//! Pointer to the source data matrix
	Matrix *d_matrix;

	//! Vector used to store in memory the data from the source matrix window
	double** d_m;

	//! Data size
	int n_rows, n_cols;

	//! Min and max values in the source data matrix
	double min_z, max_z;

	//! Data resolution in x(columns) and y(rows)
	double dx, dy;

	//! X axis left value in the data matrix
	double x_start;

	//! Y axis right value in the data matrix
	double y_end;
};

#endif

