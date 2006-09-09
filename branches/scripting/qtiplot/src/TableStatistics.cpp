#include "TableStatistics.h"

#include <gsl/gsl_vector.h>
#include <gsl/gsl_statistics.h>

TableStatistics::TableStatistics(ScriptingEnv *env, QWidget *parent, Table *base, Type t, QValueList<int> targets)
  : Table(env, 0, 0, "", parent, ""),
  d_base(base), d_type(t), d_targets(targets)
{
  worksheet->setReadOnly(true);
  setCaptionPolicy(myWidget::Both);
  if (d_type == row)
  {
    setName(QString(d_base->name())+"-rowstat");
    setWindowLabel(tr("Row Statistics of %1").arg(base->name()));
    resizeRows(d_targets.size());
    resizeCols(9);
    setColName(0, tr("Row"));
    setColName(1, tr("Cols"));
    setColName(2, tr("Mean"));
    setColName(3, tr("Standard Dev"));
    setColName(4, tr("Variance"));
    setColName(5, tr("Sum"));
    setColName(6, tr("Max"));
    setColName(7, tr("Min"));
    setColName(8, "N");

    for (unsigned i=0; i < d_targets.size(); i++)
      setText(i, 0, QString::number(d_targets[i]+1));
    update(d_base, QString::null);
  }
  else if (d_type == column)
  {
    setName(QString(d_base->name())+"-colstat");
    setWindowLabel(tr("Column Statistics of %1").arg(base->name()));
    resizeRows(d_targets.size());
    resizeCols(11);
    setColName(0, tr("Col"));
    setColName(1, tr("Rows"));
    setColName(2, tr("Mean"));
    setColName(3, tr("Standard Dev"));
    setColName(4, tr("Variance"));
    setColName(5, tr("Sum"));
    setColName(6, tr("iMax"));
    setColName(7, tr("Max"));
    setColName(8, tr("iMin"));
    setColName(9, tr("Min"));
    setColName(10, "N");

    for (unsigned i=0; i < d_targets.size(); i++)
    {
      setText(i, 0, d_base->colLabel(d_targets[i]));
      update(d_base, d_base->colName(d_targets[i]));
    }
  }
  int w=9*(worksheet->horizontalHeader())->sectionSize(0);
  int h;
  if (tableRows()>11)
    h=11*(worksheet->verticalHeader())->sectionSize(0);
  else
    h=(tableRows()+1)*(worksheet->verticalHeader())->sectionSize(0);
  setGeometry(50,50,w + 45, h + 45);
}

void TableStatistics::update(Table *t, const QString& colName)
{
  if (t != d_base) return;
  
  if (d_type == row)
    for (unsigned r=0; r < d_targets.size(); r++)
    {
      int cols=d_base->tableCols();
      int i = d_targets[r];
      int m = 0;
      for (int j = 0; j < cols; j++)
	if (!d_base->text(i, j).isEmpty() && d_base->columnType(j) == Numeric)
	  m++;
      if (m > 0)
      {
	double *dat = new double[m];
	gsl_vector *y = gsl_vector_alloc (m);
	int aux = 0;
	for (int j = 0; j<cols; j++)
	{
	  QString text = d_base->text(i,j);
	  if (!text.isEmpty() && d_base->columnType(j) == Numeric)
	  {					
	    double val = text.toDouble();
	    gsl_vector_set (y, aux, val);
	    dat[aux] = val;
	    aux++;
	  }
	}
	double mean = gsl_stats_mean (dat, 1, m);
	double min, max;
	gsl_vector_minmax (y, &min, &max);

	setText(r, 1, QString::number(d_base->tableCols()));
	setText(r, 2, QString::number(mean));
	setText(r, 3, QString::number(gsl_stats_sd(dat, 1, m)));
	setText(r, 4, QString::number(gsl_stats_variance(dat, 1, m)));
	setText(r, 5, QString::number(mean*m));
	setText(r, 6, QString::number(max));
	setText(r, 7, QString::number(min));
	setText(r, 8, QString::number(m));

	gsl_vector_free (y);
	delete[] dat;
      }
    }
  else if (d_type == column)
    for (unsigned c=0; c < d_targets.size(); c++)
      if (colName == QString(d_base->name())+"_"+text(c, 0))
      {
	int i = d_base->colIndex(colName);
	if (d_base->columnType(i) != Numeric) return;

	int rows = d_base->tableRows();
	int start = -1, m = 0;
	for (int j=0; j<rows; j++)
	  if (!d_base->text(j,i).isEmpty())
	  {
	    m++;
	    if (start<0) start=j;
	  }
	if (start<0) return;

	double *dat = new double[m];
	gsl_vector *y = gsl_vector_alloc (m);

	int aux = 0, min_index = start, max_index = start;
	double val = d_base->text(start, i).toDouble();
	gsl_vector_set (y, 0, val);
	dat[0] = val;
	double min = val, max = val;
	for (int j = start + 1; j<rows; j++)
	{
	  if (!d_base->text(j, i).isEmpty())
	  {
	    aux++;
	    val = d_base->text(j, i).toDouble();
	    gsl_vector_set (y, aux, val);
	    dat[aux] = val;
	    if (val < min)
	    {
	      min = val;
	      min_index = j;
	    }
	    if (val > max)
	    {
	      max = val;
	      max_index = j;
	    }
	  }
	}
	double mean=gsl_stats_mean (dat, 1, m);

	setText(c, 1, "[1:"+QString::number(rows)+"]");
	setText(c, 2, QString::number(mean));
	setText(c, 3, QString::number(gsl_stats_sd(dat, 1, m)));
	setText(c, 4, QString::number(gsl_stats_variance(dat, 1, m)));
	setText(c, 5, QString::number(mean*m));
	setText(c, 6, QString::number(max_index + 1));
	setText(c, 7, QString::number(max));
	setText(c, 8, QString::number(min_index));
	setText(c, 9, QString::number(min));
	setText(c, 10, QString::number(m));

	gsl_vector_free (y);
	delete[] dat;
      }
}

QString TableStatistics::saveToString(const QString &geometry)
{
  QString s = "<TableStatistics>\n";
  s += QString(name())+"\t";
  s += QString(d_base->name())+"\t";
  s += QString(d_type == row ? "row" : "col") + "\t";
  s += birthDate()+"\n";
  s += "Targets";
  for (QValueList<int>::iterator i=d_targets.begin(); i!=d_targets.end(); ++i)
    s += "\t" + QString::number(*i);
  s += "\n";
  s += geometry;
  s += saveHeader();
  s += saveColumnWidths();
  s += saveCommandes();
  s += saveColumnTypes();
  s += saveComments();
  s += "WindowLabel\t" + windowLabel() + "\t" + QString::number(captionPolicy()) + "\n";
  return s + "</TableStatistics>\n";
}

