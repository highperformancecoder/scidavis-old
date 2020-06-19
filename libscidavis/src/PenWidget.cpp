/***************************************************************************
    File                 : PenWidget.cpp
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2018 by Miquel Garriga
    Email (use @ for *)  : gbmiquel*gmail.com
    Description          : Pen options widget

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
/* based on https://github.com/jhj/aqp-qt5/raw/master/pagedesigner1/penwidget.cpp */

#include "PenWidget.h"
#include "ColorButton.h"

#include <QApplication>
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPixmapCache>
#include <QPushButton>
#include <QValidator>
#include <QVBoxLayout>

#include <QDebug>

PenWidget::PenWidget(QWidget *parent, QPen pen)
    : QWidget(parent)
{
    m_pen = pen;
    d_custom_dash = dashPatternToString(m_pen.dashPattern());

    createWidgets();
    createLayout();
    createConnections();
}

void PenWidget::createWidgets()
{
    // color
    colorButton = new ColorButton(this);
    colorButton->setColor(m_pen.color());
    // linewidth
    widthComboBox = new QComboBox(this);
    widthComboBox->addItem( tr( "1" ) );
    widthComboBox->addItem( tr( "2" ) );
    widthComboBox->addItem( tr( "3" ) );
    widthComboBox->addItem( tr( "4" ) );
    widthComboBox->addItem( tr( "5" ) );
    widthComboBox->setCurrentIndex(widthComboBox->findText(QString::number(m_pen.width())));

    widthComboBox->setEditable (true);
    widthComboBox->setEditText(QString::number(m_pen.width()));
    widthComboBox->setMinimumWidth(80);
    // line style
    lineStyleComboBox = new QComboBox(this);
    lineStyleComboBox->setIconSize(QSize(100,4));

    lineStyleComboBox->addItem(penStyleSample(Qt::SolidLine, m_pen), " Solid", static_cast<int>(Qt::SolidLine));
    lineStyleComboBox->addItem(penStyleSample(Qt::DashLine, m_pen), " Dash", static_cast<int>(Qt::DashLine));
    lineStyleComboBox->addItem(penStyleSample(Qt::DotLine, m_pen), " Dot", static_cast<int>(Qt::DotLine));
    lineStyleComboBox->addItem(penStyleSample(Qt::DashDotLine, m_pen), " DashDot", static_cast<int>(Qt::DashDotLine));
    lineStyleComboBox->addItem(penStyleSample(Qt::DashDotDotLine, m_pen), " DashDotDot", static_cast<int>(Qt::DashDotDotLine));
    lineStyleComboBox->addItem(penStyleSample(Qt::CustomDashLine, m_pen), " Custom", static_cast<int>(Qt::CustomDashLine));
    lineStyleComboBox->setCurrentIndex(lineStyleComboBox->findData(static_cast<int>(m_pen.style())));

    lineStyleComboBox->setMinimumWidth(80);
    // custom dash pattern
    dashLabel = new QLabel(tr("Dash pattern"), this);
    dashLineEdit = new QLineEdit(this);
    dashLineEdit->setText(d_custom_dash);
    // # https://snorfalorpagus.net/blog/2014/08/09/validating-user-input-in-pyqt4-using-qvalidator/
    QRegExp rx("^(\\d+\\s+\\d+\\b\\s*)*$");
    dashValidator = new QRegExpValidator(rx,this);
    // dashLineEdit->setValidator(validator);

    dashLineEdit->setMinimumWidth(80);
    // cap style
    capStyleComboBox = new QComboBox(this);
    capStyleComboBox->addItem(penCapSample(Qt::FlatCap, m_pen),tr("Flat"), Qt::FlatCap);
    capStyleComboBox->addItem(penCapSample(Qt::RoundCap, m_pen),tr("Round"), Qt::RoundCap);
    capStyleComboBox->addItem(penCapSample(Qt::SquareCap, m_pen),tr("Square"), Qt::SquareCap);
    capStyleComboBox->setCurrentIndex(capStyleComboBox->findData(static_cast<int>(m_pen.capStyle())));
    // join style
    joinStyleComboBox = new QComboBox(this);
    joinStyleComboBox->addItem(penJoinSample(Qt::BevelJoin, m_pen),tr("Bevel"), Qt::BevelJoin);
    joinStyleComboBox->addItem(penJoinSample(Qt::MiterJoin, m_pen),tr("Miter"), Qt::MiterJoin);
    joinStyleComboBox->addItem(penJoinSample(Qt::RoundJoin, m_pen),tr("Round"), Qt::RoundJoin);
    joinStyleComboBox->setCurrentIndex(joinStyleComboBox->findData(static_cast<int>(m_pen.joinStyle())));
}

