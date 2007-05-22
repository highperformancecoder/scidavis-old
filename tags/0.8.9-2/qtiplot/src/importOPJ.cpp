#include "importOPJ.h"
#include <OPJFile.h>

#include <qregexp.h>

ImportOPJ::ImportOPJ(ApplicationWindow *app, const QString& filename) :
		mw(app)
{	
OPJFile opj((const char *)filename.latin1());
parse_error = opj.Parse();
importTables(opj);
}

bool ImportOPJ::importTables(OPJFile opj) 
{
for (int s=0; s<opj.numSpreads(); s++) 
	{	
	int nr_cols = opj.numCols(s);
	int maxrows = opj.maxRows(s);

	Table *table = mw->newTable(opj.spreadName(s), maxrows, nr_cols);
	if (!table)
		return false;
	
	for (int j=0; j<nr_cols; j++) 
		{
		QString name(opj.colName(s,j));
		table->setColName(j, name.replace(QRegExp(".*_"),""));

		if (QString(opj.colType(s,j)) == "X")
			table->setColPlotDesignation(j, Table::X);
		else if (QString(opj.colType(s,j)) == "Y")
			table->setColPlotDesignation(j, Table::Y);
		else if (QString(opj.colType(s,j)) == "Z")
			table->setColPlotDesignation(j, Table::Z);
		else
			table->setColPlotDesignation(j, Table::None);

		for (int i=0; i<opj.numRows(s,j); i++) 
			{
			if(strcmp(opj.colType(s,j),"LABEL")) 
				{// number
				double val = opj.Data(s,j)[i];
				if(fabs(val)<2.0e-300)// empty entry
					continue;
				table->setText(i, j, QString::number(val));
				}
			else// label? doesn't seem to work
				table->setText(i, j, QString(opj.SData(s,j,i)));
			}		
		}
	table->showNormal();

	//cascade the tables
	int dx=table->verticalHeaderWidth();
	int dy=table->parentWidget()->frameGeometry().height() - table->height();
	table->parentWidget()->move(QPoint(s*dx,s*dy));
	}

//TO DO: import matrices
return true;
}
