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
#include "PythonClassdesc.h"

#ifdef _POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#endif
#include <Python.h>
#include <compile.h>
#include <eval.h>
#include <frameobject.h>
#include <traceback.h>

#include <iostream>

#if PY_VERSION_HEX < 0x020400A1
typedef struct _traceback {
	PyObject_HEAD
		struct _traceback *tb_next;
	PyFrameObject *tb_frame;
	int tb_lasti;
	int tb_lineno;
} PyTracebackObject;
#endif

// disable these warnings, as automatically generated code sometimes has them
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include "PythonScripting.h"

#include "future/core/AbstractAspect.h"
#include "AbstractAspect.cd"
#include "future/core/AbstractColumn.h"
#include "AbstractColumn.cd"
#include "ArrowMarker.h"
#include "ArrowMarker.cd"
#include "future/core/column/Column.h"
#include "Column.cd"
#include "ExponentialFit.h"
#include "ExponentialFit.cd"
#include "Fit.h"
#include "Fit.cd"
#include "Filter.h"
#include "Filter.cd"
#include "Folder.cd"
#include "globals.h"
#include "globals.cd"
#include "Graph.cd"
#include "Grid.h"
#include "Grid.cd"
#include "ImageMarker.h"
#include "ImageMarker.cd"
#include "Integration.h"
#include "Integration.cd"
#include "Interpolation.h"
#include "Interpolation.cd"
#include "future/lib/Interval.h"
#include "Interval.cd"
#include "future/lib/IntervalAttribute.h"
#include "IntervalAttribute.cd"
#include "Legend.h"
#include "Legend.cd"
#include "Matrix.cd"
#include "MatrixView.cd"
#include "MultiLayer.cd"
#include "MultiPeakFit.h"
#include "MultiPeakFit.cd"
#include "MyWidget.cd"
#include "NonLinearFit.h"
#include "NonLinearFit.cd"
#include "Note.cd"
#include "PlotEnrichement.cd"
#include "PolynomialFit.h"
#include "PolynomialFit.cd"
#include "Qt.h"
#include "Qt.cd"
#include "QtEnums.h"
#include "QtEnums.cd"
#include "QwtSymbol.cd"
#include "QwtErrorPlotCurve.h"
#include "QwtErrorPlotCurve.cd"
#include "Script.h"
#include "Script.cd"
#include "ScriptingEnv.cd"
#include "SmoothFilter.h"
#include "SmoothFilter.cd"
#include "Table.h"
#include "Table.cd"
#include "TableView.cd"
#include "PythonExtras.h"

#include <QPainter>

using namespace std;
using boost::python::object;
using boost::python::exec;
using boost::python::import;

#include <classdesc_epilogue.h>

#include <QObject>
#include <QStringList>
#include <QDir>
#include <QDateTime>
#include <QCoreApplication>

#if PY_MAJOR_VERSION < 3
#define PYUNICODE_AsUTF8     PyString_AsString
#define PYUNICODE_FromString PyString_FromString
#define PYLong_AsLong        PyInt_AsLong
#define PYCodeObject_cast    (PyCodeObject*)
#else
#define PYUNICODE_AsUTF8     PyUnicode_AsUTF8
#define PYUNICODE_FromString PyUnicode_FromString
#define PYLong_AsLong        PyLong_AsLong
#define PYCodeObject_cast
#endif

// nabbed from https://misspent.wordpress.com/2009/09/27/how-to-write-boost-python-converters/
struct QString_to_python_str
{
    static PyObject* convert(QString const& s)
      {
        return boost::python::incref(
          boost::python::object(
            s.toLatin1().constData()).ptr());
      }
};
 
// nabbed from https://misspent.wordpress.com/2009/09/27/how-to-write-boost-python-converters/
struct QString_from_python_str
{
    QString_from_python_str()
    {
      boost::python::converter::registry::push_back(
        &convertible,
        &construct,
        boost::python::type_id<QString>());
    }
 
    // Determine if obj_ptr can be converted in a QString
    static void* convertible(PyObject* obj_ptr)
    {
      if (!PyString_Check(obj_ptr)) return 0;
      return obj_ptr;
    }
 
