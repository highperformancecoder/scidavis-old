/***************************************************************************
    File                 : ColorButton.h
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Ion Vasilief, Tilman Benkert
    Email (use @ for *)  : ion_vasilief*yahoo.fr, thzs*gmx.net
    Description          : A button used for color selection
                           
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

#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QWidget>
class QPushButton;
class QHBoxLayout;
class QFrame;

#if QT_VERSION >= 0x050000
static inline QString COLORNAME( QColor c ) { return c.name(QColor::HexArgb); }
static inline QString COLORVALUE( QString s ) { return s; }
#else
static inline QString COLORNAME( QColor c ) { return ('#'+QString::number(c.rgba(),16)); }
static inline QString COLORVALUE( QString s ) { if ((s[0]=='#') && (s.size()==9)) return s.remove(1,2); else return s; }
#endif

//! A button used for color selection
/**
 * This button contains two widgets:
 * 1) A frame reflecting the current color
 * 2) A button showing a color wheel to select the color
 */
class ColorButton : public QWidget
{
	Q_OBJECT

public:
	//! Constructor
	ColorButton(QWidget *parent = 0);
	//! Set the color of the display part
	void setColor(const QColor& c);
	//! Get the color of the display part
	QColor color() const;
	//! Return the index for a given color
	static unsigned int colorIndex(const QColor& c);
	//! Return the color at index 'colorindex'
	static QColor color(unsigned int colorIndex);
	//! Returns TRUE if the color is included in the color box, otherwise returns FALSE.
	static bool isValidColor(const QColor& color);
	//! The number of predefined colors
	static const unsigned int colors_count;
	QSize sizeHint() const;

private:
	QPushButton *selectButton;
	QFrame *display;

signals:
	//! Signal clicked: This is emitted when the selection button is clicked
	void clicked();
	void changed(QColor);

protected:
	//! Initialize the widget (called from constructor)
	void init();
	//! Array containing the 24 predefined colors
	static const QColor colors[];

private slots:
	void pickColor();

private:
	int btn_size;
};

#endif
