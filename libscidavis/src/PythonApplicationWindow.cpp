/***************************************************************************
	File                 : PythonScripting.cpp
	Project              : SciDAVis
--------------------------------------------------------------------
	Copyright            : (C) 2019 by Russell Standish

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
// run classdesc descriptors for the Application window class
#include "PythonClassdesc.h"
#include "ApplicationWindow.h"
#include "ApplicationWindow.cd"
#include "ArrowMarker.h"
#include "ArrowMarker.cd"
#include "CurveRangeDialog.h"
#include "CurveRangeDialog.cd"
#include "AbstractAspect.cd"
#include "AbstractColumn.cd"
#include "Column.cd"
#include "Folder.h"
#include "Folder.cd"
#include "globals.h"
#include "globals.cd"
#include "Graph.h"
#include "Graph.cd"
#include "Graph3D.h"
#include "Graph3D.cd"
#include "Grid.h"
#include "Grid.cd"
#include "ImageMarker.h"
#include "ImageMarker.cd"
#include "future/lib/Interval.h"
#include "Interval.cd"
#include "future/lib/IntervalAttribute.h"
#include "IntervalAttribute.cd"
#include "Legend.h"
#include "Legend.cd"
#include "Matrix.h"
#include "Matrix.cd"
#include "MatrixView.cd"
#include "MultiLayer.h"
#include "MultiLayer.cd"
#include "MyWidget.h"
#include "MyWidget.cd"
#include "Note.h"
#include "Note.cd"
#include "PlotEnrichement.cd"
#include "Qt.h" 
#include "Qt.cd" 
#include "QtEnums.h"
#include "QtEnums.cd"
#include "QwtSymbol.cd"
#include "QwtErrorPlotCurve.h"
#include "Script.cd"
#include "ScriptingEnv.h"
#include "ScriptingEnv.cd"
#include "Table.h"
#include "Table.cd"
#include "TableView.cd"
#include <QDockWidget>
#include <classdesc_epilogue.h>

#include "PythonExtras.h"

#include <QTranslator>
#include <QToolBar>


void exposeApplicationWindow(classdesc::python_t& p)
{
  // this needs to be done first, otherwise SciDAVis::ColumnMode will
  // not be exposed
  p.defineClass<SciDAVis>();
  p.defineClass<ApplicationWindow>();
}
