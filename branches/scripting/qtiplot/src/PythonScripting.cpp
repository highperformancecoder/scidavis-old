// get rid of a compiler warning
#ifdef _POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#endif
#include <Python.h>
#include <compile.h>
#include <eval.h>
#include <frameobject.h>
#include <traceback.h>
// for Python < 2.4, uncomment the following
/*
typedef struct _traceback {
        PyObject_HEAD
        struct _traceback *tb_next;
        PyFrameObject *tb_frame;
        int tb_lasti;
        int tb_lineno;
} PyTracebackObject;
 */

#include "PythonScripting.h"
#include "sipAPIqti.h"
extern "C" void initqti();
#include "application.h"

#include <qobject.h>
#include <qstringlist.h>
#include <qvariant.h>

const char* PythonScripting::langName = "Python";

//! like str(object) in Python
//! steals reference to object if decref is true; borrows otherwise
QString PythonScripting::toString(PyObject *object, bool decref)
{
  QString ret;
  if (!object) return "";
  PyObject *repr = PyObject_Str(object);
  if (decref) Py_DECREF(object);
  if (!repr) return "";
  ret = PyString_AsString(repr);
  Py_DECREF(repr);
  return ret;
}

//! evaluates code, using argDict (borrowed reference) as local dictionary or an empty one if argDict==NULL
//! name is the filename Python uses when reporting errors
//! returns a new reference; NULL means caller has to do exception handling
PyObject *PythonScripting::eval(const QString &code, PyObject *argDict, const char *name)
{
  PyObject *args;
  if (argDict)
  {
    Py_INCREF(argDict);
    args = argDict;
  } else
    args = PyDict_New();
  PyObject *ret=NULL;
  PyObject *co = Py_CompileString(code.ascii(), name, Py_eval_input);
  if (co)
  {
    ret = PyEval_EvalCode((PyCodeObject*)co, globals, args);
    Py_DECREF(co);
  }
  Py_DECREF(args);
  return ret;
}

//! executes code, using argDict (borrowed reference) as local dictionary or an empty one if argDict==NULL
//! name is the filename Python uses when reporting errors
//! a false return value means caller has to do exception handling
bool PythonScripting::exec (const QString &code, PyObject *argDict, const char *name)
{
  PyObject *args;
  if (argDict)
  {
    Py_INCREF(argDict);
    args = argDict;
  } else
    args = PyDict_New();
  PyObject *tmp = NULL;
  PyObject *co = Py_CompileString(code.ascii(), name, Py_file_input);
  if (co)
  {
    tmp = PyEval_EvalCode((PyCodeObject*)co, globals, args);
    Py_DECREF(co);
  }
  Py_DECREF(args);
  if (!tmp) return false;
  Py_DECREF(tmp);
  return true;
}

QString PythonScripting::errorMsg()
{
  PyObject *exception=0, *value=0, *traceback=0;
  PyTracebackObject *excit=0;
  PyFrameObject *frame;
  char *fname;
  QString msg;
  if (!PyErr_Occurred()) return "";

  PyErr_Fetch(&exception, &value, &traceback);
  msg.append(toString(exception,true)).remove("exceptions.").append(": ");
  msg.append(toString(value,true));
  msg.append("\n");
  Py_DECREF(exception);
  Py_DECREF(value);

  if (traceback) {
    excit = (PyTracebackObject*)traceback;
    while (excit && (PyObject*)excit != Py_None)
    {
      frame = excit->tb_frame;
      msg.append("at ").append(PyString_AsString(frame->f_code->co_filename));
      msg.append(":").append(QString::number(excit->tb_lineno));
      if (frame->f_code->co_name && *(fname = PyString_AsString(frame->f_code->co_name)) != '?')
	msg.append(" in ").append(fname);
      msg.append("\n");
      excit = excit->tb_next;
    }
    Py_DECREF(traceback);
  }

  return msg;
}

bool PythonScripting::importModule(char *name)
{
  PyObject *mod = PyImport_ImportModule(name);
  if (!mod) return false;
  PyDict_SetItemString(globals, name, mod);
  Py_DECREF(mod);
  return true;
}

