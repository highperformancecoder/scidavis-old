/***************************************************************************
	File                 : PythonScripting.cpp
	Project              : SciDAVis
--------------------------------------------------------------------
	Copyright            : (C) 2006 by Knut Franke
	Email (use @ for *)  : knut.franke*gmx.de
	Description          : Execute Python code from within SciDAVis

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
// get rid of a compiler warning
#ifdef _POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#endif
#include <Python.h>
#include <compile.h>
#include <eval.h>
#include <frameobject.h>
#include <traceback.h>

#include <iostream>
using namespace std;

#define str(x) xstr(x)
#define xstr(x) #x

#if PY_VERSION_HEX < 0x020400A1
typedef struct _traceback {
	PyObject_HEAD
		struct _traceback *tb_next;
	PyFrameObject *tb_frame;
	int tb_lasti;
	int tb_lineno;
} PyTracebackObject;
#endif

#include "PythonScript.h"
#include "PythonScripting.h"
#include "ApplicationWindow.h"

#include <QObject>
#include <QStringList>
#include <QDir>
#include <QDateTime>
#include <QCoreApplication>

#ifdef _WIN32
#include <windows.h>
static char pythonHome[MAX_PATH];
#endif

// includes sip.h, which undefines Qt's "slots" macro since SIP 4.6
#include "sipAPIscidavis.h"
extern "C" 
{
  void initerrno();
  void initmath();
  void initsip();
  void initQt();
  void initQtCore();
  void initQtGui();
  void initscidavis();
}

const char* PythonScripting::langName = "Python";

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

PyObject *PythonScripting::eval(const QString &code, PyObject *argDict, const char *name)
{
	PyObject *args;
	if (argDict)
		args = argDict;
	else
		args = globals;
	PyObject *ret=NULL;
	PyObject *co = Py_CompileString(code.toAscii().constData(), name, Py_eval_input);
	if (co)
	{
		ret = PyEval_EvalCode((PyCodeObject*)co, globals, args);
		Py_DECREF(co);
	}
	return ret;
}

bool PythonScripting::exec (const QString &code, PyObject *argDict, const char *name)
{
	PyObject *args;
	if (argDict)
		args = argDict;
	else
		args = globals;
	PyObject *tmp = NULL;
	PyObject *co = Py_CompileString(code.toAscii().constData(), name, Py_file_input);
	if (co)
	{
		tmp = PyEval_EvalCode((PyCodeObject*)co, globals, args);
		Py_DECREF(co);
	}
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
	PyErr_NormalizeException(&exception, &value, &traceback);
	if(PyErr_GivenExceptionMatches(exception, PyExc_SyntaxError))
	{
		QString text = toString(PyObject_GetAttrString(value, "text"), true);
		msg.append(text + "\n");
		PyObject *offset = PyObject_GetAttrString(value, "offset");
		for (int i=0; i<(PyInt_AsLong(offset)-1); i++)
			if (text[i] == '\t')
				msg.append("\t");
			else
				msg.append(" ");
		msg.append("^\n");
		Py_DECREF(offset);
		msg.append("SyntaxError: ");
		msg.append(toString(PyObject_GetAttrString(value, "msg"), true) + "\n");
		msg.append("at ").append(toString(PyObject_GetAttrString(value, "filename"), true));
		msg.append(":").append(toString(PyObject_GetAttrString(value, "lineno"), true));
		msg.append("\n");
		Py_DECREF(exception);
		Py_DECREF(value);
	} else {
		msg.append(toString(exception,true)).remove("exceptions.").append(": ");
		msg.append(toString(value,true));
		msg.append("\n");
	}

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

PythonScripting::PythonScripting(ApplicationWindow *parent, bool batch)
  : ScriptingEnv(parent, langName)
{
  PyObject *mainmod=NULL, *scidavismod=NULL, *sysmod=NULL;
  math = NULL;
  sys = NULL;
  d_initialized = false;
  cout << "b4 Py_IsInitialized()" << endl;
  if (Py_IsInitialized())
    {
      //		PyEval_AcquireLock();
      cout << "b4 PyImport_ImportModule(__main__)" << endl;
      mainmod = PyImport_ImportModule("__main__");
      if (!mainmod)
        {
          PyErr_Print();
          //			PyEval_ReleaseLock();
          return;
        }
      cout << "b4 PyModule_GetDict" << endl;
      globals = PyModule_GetDict(mainmod);
      cout << "b4 Py_DECREF(mainmod)" << endl;
      Py_DECREF(mainmod);
    } else {
    // if we need to bundle Python libraries with the executable,
    // specify the library location here
    cout << "WIN32="<<_WIN32<< endl;
#if defined(PYTHONHOME)
    cout << "Py_SetPythonHome" << endl;
    Py_SetPythonHome(str(PYTHONHOME));
#elif defined(_WIN32)
    // look in the executables directory
    GetModuleFileNameA( NULL, pythonHome, MAX_PATH );
    cout << "pythonhome="<<pythonHome<<endl;
    *strrchr(pythonHome,'\\')='\0'; // trim of exe name
    cout << "pythonhome="<<pythonHome<<endl;
    Py_SetPythonHome(pythonHome);
#endif
    //		PyEval_InitThreads ();
      cout << "Py_Initialize" << endl;
      puts("just b4 Py_Initialize");
      cout << "updated..."<<endl;
#ifdef _WIN32
      Py_NoSiteFlag=1;
#endif
    Py_Initialize ();
    if (!Py_IsInitialized ())
      return;

    PyRun_SimpleString("print 'hello from scidavis'\n");
    int r=PyRun_SimpleString("import sys; print sys.path\n");
    cout << "PyRun_SimpleString retruned "<<r<< endl;
    
#ifdef SIP_STATIC_MODULE
    initerrno();
    initmath();
    cout << "msg: "<<errorMsg().toStdString() << endl;
      cout << "initsip" << endl;
    initsip();
    initQt();
    cout << "msg: "<<errorMsg().toStdString() << endl;
      cout << "initQtCore" << endl;
    initQtCore();
    PyRun_SimpleString("import sys; print sys.modules\n");
    cout << "msg: "<<errorMsg().toStdString() << endl;
      cout << "initQtGui" << endl;
    initQtGui();
    cout << "msg: "<<errorMsg().toStdString() << endl;
#endif
      cout << "initscidavis" << endl;
    initscidavis();

      cout << "PyImport_AddModule(__main__)" << endl;
    mainmod = PyImport_AddModule("__main__");
    if (!mainmod)
      {
        //			PyEval_ReleaseLock();
        PyErr_Print();
        return;
      }
      cout << "PyModule_GetDict(mainmod)" << endl;
    globals = PyModule_GetDict(mainmod);
  }

  if (!globals)
    {
      PyErr_Print();
      //		PyEval_ReleaseLock();
      return;
    }
      cout << "Py_INCREF(globals)" << endl;
  Py_INCREF(globals);

      cout << "PyDict_New" << endl;
  math = PyDict_New();
  if (!math)
    PyErr_Print();

      cout << "PyImport_ImportModule(scidavis)" << endl;
  scidavismod = PyImport_ImportModule("scidavis");
  if (scidavismod)
    {
      cout << "PyDict_SetItemString" << endl;
      PyDict_SetItemString(globals, "scidavis", scidavismod);
      cout << "PyModule_GetDict(scidavismod)" << endl;
      PyObject *scidavisDict = PyModule_GetDict(scidavismod);
      if (!setQObject(d_parent, "app", scidavisDict))
        QMessageBox::warning
          (d_parent, tr("Failed to export SciDAVis API"),
           tr("Accessing SciDAVis functions or objects from Python code won't work." 
              "Probably your version of SIP differs from the one SciDAVis was compiled against;" 
              "try updating SIP or recompiling SciDAVis."));
      cout << "PyDict_SetItemString(scidavisDict" << endl;
      PyDict_SetItemString(scidavisDict, "mathFunctions", math);
      cout << "Py_DECREF(scidavismod)" << endl;
      Py_DECREF(scidavismod);
    } else
    PyErr_Print();

      cout << "PyImport_ImportModule(sys)" << endl;
  sysmod = PyImport_ImportModule("sys");
  if (sysmod)
    {
      cout << "PyModule_GetDict(sysmod)" << endl;
      sys = PyModule_GetDict(sysmod);
      cout << "Py_INCREF(sys)" << endl;
      Py_INCREF(sys);
    } else
    PyErr_Print();

  //	PyEval_ReleaseLock();
  d_initialized = true;
  cout << "exiting PythonScripting::PythonScripting"<<endl;
}

void PythonScripting::redirectStdIO()
{
  // Redirect output to the print(const QString&) signal.
  // Also see method write(const QString&) and Python documentation on
  // sys.stdout and sys.stderr.
  setQObject(this, "stdout", sys);
  setQObject(this, "stderr", sys);
}

bool PythonScripting::initialize()
{
  if (!d_initialized) return false;
  //	PyEval_AcquireLock();

  if (!d_parent->batchMode())
    {
      // Redirect output to the print(const QString&) signal.
      // Also see method write(const QString&) and Python documentation on
      // sys.stdout and sys.stderr.
      setQObject(this, "stdout", sys);
      setQObject(this, "stderr", sys);
    }
  bool initialized;
  initialized = loadInitFile(QDir::homeDirPath()+"/scidavisrc");
  if(!initialized) initialized = loadInitFile(QDir::homeDirPath()+"/.scidavisrc");
#ifdef PYTHON_CONFIG_PATH 
  if(!initialized) initialized = loadInitFile(PYTHON_CONFIG_PATH"/scidavisrc");
  if(!initialized) initialized = loadInitFile(PYTHON_CONFIG_PATH"/.scidavisrc");
#endif
  if(!initialized) initialized = loadInitFile(QDir::rootDirPath()+"etc/scidavisrc");
  if(!initialized) initialized = loadInitFile(QCoreApplication::instance()->applicationDirPath()+"/scidavisrc");
  if(!initialized) initialized = loadInitFile("scidavisrc");

  if (PyErr_Occurred()) cout << errorMsg().toStdString() << endl;
  //	PyEval_ReleaseLock();
  return true;
}

PythonScripting::~PythonScripting()
{
	Py_XDECREF(globals);
	Py_XDECREF(math);
	Py_XDECREF(sys);
}

#ifndef PYTHON_UTIL_PATH
#define PYTHON_UTIL_PATH "."
#endif

bool PythonScripting::loadInitFile(const QString &path)
{
	PyRun_SimpleString("import sys\nsys.path.append('" PYTHON_UTIL_PATH "')"); 
	QFileInfo pyFile(path+".py"), pycFile(path+".pyc");
	bool success = false;
	if (pycFile.isReadable() && (pycFile.lastModified() >= pyFile.lastModified())) {
		// if we have a recent pycFile, use it
		FILE *f = fopen(pycFile.filePath(), "rb");
		success = PyRun_SimpleFileEx(f, pycFile.filePath(), false) == 0;
		fclose(f);
	} else if (pyFile.isReadable() && pyFile.exists()) {
		// try to compile pyFile to pycFile
		PyObject *compileModule = PyImport_ImportModule("py_compile");
		if (compileModule) {
			PyObject *compile = PyDict_GetItemString(PyModule_GetDict(compileModule), "compile");
			if (compile) {
				PyObject *tmp = PyObject_CallFunctionObjArgs(compile,
						PyString_FromString(pyFile.filePath()),
						PyString_FromString(pycFile.filePath()),
						NULL);
				if (tmp)
					Py_DECREF(tmp);
				else
					PyErr_Print();
			} else
				PyErr_Print();
			Py_DECREF(compileModule);
		} else
			PyErr_Print();
		pycFile.refresh();
		if (pycFile.isReadable() && (pycFile.lastModified() >= pyFile.lastModified())) {
			// run the newly compiled pycFile
			FILE *f = fopen(pycFile.filePath(), "rb");
			success = PyRun_SimpleFileEx(f, pycFile.filePath(), false) == 0;
			fclose(f);
		} else {
			// fallback: just run pyFile
			/*FILE *f = fopen(pyFile.filePath(), "r");
			success = PyRun_SimpleFileEx(f, pyFile.filePath(), false) == 0;
			fclose(f);*/
			//TODO: code above crashes on Windows - bug in Python?
			QFile f(pyFile.filePath());
			if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
				QByteArray data = f.readAll();
				success = PyRun_SimpleString(data.data());
				f.close();
			}
		}
	}
	return success;
}