    // Convert obj_ptr into a QString
    static void construct(
    PyObject* obj_ptr,
    boost::python::converter::rvalue_from_python_stage1_data* data)
    {
      // Extract the character data from the python string
      const char* value = PyString_AsString(obj_ptr);
 
      // Verify that obj_ptr is a string (should be ensured by convertible())
      assert(value);
 
      // Grab pointer to memory into which to construct the new QString
      void* storage = (
        (boost::python::converter::rvalue_from_python_storage<QString>*)
        data)->storage.bytes;
 
      // in-place construct the new QString using the character data
      // extraced from the python object
      new (storage) QString(value);
 
      // Stash the memory chunk pointer for later use by boost.python
      data->convertible = storage;
    }
};

const char* PythonScripting::langName = "Python";

// returns the current ApplicationWindow instance
ApplicationWindow& theApp()
{
  using namespace py;
  return extract<ApplicationWindow&>(modDict("__main__")["app"]);

}

BOOST_PYTHON_MODULE(scidavis)
{
  // register the Qstring to-python converter
  py::to_python_converter<QString,QString_to_python_str>();
  // register the Qstring from-python converter
  QString_from_python_str();

  classdesc::python_t p;
  exposeApplicationWindow(p);
  p.defineClass<ArrowMarker>();
  p.defineClass<ExponentialFit>();
  p.defineClass<GaussFit>();
  p.defineClass<GaussAmpFit>();
  p.defineClass<LinearFit>();
  p.defineClass<LorentzFit>();
  p.defineClass<MultiPeakFit>();
  p.defineClass<NonLinearFit>();
  p.defineClass<PolynomialFit>();
  p.defineClass<ThreeExpFit>();
  p.defineClass<TwoExpFit>();
  p.defineClass<QtNamespace>();
  p.defineClass<Integration>();
  p.defineClass<Interpolation>();
  p.defineClass<SmoothFilter>();
  p.defineClass<SciQwtSymbol>();

  // redefine Qt as an alias for QtNamespace - unfortunately QtNamespace cannot be called Qt in C++ as Qt is already taken
  modDict("__main__")["Qt"]=modDict("scidavis")["QtNamespace"];
}