void PenWidget::createLayout()
{
    QGridLayout *gl1 = new QGridLayout(this);
    gl1->setColumnMinimumWidth(0,90);
    gl1->addWidget(new QLabel(tr("Color")), 0, 0);
    gl1->addWidget(colorButton, 0, 1, Qt::AlignLeft);
    gl1->setColumnStretch(1,1);

    gl1->addWidget(new QLabel(tr("Line type")), 1, 0);
    gl1->addWidget(lineStyleComboBox, 1, 1);

    gl1->addWidget(dashLabel, 2, 0);
    gl1->addWidget(dashLineEdit, 2, 1);
    lineStyleComboBox->setCurrentIndex(m_pen.style()-1);
    toggleDashPattern(lineStyleComboBox->currentIndex());

    gl1->addWidget(new QLabel(tr("Line width")), 3, 0);
    gl1->addWidget(widthComboBox, 3, 1);

    gl1->addWidget(new QLabel(tr("Cap style")), 4, 0);
    gl1->addWidget(capStyleComboBox, 4, 1);

    gl1->addWidget(new QLabel(tr("Join style")), 5, 0);
    gl1->addWidget(joinStyleComboBox, 5, 1);

    setLayout(gl1);
}

void PenWidget::createConnections()
{
    connect(colorButton, SIGNAL(changed(QColor)), this, SLOT(updateColor(QColor)));
    connect(widthComboBox, SIGNAL(editTextChanged(QString)), this, SLOT(updateWidth(QString)));
#if QT_VERSION >= 0x050000
    connect(widthComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(updateWidth(QString)));
#endif
    connect(lineStyleComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateLineStyle(int)));
    connect(capStyleComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateCapStyle(int)));
    connect(joinStyleComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateJoinStyle(int)));

    connect(lineStyleComboBox, SIGNAL( activated(int) ), this, SLOT(toggleDashPattern(int)));
    connect(dashLineEdit,SIGNAL(textEdited(const QString &)), this, SLOT(customDashCheck(const QString &)));
    connect(dashLineEdit,SIGNAL(editingFinished()), this, SLOT(updateCustomDash()));
}

void PenWidget::setPen(const QPen &pen)
{
    if (m_pen != pen) {
        m_pen = pen;
        d_custom_dash = dashPatternToString(m_pen.dashPattern());
        colorButton->setColor(m_pen.color());
        lineStyleComboBox->setCurrentIndex(lineStyleComboBox->findData(static_cast<int>(m_pen.style())));
        dashLineEdit->setText(d_custom_dash);
        if (widthComboBox->findText(QString::number(m_pen.width()))>=0)
            widthComboBox->setCurrentIndex(widthComboBox->findText(QString::number(m_pen.width())));
        widthComboBox->setEditText(QString::number(m_pen.width()));
        capStyleComboBox->setCurrentIndex(capStyleComboBox->findData(static_cast<int>(m_pen.capStyle())));
        joinStyleComboBox->setCurrentIndex(joinStyleComboBox->findData(static_cast<int>(m_pen.joinStyle())));
    }
}


void PenWidget::updateColor(QColor color)
{
    m_pen.setColor(color);
    updateSamples();
    emit penChanged(m_pen);
}

void PenWidget::updateWidth(QString value)
{
    m_pen.setWidth(value.toInt());
    emit penChanged(m_pen);
}

void PenWidget::updateLineStyle(int index)
{
    m_pen.setStyle(static_cast<Qt::PenStyle>(lineStyleComboBox->itemData(index).toInt()));
    toggleDashPattern(index);
    emit penChanged(m_pen);
}

void PenWidget::updateCapStyle(int index)
{
    m_pen.setCapStyle(static_cast<Qt::PenCapStyle>(capStyleComboBox->itemData(index).toInt()));
    emit penChanged(m_pen);
}

void PenWidget::updateJoinStyle(int index)
{
    m_pen.setJoinStyle(static_cast<Qt::PenJoinStyle>(joinStyleComboBox->itemData(index).toInt()));
    emit penChanged(m_pen);
}

void PenWidget::updateSamples()
{
    for (int i = 0; i < lineStyleComboBox->count(); ++i)
        lineStyleComboBox->setItemIcon(i, penStyleSample(static_cast<Qt::PenStyle>(lineStyleComboBox->itemData(i).toInt()), m_pen));
    for (int i = 0; i < capStyleComboBox->count(); ++i)
        capStyleComboBox->setItemIcon(i, penCapSample(static_cast<Qt::PenCapStyle>(capStyleComboBox->itemData(i).toInt()), m_pen));
    for (int i = 0; i < joinStyleComboBox->count(); ++i)
        joinStyleComboBox->setItemIcon(i, penJoinSample(static_cast<Qt::PenJoinStyle>(joinStyleComboBox->itemData(i).toInt()), m_pen));
}

