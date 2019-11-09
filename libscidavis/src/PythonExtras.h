/***************************************************************************
	File                 : ModDict.h
	Project              : SciDAVis
--------------------------------------------------------------------
	Copyright            : (C) 2019 Russell Standish

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

// NB do not include this file in anything processed by MOC
// Only include this file if SCRIPTING_PYTHON is true

#ifndef PYTHONEXTRAS_H
#include <boost/python.hpp>
#include <python_base.h>

inline boost::python::dict modDict(const char* mod) {
  using namespace boost::python;
  return extract<dict>(import(mod).attr("__dict__"))();
}

struct pyobject: public boost::python::object {};

namespace classdesc
{
  template <> struct tn<pyobject>
  {
    static string name() {return "boost::python::object";}
  };
}

namespace classdesc_access
{
  template <> struct access_python<pyobject>:
    public classdesc::NullDescriptor<classdesc::python_t> {};
}

#endif
