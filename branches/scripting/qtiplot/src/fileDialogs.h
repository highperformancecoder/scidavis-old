#ifndef MYFILESDIALOGS_H
#define MYFILESDIALOGS_H

#include <qfiledialog.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qimage.h>
#include <qmessagebox.h>

class ImportFilesDialog: public QFileDialog
{
public:
	QComboBox* importType;

	ImportFilesDialog(bool importTypeEnabled, QWidget* parent, const char* name);
	int importFileAs(){return importType->currentItem();};
};

class ImageExportDialog: public QFileDialog
{
	Q_OBJECT

public:
	QCheckBox* boxOptions;

	ImageExportDialog(QWidget*, const char* );
	bool showExportOptions(){return boxOptions->isChecked();};

public slots:
	void showOptionsBox ( const QString & filter);
};

#endif
