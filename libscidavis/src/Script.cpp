/***************************************************************************
    File                 : Script.cpp
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Knut Franke
    Email (use @ for *)  : knut.franke*gmx.de
    Description          : Implementations of generic scripting classes

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
#include "ScriptingEnv.h"
#include "Script.h"

#include <string.h>

#ifdef SCRIPTING_MUPARSER
#include "MuParserScript.h"
#include "MuParserScripting.h"
#endif
#ifdef SCRIPTING_PYTHON
#include "PythonScript.h"
#include "PythonScripting.h"
#endif

namespace {
const char *langs[] = {
#ifdef SCRIPTING_MUPARSER
    MuParserScripting::langName,
#endif
#ifdef SCRIPTING_PYTHON
    PythonScripting::langName,
#endif
    nullptr
};
}

ScriptingEnv *ScriptingLangManager::newEnv(ApplicationWindow *parent)
{
    if (!langs[0])
        return nullptr;
    else
        return newEnv(langs[0], parent);
}

ScriptingEnv *ScriptingLangManager::newEnv(const std::string &name, ApplicationWindow *parent,
                                           bool
#ifdef SCRIPTING_PYTHON // avoids unused warning
                                                   batch
#endif
)
{
#ifdef SCRIPTING_MUPARSER
    if (name == MuParserScripting::langName)
        return new MuParserScripting(parent);
#endif
#ifdef SCRIPTING_PYTHON
    if (name == PythonScripting::langName)
        return new PythonScripting(parent, batch);
#endif
    return nullptr;
}

QStringList ScriptingLangManager::languages()
{
    QStringList l;
    for (auto i = langs; *i; i++)
        l << *i;
    return l;
}

bool Script::compile(bool)
{
    emit_error("Script::compile called!", 0);
    return false;
}

QVariant Script::eval()
{
    emit_error("Script::eval called!", 0);
    return QVariant();
}

bool Script::exec()
{
    emit_error("Script::exec called!", 0);
    return false;
}

scripted::scripted(ScriptingEnv *env)
{
    env->incref();
    scriptEnv = env;
}

scripted::~scripted()
{
    scriptEnv->decref();
}

void scripted::scriptingChangeEvent(ScriptingChangeEvent *sce)
{
    scriptEnv->decref();
    sce->scriptingEnv()->incref();
    scriptEnv = sce->scriptingEnv();
}
