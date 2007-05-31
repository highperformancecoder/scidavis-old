/***************************************************************************
    File                 : StatisticsFilter.cpp
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Knut Franke
    Email (use @ for *)  : knut.franke*gmx.de
    Description          : Computes standard statistics on any number of inputs.

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/
#include "StatisticsFilter.h"
#include <gsl/gsl_statistics.h>

StatisticsFilter::StatisticsFilter()
{
	for (int i=0; i<2; i++)
		d_strings[i] = new StringStatisticsColumn(this, (StatItem)i);
	for (int i=2; i<11; i++)
		d_doubles[i-2] = new DoubleStatisticsColumn(this, (StatItem)i);
}

StatisticsFilter::~StatisticsFilter()
{
	for (int i=0; i<2; i++)
		delete d_strings[i];
	for (int i=0; i<9; i++)
		delete d_doubles[i];
}

AbstractDataSource *StatisticsFilter::output(int port) const
{
	if (port < 0 || port >= numOutputs()) return 0;
	if (port <= 1) return d_strings[port];
	return d_doubles[port-2];
}

int StatisticsFilter::numRows() const
{
	int result = 0;
	foreach(AbstractDataSource* i, d_inputs)
		if (i) result++;
	return result;
}

void StatisticsFilter::inputDataAboutToChange(AbstractDataSource*)
{
	d_strings[1]->dataAboutToChange(d_strings[1]);
	for (int i=0; i<9; i++)
		d_doubles[i]->dataAboutToChange(d_doubles[i]);
}

void StatisticsFilter::inputDataChanged(int port)
{
	if (port >= d_s.size()) d_s.resize(port+1);
	Statistics *s = &d_s[port];
	for (s->first_valid_row=0; s->first_valid_row < d_inputs[port]->numRows(); s->first_valid_row++)
		if (!d_inputs[port]->textAt(s->first_valid_row).isEmpty()) break;
	if (s->first_valid_row == d_inputs[port]->numRows()) {
		s->first_valid_row = -1;
		s->N = 0;
		return;
	}
	for (s->last_valid_row=s->first_valid_row; s->last_valid_row < d_inputs[port]->numRows(); s->last_valid_row++)
		if (d_inputs[port]->textAt(s->last_valid_row).isEmpty()) break;
	--(s->last_valid_row);
	s->N = s->last_valid_row - s->first_valid_row + 1;
	double *data = new double[s->N];
	s->min_index = s->max_index = s->first_valid_row;
	data[0] = s->min = s->max = d_inputs[port]->valueAt(s->first_valid_row);
	for (int i = 1; i < s->N; i++) {
		data[i] = d_inputs[port]->valueAt(s->first_valid_row+i);
		if (data[i] < s->min) {
			s->min = data[i];
			s->min_index = s->first_valid_row + i;
		} else if (data[i] > s->max) {
			s->max = data[i];
			s->max_index = s->first_valid_row + i;
		}
	}
	s->mean = gsl_stats_mean(data, 1, s->N);
	s->sigma = gsl_stats_sd(data, 1, s->N);
	s->variance = gsl_stats_variance(data, 1, s->N);
	delete[] data;
	d_strings[1]->dataChanged(d_strings[1]);
	for (int i=0; i<9; i++)
		d_doubles[i]->dataChanged(d_doubles[i]);
}

double StatisticsFilter::DoubleStatisticsColumn::valueAt(int row) const
{
	if (row<0 || row>=d_parent->numRows()) return 0;
	const Statistics *s = &(d_parent->d_s.at(row));
	switch(d_item) {
		case Mean: return s->mean;
		case Sigma: return s->sigma;
		case Variance: return s->variance;
		case Sum: return s->mean * s->N;
		case iMax: return s->max_index + 1;
		case Max: return s->max;
		case iMin: return s->min_index + 1;
		case Min: return s->min;
		case N: return s->N;
		default: return 0;
	}
}

QString StatisticsFilter::DoubleStatisticsColumn::label() const
{ 
	switch(d_item) {
		case Mean: return tr("Mean");
		case Sigma: return tr("StandardDev");
		case Variance: return tr("Variance");
		case Sum: return tr("Sum");
		case iMax: return tr("iMax");
		case Max: return tr("Max");
		case iMin: return tr("iMin");
		case Min: return tr("Min");
		case N: return tr("N");
		default: return QString();
	}
}

QString StatisticsFilter::StringStatisticsColumn::textAt(int row) const
{
	const Statistics *s = &(d_parent->d_s.at(row));
	switch(d_item) {
		case Label:
			return d_parent->d_inputs.value(row) ?
				d_parent->d_inputs[row]->label() :
				QString();
		case Rows: return QString("[%1:%2]").arg(s->first_valid_row + 1).arg(s->last_valid_row+1);
		default: return QString();
	}
}

QString StatisticsFilter::StringStatisticsColumn::label() const
{
	switch(d_item) {
		case Label: return tr("Name");
		case Rows: return tr("Rows");
		default: return QString();
	}
}