QPixmap PenWidget::penStyleSample(const Qt::PenStyle style, const QPen &pen, const QSize &size)
{
    QString key = QString("PENSTYLESAMPLE:%1:%2:%3x%4:%5").arg(static_cast<int>(style)).arg(pen.color().name()).arg(size.width()).arg(size.height()).arg(d_custom_dash);
    QPixmap pixmap(size);
    if (!QPixmapCache::find(key, &pixmap)) {
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        QPen sample_pen(pen);
        sample_pen.setStyle(style);
        if (style == Qt::CustomDashLine)
            sample_pen.setDashPattern(pen.dashPattern());
        sample_pen.setWidth(4);
        sample_pen.setCapStyle(Qt::FlatCap);
        painter.setPen(sample_pen);
        const int Y = size.height() / 2;
        painter.drawLine(0, Y, size.width(), Y);
        painter.end();
        QPixmapCache::insert(key, pixmap);
    }
    return pixmap;
}

QPixmap penCapSample(const Qt::PenCapStyle capStyle, const QPen &pen, const QSize &size)
{
    QString key = QString("PENCAPSTYLESAMPLE:%1:%2:%3x%4").arg(static_cast<int>(capStyle)).arg(pen.color().name()).arg(size.width()).arg(size.height());
    QPixmap pixmap(size);
    if (!QPixmapCache::find(key, &pixmap)) {
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        QPen sample_pen;
        sample_pen.setCapStyle(Qt::SquareCap);
        sample_pen.setWidthF(size.height() / 2.5);
        sample_pen.setColor(Qt::white);
        painter.setPen(sample_pen);
        const int Y = size.height() / 2;
        painter.drawLine(0, Y, size.width(), Y);
        sample_pen.setColor(pen.color());
        sample_pen.setCapStyle(capStyle);
        painter.setPen(sample_pen);
        painter.drawLine(size.width() / 2.5, Y, size.width(), Y);
        painter.end();
        QPixmapCache::insert(key, pixmap);
    }
    return pixmap;
}

QPixmap penJoinSample(const Qt::PenJoinStyle joinStyle, const QPen &pen, const QSize &size)
{
    QString key = QString("PENJOINSTYLESAMPLE:%1:%2:%3x%4").arg(static_cast<int>(joinStyle)).arg(pen.color().name()).arg(size.width()).arg(size.height());
    QPixmap pixmap(size);
    if (!QPixmapCache::find(key, &pixmap)) {
        const double Indent = size.width() / 5.0;
        QPolygonF polygon;
        polygon << QPointF(Indent, Indent)
                << QPointF(Indent, size.height() - Indent)
                << QPointF(size.width() - Indent,
                           size.height() - Indent);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        QPen sample_pen;
        sample_pen.setJoinStyle(joinStyle);
        sample_pen.setColor(pen.color());
        sample_pen.setWidthF(size.height() / 2.5);
        painter.setPen(sample_pen);
        painter.drawPolyline(polygon);
        painter.end();
        QPixmapCache::insert(key, pixmap);
    }
    return pixmap;
}

void PenWidget::toggleDashPattern(int penstyle)
{
    if (penstyle == 5)
    {
        dashLabel->show();
        dashLineEdit->show();
        dashLineEdit->setFocus();
        setTabOrder(lineStyleComboBox, dashLineEdit);
        setTabOrder(dashLineEdit, widthComboBox);
        customDashCheck(dashLineEdit->text());
    } else {
        dashLabel->hide();
        dashLineEdit->hide();
    }
}

void PenWidget::customDashCheck(const QString & s)
{
    QString text = s;
    int i=0;
    QValidator::State status = dashValidator->validate(text,i);
    switch (status)
    {
    case QValidator::Invalid:
        dashLineEdit->setStyleSheet("QLineEdit { background-color: #f6989d }");
        break;
    case QValidator::Intermediate:
        dashLineEdit->setStyleSheet("QLineEdit { background-color: #fff79a }");
        break;
    case QValidator::Acceptable:
        dashLineEdit->setStyleSheet("QLineEdit { background-color: #c4df9b }");
        break;
    }
    return;
}

void PenWidget::updateCustomDash()
{
    QString text = dashLineEdit->text();
    int i=0;
    QValidator::State status = dashValidator->validate(text,i);
    if (status == QValidator::Acceptable)
    {
        d_custom_dash = text;
        m_pen.setDashPattern(dashPattern());
        // update icon in combobox
        lineStyleComboBox->setItemIcon(5,penStyleSample(Qt::CustomDashLine, m_pen));
    } else {
        dashLineEdit->setFocus();
    }
}

QVector<qreal> PenWidget::dashPattern()
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QStringList custDash = d_custom_dash.split(' ', Qt::SkipEmptyParts);
    QVector<qreal> custDashF;
    for (auto s: d_custom_dash.split(' ', Qt::SkipEmptyParts))
#else
    QStringList custDash = d_custom_dash.split(' ', QString::SkipEmptyParts);
    QVector<qreal> custDashF;
    for (auto s: d_custom_dash.split(' ', QString::SkipEmptyParts))
#endif
        custDashF << s.toDouble();
    return custDashF;
}

QString PenWidget::dashPatternToString(QVector<qreal> dashP)
{
    QStringList dashPatternString;
    for (auto v: dashP)
        dashPatternString << QString::number(v,'f',0);
    return dashPatternString.join(" ");
}