PythonScripting::PythonScripting(ApplicationWindow *parent)
: ScriptingEnv(parent, langName)
{
  PyObject *mainmod=NULL, *mathmod=NULL, *qtimod=NULL;
  if (Py_IsInitialized())
  {
    PyEval_AcquireLock();
    mainmod = PyImport_ImportModule("__main__");
    if (!mainmod)
    {
      PyErr_Print();
      PyEval_ReleaseLock();
      return;
    }
    globals = PyModule_GetDict(mainmod);
    Py_DECREF(mainmod);
  } else {
    PyEval_InitThreads ();
    Py_Initialize ();
    if (!Py_IsInitialized ())
      return;
    initqti();

    mainmod = PyImport_AddModule("__main__");
    if (!mainmod)
    {
      PyEval_ReleaseLock();
      PyErr_Print();
      return;
    }
    globals = PyModule_GetDict(mainmod);
  }

  if (!globals)
  {
    PyErr_Print();
    PyEval_ReleaseLock();
    return;
  }
  Py_INCREF(globals);
  
  if (!importModule("sys")) PyErr_Print();
  if (!importModule("qt")) PyErr_Print();
  if (!importModule("qti")) PyErr_Print();

  mathmod = PyImport_ImportModule("math");
  if (mathmod && (math = PyModule_GetDict(mathmod)))
  {
    Py_INCREF(math);
    Py_DECREF(mathmod);
    PyObject *key, *value;
    int i=0;
    while(PyDict_Next(math, &i, &key, &value))
      PyDict_SetItem(globals, key, value);
  } else {
    math = NULL;
    PyErr_Print();
  }

  qtimod = PyImport_ImportModule("qti");
  if (qtimod)
  {
    PyObject *qtiDict = PyModule_GetDict(qtimod);
    setQObject(Parent, "app", qtiDict);
    Py_DECREF(qtimod);
    if (!exec("table=qti.app.table\nplot=qti.app.plot\nnote=qti.app.note",globals))
      PyErr_Print();
  } else
    PyErr_Print();

#ifdef SCRIPTING_CONSOLE
  const char *console="class ConsoleWriter:\n\tdef write(self, stuff):\n\t\tqti.app.console.append(stuff.strip())\n\nsys.stdout=ConsoleWriter()\nsys.stderr=ConsoleWriter()\n";
  if (qtimod && !exec(console))
    PyErr_Print();
#endif

  PyEval_ReleaseLock();
  initialized=true;
}

PythonScripting::~PythonScripting()
{
  Py_XDECREF(globals);
  Py_XDECREF(math);
}

bool PythonScripting::isRunning() const
{
  return Py_IsInitialized();
}

bool PythonScripting::setQObject(const QObject *val, const char *name, PyObject *dict)
{
  if(!val) return false;
  PyObject *pyobj=NULL;
  sipWrapperType **typesTable = sipModuleAPI_qti.em_types;
  QString cN = val->className();
  cN.prepend("qti.");
  while (*typesTable != 0)
    if(!strcmp((**typesTable++).type->td_name,cN.ascii()))
    {
      pyobj=sipBuildResult(NULL,"M",val,*--typesTable);
      break;
    }
  if (!pyobj) {
    cN = val->className();
    cN.prepend("qt.");
    typesTable = sipModuleAPI_qti_qt->em_types;
    while (*typesTable != 0)
      if(!strcmp((**typesTable++).type->td_name,cN.ascii()))
      {
	pyobj=sipBuildResult(NULL,"M",val,*--typesTable);
	break;
      }
  } 
  if (!pyobj) return false;
 
  if (dict)
    PyDict_SetItemString(dict,name,pyobj);
  else
    PyDict_SetItemString(globals,name,pyobj);
  Py_DECREF(pyobj);
  return true;
}

bool PythonScripting::setInt(int val, const char *name, PyObject *dict)
{
  PyObject *pyobj = Py_BuildValue("i",val);
  if (!pyobj) return false;
  if (dict)
    PyDict_SetItemString(dict,name,pyobj);
  else
    PyDict_SetItemString(globals,name,pyobj);
  Py_DECREF(pyobj);
  return true;
}

bool PythonScripting::setDouble(double val, const char *name, PyObject *dict)
{
  PyObject *pyobj = Py_BuildValue("d",val);
  if (!pyobj) return false;
  if (dict)
    PyDict_SetItemString(dict,name,pyobj);
  else
    PyDict_SetItemString(globals,name,pyobj);
  Py_DECREF(pyobj);
  return true;
}

const QStringList PythonScripting::mathFunctions() const
{
  QStringList flist;
  PyObject *key, *value;
  int i=0;
  while(PyDict_Next(math, &i, &key, &value))
  {
    if (PyCallable_Check(value))
      flist << PyString_AsString(key);
  }
  return flist;
}

const QString PythonScripting::mathFunctionDoc(const QString &name) const
{
  PyObject *mathf = PyDict_GetItemString(math,name); // borrowed
  PyObject *pydocstr = PyObject_GetAttrString(mathf, "__doc__"); // new
  QString qdocstr = PyString_AsString(pydocstr);
  Py_XDECREF(pydocstr);
  return qdocstr;
}

PythonScript::PythonScript(PythonScripting *env, const QString &code, QObject *context, const QString &name)
  : Script(env, code, context, name)
{
  localDict = PyDict_New();
  PyCode = NULL;
}

PythonScript::~PythonScript()
{
  Py_DECREF(localDict);
  Py_XDECREF(PyCode);
}

