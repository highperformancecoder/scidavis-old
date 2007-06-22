#include "Scripting.h"

#include <string.h>

#ifdef SCRIPTING_MUPARSER
#include "muParserScripting.h"
#endif
#ifdef SCRIPTING_PYTHON
#include "PythonScripting.h"
#endif

ScriptingLangManager::ScriptingLang ScriptingLangManager::langs[] = {
#ifdef SCRIPTING_MUPARSER
  { muParserScripting::langName, muParserScripting::constructor },
#endif
#ifdef SCRIPTING_PYTHON
  { PythonScripting::langName, PythonScripting::constructor },
#endif
  { NULL, NULL }
};

ScriptingEnv *ScriptingLangManager::newEnv(ApplicationWindow *parent)
{
  if (!langs[0].constructor)
    return NULL;
  else
    return langs[0].constructor(parent);
}

ScriptingEnv *ScriptingLangManager::newEnv(const char *name, ApplicationWindow *parent)
{
  for (ScriptingLang *i = langs; i->constructor; i++)
    if (!strcmp(name, i->name))
      return i->constructor(parent);
  return NULL;
}

QStringList ScriptingLangManager::languages()
{
  QStringList l;
  for (ScriptingLang *i = langs; i->constructor; i++)
    l << i->name;
  return l;
}

ScriptingEnv::ScriptingEnv(ApplicationWindow *parent, const char *langName)
  : QObject(0, langName), Parent(parent)
{
  initialized=false;
  refcount=0;
}

void ScriptingEnv::incref()
{
  refcount++;
}

void ScriptingEnv::decref()
{
  refcount--;
  if (refcount==0)
    delete this;
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
