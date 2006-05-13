#ifndef MYFILESDIALOGS_H
#define MYFILESDIALOGS_H

#include <q3filedialog.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qimage.h>
#include <QPicture>

class ImportFilesDialog: public Q3FileDialog
{
public:
	QComboBox* importType;

	ImportFilesDialog(bool importTypeEnabled, QWidget*, const char* )
		{
		setCaption(tr("QtiPlot - Import Multiple ASCII Files"));

		QString filter="All files *;;Text (*.TXT *.txt);;Data (*DAT *.dat);;";
		setFilters( filter );

		setMode( Q3FileDialog::ExistingFiles );

		if (importTypeEnabled)
			{
			QLabel* label = new QLabel( "Import each file as: ", this );
        
			importType = new QComboBox( this );
			importType->insertItem(tr("New Table"));
			importType->insertItem(tr("New Columns"));
			importType->insertItem(tr("New Rows"));

			addWidgets( label, importType, 0 );
			}
		};

	int importFileAs(){return importType->currentItem();};

//	~ImportFilesDialog(){};
};

class ImageExportDialog: public Q3FileDialog
{
public:
	QCheckBox* boxOptions;

	ImageExportDialog(QWidget*, const char* )
		{
		setCaption( tr( "QtiPlot - Choose a filename to save under" ) );

		QStringList list=QPicture::outputFormatList ();
		list<<"EPS";
		//list<<"WMF";
		list.sort();
		
		QString filter, selectedFilter,aux;			
		for (int i=0;i<(int)list.count();i++)
			{
			aux="*."+(list[i]).lower()+";;";
			filter+=aux;
			}
		setFilters( filter );
		setMode( Q3FileDialog::AnyFile );
	
		boxOptions = new QCheckBox(this, "boxOptions" );
		boxOptions->setText( "Show export &options" );
		#ifdef Q_OS_WIN // Windows systems
			boxOptions->setChecked( true );			
		#else
			boxOptions->setChecked( false );
		#endif

		addWidgets( 0, boxOptions, 0 );
		};

	bool showExportOptions(){return boxOptions->isChecked();};
};

#endif