QString PythonScripting::toString(PyObject *object, bool decref)
{
	QString ret;
	if (!object) return "";
	PyObject *repr = PyObject_Str(object);
	if (decref) Py_DECREF(object);
	if (!repr) return "";
	ret = PYUNICODE_AsUTF8(repr);
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
	PyObject *co = Py_CompileString(code.toUtf8().constData(), name, Py_eval_input);
	if (co)
	{
		ret = PyEval_EvalCode(PYCodeObject_cast co, globals, args);
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
	PyObject *co = Py_CompileString(code.toUtf8().constData(), name, Py_file_input);
	if (co)
	{
		tmp = PyEval_EvalCode(PYCodeObject_cast co, globals, args);
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
	const char *fname;
	QString msg;
	if (!PyErr_Occurred()) return "";

	PyErr_Fetch(&exception, &value, &traceback);
	PyErr_NormalizeException(&exception, &value, &traceback);
	if(PyErr_GivenExceptionMatches(exception, PyExc_SyntaxError))
	{
		QString text = toString(PyObject_GetAttrString(value, "text"), true);
		msg.append(text + "\n");
		PyObject *offset = PyObject_GetAttrString(value, "offset");
		for (int i=0; i<(PYLong_AsLong(offset)-1); i++)
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
			msg.append("at ").append(PYUNICODE_AsUTF8(frame->f_code->co_filename));
			msg.append(":").append(QString::number(excit->tb_lineno));
			if (frame->f_code->co_name && *(fname = PYUNICODE_AsUTF8(frame->f_code->co_name)) != '?')
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
  Q_UNUSED(batch);

  Py_Initialize ();
  if (!Py_IsInitialized ())
    return;
  initscidavis();
  classdesc::addPythonObject("app",*parent);
  
  PyObject *mainmod=NULL, *sysmod=NULL;
  math = NULL;
  sys = NULL;
  d_initialized = false;
  if (Py_IsInitialized())
    {
      //		PyEval_AcquireLock();
      mainmod = PyImport_ImportModule("__main__");
      if (!mainmod)
        {
          PyErr_Print();
          //			PyEval_ReleaseLock();
          return;
        }
      globals = PyModule_GetDict(mainmod);
      Py_DECREF(mainmod);
    } else {
    // if we need to bundle Python libraries with the executable,
    // specify the library location here
#ifdef PYTHONHOME
    Py_SetPythonHome(const_cast<char*>(str(PYTHONHOME)));
#endif
    //		PyEval_InitThreads ();
#if PY_MAJOR_VERSION >= 3
    PyImport_AppendInittab("scidavis", &PyInit_scidavis);
#endif


    mainmod = PyImport_AddModule("__main__");
    if (!mainmod)
      {
        //			PyEval_ReleaseLock();
        PyErr_Print();
        return;
      }
    globals = PyModule_GetDict(mainmod);
  }

  if (!globals)
    {
      PyErr_Print();
      //		PyEval_ReleaseLock();
      return;
    }
  Py_INCREF(globals);

  math = PyDict_New();
  if (!math)
    PyErr_Print();

  sysmod = PyImport_ImportModule("sys");
  if (sysmod)
    {
      sys = PyModule_GetDict(sysmod);
      Py_INCREF(sys);
    } else
    PyErr_Print();

  //	PyEval_ReleaseLock();
  d_initialized = true;
}

void PythonScripting::redirectStdIO()
{
  // Redirect output to the print(const QString&) signal.
  // Also see method write(const QString&) and Python documentation on
  // sys.stdout and sys.stderr.
  classdesc::addPythonObject("sys.stdout",*this);
  classdesc::addPythonObject("sys.stderr",*this);
}

bool PythonScripting::initialize()
{
  if (!d_initialized) return false;

  if (!d_parent->batchMode()) redirectStdIO();

  bool initialized;
  initialized = loadInitFile(QDir::homePath()+"/scidavisrc");
  if(!initialized) initialized = loadInitFile(QDir::homePath()+"/.scidavisrc");
#ifdef PYTHON_CONFIG_PATH 
  if(!initialized) initialized = loadInitFile(PYTHON_CONFIG_PATH"/scidavisrc");
  if(!initialized) initialized = loadInitFile(PYTHON_CONFIG_PATH"/.scidavisrc");
#endif
  if(!initialized) initialized = loadInitFile(QDir::rootPath()+"etc/scidavisrc");
  if(!initialized) initialized = loadInitFile(QCoreApplication::instance()->applicationDirPath()+"/scidavisrc");
  if(!initialized) initialized = loadInitFile("scidavisrc");

  return initialized;
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
		FILE *f = fopen(pycFile.filePath().toLocal8Bit(), "rb");
		success = PyRun_SimpleFileEx(f, pycFile.filePath().toLocal8Bit(), false) == 0;
		fclose(f);
	} else if (pyFile.isReadable() && pyFile.exists()) {
		// try to compile pyFile to pycFile
		PyObject *compileModule = PyImport_ImportModule("py_compile");
		if (compileModule) {
			PyObject *compile = PyDict_GetItemString(PyModule_GetDict(compileModule), "compile");
			if (compile) {
				PyObject *tmp = PyObject_CallFunctionObjArgs(compile,
						PYUNICODE_FromString(pyFile.filePath().toUtf8().constData()),
						PYUNICODE_FromString(pycFile.filePath().toUtf8().constData()),
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
			FILE *f = fopen(pycFile.filePath().toLocal8Bit(), "rb");
			success = PyRun_SimpleFileEx(f, pycFile.filePath().toLocal8Bit(), false) == 0;
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

QStringList PythonScripting::mathFunctions() const
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
			flist << PYUNICODE_AsUTF8(key);
	flist.sort();
	return flist;
}

QString PythonScripting::mathFunctionDoc(const QString &name) const
{
	PyObject *mathf = PyDict_GetItemString(math,name.toLocal8Bit()); // borrowed
	if (!mathf) return "";
	PyObject *pydocstr = PyObject_GetAttrString(mathf, "__doc__"); // new
	QString qdocstr = PYUNICODE_AsUTF8(pydocstr);
	Py_XDECREF(pydocstr);
	return qdocstr;
}

QStringList PythonScripting::fileExtensions() const
{
	QStringList extensions;
	extensions << "py" << "PY";
	return extensions;
}


