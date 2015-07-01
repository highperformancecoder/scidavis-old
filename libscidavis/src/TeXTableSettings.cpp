/**************************************************************************
    File                 : TeXTableSettings.cpp
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2010 by Iurie Nistor
    Email (use @ for *)  : nistor*iurie.org
    Description          : TeX table settings

 *************************************************************************/

/**************************************************************************
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; either version 3 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *   You should have received a copy of the GNU General Public License    *
 *   along with this program; if not, write to the Free Software          *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                   *
 *   Boston, MA  02110-1301  USA                                          *
 *                                                                        *
 *************************************************************************/

#include "TeXTableSettings.h"

TeXTableSettings::TeXTableSettings(void)
{    
    table_caption = false;
    table_labels = false;
    columns_alignment = ALIGN_CENTER;
}

void TeXTableSettings::set_with_caption(bool b)
{
    table_caption = b;
}

void TeXTableSettings::set_with_labels(bool b)
{
    table_labels = b;
}

void TeXTableSettings::set_columnsAlignment (enum ColumAlignment alignment)
{
    columns_alignment = alignment;
}

bool TeXTableSettings::with_caption( void )
{
    return table_caption;
}

bool TeXTableSettings::with_labels(void)
{
    return table_labels;
}

enum ColumAlignment columnsAlignment(void)
{
    return columns_alignment;
}

