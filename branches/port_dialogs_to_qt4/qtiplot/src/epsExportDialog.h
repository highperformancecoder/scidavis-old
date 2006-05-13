#ifndef EPSEXPORTDIALOG_H
#define EPSEXPORTDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
#include <qprinter.h>

class Q3ButtonGroup;
class QPushButton;
class QCheckBox;
class QSpinBox;
class QComboBox;
	
class epsExportDialog : public QDialog
{
    Q_OBJECT

public:
    epsExportDialog(const QString& fileName, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~epsExportDialog();

    QPushButton* buttonOk;
	QPushButton* buttonCancel;
    Q3ButtonGroup* GroupBox1, *GroupBox2;
    QCheckBox* boxColor;
	QSpinBox * boxResolution;
	QComboBox *boxPageSize, *boxOrientation;
	
public slots:
	QPrinter::PageSize pageSize();

protected slots:
    virtual void languageChange();
	void accept();

signals:
	void exportToEPS(const QString&, int, QPrinter::Orientation, QPrinter::PageSize, QPrinter::ColorMode);

private:
	QString f_name;
};

#endif // imageExportDialog_H