bool PythonScript::compile()
{
  PyObject *ret = PyRun_String("def col(c,*arg):\n\tif len(arg)>0: return tt.cell(c,arg[0])\n\telse: return tt.cell(c,i)\n",Py_file_input,localDict,localDict);
  if (ret)
    Py_DECREF(ret);
  else
    PyErr_Print();
  bool success=false;
  PyObject *tmp;
  Py_XDECREF(PyCode);
  PyCode = Py_CompileString(Code.ascii(),Name,Py_eval_input);
  if (PyCode) { // code is a single expression
    success = true;
  } else { // code contains statements
    PyErr_Clear();
    PyObject *key, *value;
    int i=0;
    QString signature = "";
    while(PyDict_Next(localDict, &i, &key, &value))
      signature.append(PyString_AsString(key)).append(",");
    signature.truncate(signature.length()-1);
    QString fdef = "def __doit__("+signature+"):\n";
    fdef.append(Code);
    fdef.replace('\n',"\n\t");
    tmp = PyDict_New();
    PyCode = Py_CompileString(fdef,Name,Py_file_input);
    if (PyCode)
    {
      Py_XDECREF(PyEval_EvalCode((PyCodeObject*)PyCode, env()->globalDict(), tmp));
      Py_DECREF(PyCode);
      PyCode = PyDict_GetItemString(tmp,"__doit__");
      Py_XINCREF(PyCode);
    }
    success = PyCode != NULL;
    Py_DECREF(tmp);
  }
  if (!success)
  {
    compiled = compileErr;
    emit_error(env()->errorMsg(), 0);
  } else
    compiled = isCompiled;
  return success;
}

QVariant PythonScript::eval()
{
  if (compiled != Script::isCompiled && !compile())
    return QVariant();
  PyObject *pyret;
  PyObject *empty_tuple = PyTuple_New(0);
  if (PyCallable_Check(PyCode))
    pyret = PyObject_Call(PyCode, empty_tuple, localDict);
  else
    pyret = PyEval_EvalCode((PyCodeObject*)PyCode, env()->globalDict(), localDict);
  Py_DECREF(empty_tuple);
  if (!pyret)
  {
    emit_error(env()->errorMsg(), 0);
    return QVariant();
  }
  if (pyret == Py_None)
  {
    Py_DECREF(pyret);
    return QVariant("");
  }
  QVariant qret;
  if (PyNumber_Check(pyret))
  {
    PyObject *number = PyNumber_Int(pyret);
    if (number)
    {
      qret = QVariant((Q_LLONG)PyInt_AS_LONG(number));
      Py_DECREF(number);
      Py_DECREF(pyret);
      return qret;
    }
    number = PyNumber_Float(pyret);
    if (number)
    {
      qret = QVariant(PyFloat_AS_DOUBLE(number));
      Py_DECREF(number);
      Py_DECREF(pyret);
      return qret;
    }
  }
  if (PyBool_Check(pyret))
    if (pyret == Py_True)
    {
      Py_DECREF(pyret);
      return QVariant(true, 0);
    } else {
      Py_DECREF(pyret);
      return QVariant(false, 0);
    }
  // could handle advanced types (such as PyList->QValueList) here if needed
  PyObject *pystring = PyObject_Unicode(pyret);
  Py_DECREF(pyret);
  if (!pystring)
  {
    emit_error(env()->errorMsg(), 0);
    return QVariant();
  }
  PyObject *asUCS2 = PyUnicode_Encode(PyUnicode_AS_UNICODE(pystring), PyUnicode_GET_DATA_SIZE(pystring), "UCS-2", 0);
//  PyObject *asUTF8 = PyUnicode_EncodeUTF8(PyUnicode_AS_UNICODE(pystring), PyUnicode_GET_DATA_SIZE(pystring), 0);
  if (!asUCS2)
  {
    emit_error(env()->errorMsg(), 0);
    Py_DECREF(pystring);
    return QVariant();
  }
//  qret = QVariant(QString::fromUtf8(PyString_AS_STRING(asUTF8)));
//  Qt4: fromUcs2 -> fromRawData
  qret = QVariant(QString::fromUcs2((unsigned short*)PyString_AS_STRING(asUCS2)));
  Py_DECREF(pystring);
  Py_DECREF(asUCS2);
  return qret;
}

bool PythonScript::exec()
{
  if (compiled != Script::isCompiled && !compile())
    return false;
  PyObject *pyret;
  PyObject *empty_tuple = PyTuple_New(0);
  if (PyCallable_Check(PyCode))
    pyret = PyObject_Call(PyCode,empty_tuple,localDict);
  else
    pyret = PyEval_EvalCode((PyCodeObject*)PyCode, env()->globalDict(), localDict);
  Py_DECREF(empty_tuple);
  if (pyret) {
    Py_DECREF(pyret);
    return true;
  }
  emit_error(env()->errorMsg(), 0);
  return false;
}

bool PythonScript::setQObject(QObject *val, const char *name)
{
  if (!PyDict_Contains(localDict, PyString_FromString(name)))
    compiled = notCompiled;
  return env()->setQObject(val, name, localDict);
}

bool PythonScript::setInt(int val, const char *name)
{
  if (!PyDict_Contains(localDict, PyString_FromString(name)))
    compiled = notCompiled;
  return env()->setInt(val, name, localDict);
}

bool PythonScript::setDouble(double val, const char *name)
{
  if (!PyDict_Contains(localDict, PyString_FromString(name)))
    compiled = notCompiled;
  return env()->setDouble(val, name, localDict);
}

