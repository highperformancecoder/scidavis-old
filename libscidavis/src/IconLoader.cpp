/***************************************************************************
    File                 : iconloader.cpp
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2016 by Arun Narayanankutty
    Email (use @ for *)  : n.arun.lifescience@gmail.com
    Description          : Load icons 
                           
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

/* TODO: 
   1) use diffrent size icons 16x16, 22x22, 32x32 etc..
   2) use svg icons insted of xpm 
   3) use freedesktop icon naming when possible */

#include "IconLoader.h"

#include <QFile>
#include <QtDebug>

QIcon IconLoader::load(const QString& name) {

  QIcon ret;
  // If the icon name is empty
  if (name.isEmpty()) {
    qDebug() << "Icon name is null";
    return ret;
  }

  // comment out this for now as we dont
  // want to load icons from system theme
/*#if QT_VERSION >= 0x040600
  ret = QIcon::fromTheme(name);
  if (!ret.isNull()) return ret;
#endif*/

  const QString filename(":" + name + ".xpm");
  if (QFile::exists(filename)) {
    ret.addFile(filename);
  }

  if (ret.isNull()){
    qDebug() << "Couldn't load icon" << name;
  }
  return ret;
}
