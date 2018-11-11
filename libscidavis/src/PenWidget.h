/***************************************************************************
    File                 : PenWidget.h
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
#ifndef PENWIDGET_H
#define PENWIDGET_H

#include <QPen>
#include <QWidget>
#include <QPixmap>

class QApplication;
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QValidator;
class ColorButton;

static const QSize DefaultSampleSize(24, 24), DefaultLineStyleSampleSize(92,4);

QPixmap penCapSample(const Qt::PenCapStyle capStyle, const QPen &pen, const QSize &size=DefaultSampleSize);
QPixmap penJoinSample(const Qt::PenJoinStyle joinStyle, const QPen &pen, const QSize &size=DefaultSampleSize);

class PenWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PenWidget(QWidget *parent=0, QPen pen=QPen());

    QPen pen() const { return m_pen; }

signals:
    void penChanged(const QPen &pen);

public slots:
    void setPen(const QPen &pen);

private slots:
    void updateColor(QColor);
    void updateWidth(QString);
    void updateLineStyle(int);
    void updateCapStyle(int);
    void updateJoinStyle(int);

    void toggleDashPattern(int);
    void customDashCheck(const QString &);
    QVector<qreal> dashPattern();
    void setDashPattern(QString s){d_custom_dash = s;};
    void setDashPattern(QVector<qreal> dp) {d_custom_dash = dashPatternToString(dp);};
    QString dashPatternToString(QVector<qreal>);
    QPixmap penStyleSample(const Qt::PenStyle, const QPen &, const QSize &size=DefaultLineStyleSampleSize);
    void updateCustomDash();

private:
    void createWidgets();
    void createLayout();
    void createConnections();
    void updateSamples();

    ColorButton *colorButton;
    QComboBox *widthComboBox;
    QComboBox *lineStyleComboBox;
    QComboBox *capStyleComboBox;
    QComboBox *joinStyleComboBox;

    QLabel* dashLabel;
    QLineEdit* dashLineEdit;
    QValidator* dashValidator;

    QPen m_pen;
    QString d_custom_dash;
};

#endif // PENWIDGET_H
