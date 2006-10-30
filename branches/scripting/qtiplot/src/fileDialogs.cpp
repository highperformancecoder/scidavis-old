#include "fileDialogs.h"

/*****************************************************************************
 *
 * Class ImportFilesDialog
 *
 *****************************************************************************/

ImportFilesDialog::ImportFilesDialog(bool importTypeEnabled, QWidget* parent, const char*name):
		QFileDialog(parent, name)
{
setCaption(tr("QtiPlot - Import Multiple ASCII Files"));

QString filter=tr("All files") + " *;;" + tr("Text") + " (*.TXT *.txt);;" +
			   tr("Data")+" (*DAT *.dat);;" + tr("Comma Separated Values") + " (*.CSV *.csv);;";
setFilters( filter );
setMode( QFileDialog::ExistingFiles );

if (importTypeEnabled)
	{
	QLabel* label = new QLabel( tr("Import each file as") + ": ", this );
        
	importType = new QComboBox( this );
	importType->insertItem(tr("New Table"));
	importType->insertItem(tr("New Columns"));
	importType->insertItem(tr("New Rows"));

	addWidgets( label, importType, 0 );
	}
}

/*****************************************************************************
 *
 * Class ImageExportDialog
 *
 *****************************************************************************/

ImageExportDialog::ImageExportDialog(QWidget* parent, const char*name):
		QFileDialog(parent, name)
{
setCaption( tr( "QtiPlot - Choose a filename to save under" ) );

QStringList list=QImage::outputFormatList ();
#ifndef Q_OS_WIN
list<<"EPS";
#endif
//list<<"WMF";
list<<"SVG";
list.sort();
		
QString filter, selectedFilter,aux;			
for (int i=0;i<(int)list.count();i++)
	{
	aux="*."+(list[i]).lower()+";;";
	filter+=aux;
	}
setFilters( filter );
setMode( QFileDialog::AnyFile );
	
boxOptions = new QCheckBox(this, "boxOptions" );
boxOptions->setText( tr("Show export &options") );
#ifdef Q_OS_WIN // Windows systems
	boxOptions->setChecked( true );			
#else
	boxOptions->setChecked( false );
#endif

addWidgets( 0, boxOptions, 0 );

connect(this, SIGNAL(filterSelected ( const QString & )), 
		this, SLOT(showOptionsBox ( const QString & )));
}

void ImageExportDialog::showOptionsBox ( const QString & filter)
{
if (filter.contains("svg"))
	boxOptions->hide();
else
	boxOptions->show();
}