bool PythonScripting::isRunning() const
{
	return Py_IsInitialized();
}

bool PythonScripting::setQObject(QObject *val, const char *name, PyObject *dict)
{
  cout << "in PythonScripting::setQObject"<<endl;
	if(!val) return false;
	PyObject *pyobj=NULL;

	PyGILState_STATE state = PyGILState_Ensure();

        cout << "b4 sipFindType "<<val->className()<<endl;
        cout << "b4 sipAPI_scidavis="<<sipAPI_scidavis<<endl;
        //sipWrapperType * klass = sipFindClass(val->className());
        const sipTypeDef* klass=sipFindType(val->className());
	if (!klass) return false;
        //pyobj = sipConvertFromInstance(val, klass, NULL);
        cout << "b4 sipConvertFromType "<<val->className()<<endl;
	pyobj = sipConvertFromType(val, klass, NULL);
	if (!pyobj) return false;

        cout << "b4 PyDict_SetItemString "<<val->className()<<endl;
	if (dict)
		PyDict_SetItemString(dict,name,pyobj);
	else
		PyDict_SetItemString(globals,name,pyobj);
        cout << "b4 Py_DECREF "<<val->className()<<endl;
	Py_DECREF(pyobj);

        cout << "b4 PyGILState_Release "<<val->className()<<endl;
	PyGILState_Release(state);
        cout <<"returnng from PythonScripting::setQObject"<<endl;
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
#if PY_VERSION_HEX >= 0x02050000
	Py_ssize_t i=0;
#else
	int i=0;
#endif
	while(PyDict_Next(math, &i, &key, &value))
		if (PyCallable_Check(value))
			flist << PyString_AsString(key);
	flist.sort();
	return flist;
}

const QString PythonScripting::mathFunctionDoc(const QString &name) const
{
	PyObject *mathf = PyDict_GetItemString(math,name); // borrowed
	if (!mathf) return "";
	PyObject *pydocstr = PyObject_GetAttrString(mathf, "__doc__"); // new
	QString qdocstr = PyString_AsString(pydocstr);
	Py_XDECREF(pydocstr);
	return qdocstr;
}

const QStringList PythonScripting::fileExtensions() const
{
	QStringList extensions;
	extensions << "py" << "PY";
	return extensions;
}


