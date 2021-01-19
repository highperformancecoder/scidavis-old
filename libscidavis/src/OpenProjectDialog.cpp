/***************************************************************************
    File                 : OpenProjectDialog.cpp
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Knut Franke, Ion Vasilief
    Email (use @ for *)  : knut.franke*gmx.de, ion_vasilief*yahoo.fr
    Description          : Dialog for opening project files.

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

#include "OpenProjectDialog.h"
#include "ApplicationWindow.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCloseEvent>
#include <QTextCodec>

OpenProjectDialog::OpenProjectDialog(QWidget *parent, bool extended, Qt::WindowFlags flags)
    : ExtensibleFileDialog(parent, extended, flags)
{
    setWindowTitle(tr("Open Project"));
    setFileMode(ExistingFile);
    QStringList filters;
    filters << tr("SciDAVis project") + " (*.sciprj)"
            << tr("Compressed SciDAVis project") + " (*.sciprj.gz)"
            << tr("QtiPlot project") + " (*.qti)"
            << tr("Compressed QtiPlot project") + " (*.qti.gz)"
#ifdef ORIGIN_IMPORT
            << tr("Origin project") + " (*.opj *.OPJ)" << tr("Origin matrix") + " (*.ogm *.OGM)"
            << tr("Origin worksheet") + " (*.ogw *.OGW)" << tr("Origin graph") + " (*.ogg *.OGG)"
            << tr("Origin 3.5 project") + " (*.org *.ORG)"
#endif
            << tr("Backup files") + " (*.sciprj~)"
            //<< tr("Python Source") + " (*.py *.PY)"
            << tr("All files") + " (*)";
    setNameFilters(filters);

    d_advanced_options = new QWidget();
    QVBoxLayout *advanced_layout = new QVBoxLayout();
    d_advanced_options->setLayout(advanced_layout);

    // Open As options
    QHBoxLayout *openAs_layout = new QHBoxLayout();
    openAs_layout->addWidget(new QLabel(tr("Open As")));
    d_open_mode = new QComboBox();
    // Important: Keep this is sync with enum OpenMode.
    d_open_mode->addItem(tr("New Project Window"));
    d_open_mode->addItem(tr("New Folder"));
    openAs_layout->addWidget(d_open_mode);
    advanced_layout->addLayout(openAs_layout);

    // Origin codepage options
    QHBoxLayout *codec_layout = new QHBoxLayout();
    codec_layout->addWidget(new QLabel(tr("Codepage")));
    d_open_codec = new QComboBox();
    for (const int id : QTextCodec::availableMibs())
        d_open_codec->addItem(QString::fromLocal8Bit(QTextCodec::codecForMib(id)->name()), id);
    codec_layout->addWidget(d_open_codec);
    codec_layout->setEnabled(false);
    advanced_layout->addLayout(codec_layout);

    setExtensionWidget(d_advanced_options);

#if QT_VERSION >= 0x040300
    connect(this, SIGNAL(filterSelected(const QString &)), this,
            SLOT(updateAdvancedOptions(const QString &)));
#else
    QList<QComboBox *> combo_boxes = findChildren<QComboBox *>();
    if (combo_boxes.size() >= 2)
        connect(combo_boxes[1], SIGNAL(currentIndexChanged(const QString &)), this,
                SLOT(updateAdvancedOptions(const QString &)));
#endif
    updateAdvancedOptions(selectedNameFilter());
}

void OpenProjectDialog::updateAdvancedOptions(const QString &filter)
{
    if (filter.contains("*.ogm") || filter.contains("*.ogw"))
        d_open_mode->setEnabled(false);
    else
        d_open_mode->setEnabled(true);

    if (filter.contains("*.ogm") || filter.contains("*.ogw") || filter.contains("*.opj")
        || filter.contains("*.ogg") || filter.contains("*.org"))
        d_open_codec->setEnabled(true);
    else
        d_open_codec->setEnabled(false);
}

void OpenProjectDialog::closeEvent(QCloseEvent *e)
{
    if (isExtendable()) {
        ApplicationWindow *app = (ApplicationWindow *)this->parent();
        if (app)
            app->d_extended_open_dialog = this->isExtended();
    }

    e->accept();
}

QString OpenProjectDialog::codec() const
{
    return QString::fromLocal8Bit(
            QTextCodec::codecForMib(d_open_codec->currentData().toInt())->name());
}

bool OpenProjectDialog::setCodec(const QString &codec)
{
    if ("" == codec)
        return false;
    for (const int id : QTextCodec::availableMibs())
        if (codec == QString::fromLocal8Bit(QTextCodec::codecForMib(id)->name())) {
            // search data only if codec matches
            int ind = d_open_codec->findData(id);
            if (-1 != ind) {
                d_open_codec->setCurrentIndex(ind);
                return true;
            }
        }
    return false;
}
