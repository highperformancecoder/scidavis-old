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
#include <python_base.h>

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

#include "PythonScript.h"
#include "PythonScript.cd"
#include "PythonScripting.h"
#include "PythonScripting.cd"

#include "ApplicationWindow.h"
#include "ApplicationWindow.cd"
#include "Folder.h"
#include "Folder.cd"
#include "Graph.h"
#include "Graph.cd"
#include "Graph3D.h"
#include "Matrix.h"
#include "Matrix.cd"
#include "MultiLayer.h"
#include "MyWidget.h"
#include "MyWidget.cd"
#include "Note.h"
#include "Note.cd"
#include "QtEnums.cd"
#include "Script.h"
#include "Script.cd"
#include "ArrowMarker.h"
#include "ArrowMarker.cd"
#include "Table.h"
#include "Table.cd"
#include "TableView.cd"
#include "MultiLayer.cd"
#include "QwtSymbol.cd"
#include "QwtErrorPlotCurve.h"
#include "IntervalAttribute.cd"
#include "Interval.cd"
#include "Fit.h"
#include "Fit.cd"
#include "ExponentialFit.h"
#include "ExponentialFit.cd"
#include "Qt.h"
#include "Qt.cd"
#include "AbstractAspect.cd"
#include "AbstractColumn.cd"
#include "Column.cd"
#include "globals.cd"

#include <QTranslator>
#include <QToolBar>

using namespace std;
using boost::python::object;
using boost::python::exec;
using boost::python::import;

namespace classdesc_access
{
//  template <>
//  struct access_python<QString>
//  {
//    template <class C> 
//    void type(classdesc::python_t& targ, const classdesc::string&)
//    {
//      auto& c=targ.getClass<C>();
//      if (!c.completed)
//        c.def("__str__",&QString::toStdString);
//    }
//  };

  template <class T>
  struct access_python<QList<T>>:
    public classdesc::NullDescriptor<classdesc::python_t> {};

//  template <>
//  struct access_python<QStringList>:
//    public classdesc::NullDescriptor<classdesc::python_t> {};

  template <class E, class Q>
  struct access_python<QtEnumWrapper<E,Q>>
  {
    template <class C> 
    void type(classdesc::python_t& targ, const classdesc::string& desc)
    {
      targ.addEnum<C>(desc,&QtEnumWrapper<E,Q>::value);
    }
  };
}

namespace classdesc
{
  template <class E, class Q>
  struct tn<QtEnumWrapper<E,Q>>
  {
    static string name() {return typeName<E>();}
  };
  
  // Use Qt's MOC system for reflection on Qt objects
  template <class T>
  typename enable_if<is_base_of<QObject,T>, string>::T
  mocTypeName() {return T::staticMetaObject.className();}

#define DEF_TYPENAME(X)                         \
  template <> struct tn<X>                      \
  {                                             \
    static string name() {return #X;}           \
  };
  
  DEF_TYPENAME(QString);
  DEF_TYPENAME(QLocale);
  DEF_TYPENAME(QColor);
  DEF_TYPENAME(QSize);
  DEF_TYPENAME(QFont);
  DEF_TYPENAME(QPoint);
  DEF_TYPENAME(QPointF);
  DEF_TYPENAME(QStringList);
  DEF_TYPENAME(QChar);
  DEF_TYPENAME(QRect);
  DEF_TYPENAME(QRectF);
  DEF_TYPENAME(QPen);
  DEF_TYPENAME(QIcon);
  DEF_TYPENAME(QBrush);
  DEF_TYPENAME(QVariant);
  DEF_TYPENAME(QwtPlotCurve);
  DEF_TYPENAME(QwtPlotZoomer);
  DEF_TYPENAME(QTreeWidgetItem);
  DEF_TYPENAME(QDate);
  DEF_TYPENAME(QDateTime);
  DEF_TYPENAME(QTime);

  template <class T> struct tn<QList<T>>
  {
    static string name() {return "QList<"+typeName<T>()+">";}
  };

  template <class T> struct tn<QVector<T>>
  {
    static string name() {return "QVector<"+typeName<T>()+">";}
  };

  template <class T> struct tn<QPointer<T>>
  {
    static string name() {return "QPointer<"+typeName<T>()+">";}
  };

  template <class K, class V> struct tn<QMap<K,V>>
  {
    static string name() {return "QMap<"+typeName<K>()+","+typeName<V>()+">";}
  };


  template <class T> struct tn
  {
    static string name() {return mocTypeName<T>();}
  };

  // generates list semantics for QStringList
  template <> struct is_sequence<QStringList> {static const bool value=true;};
}

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

// variable argument overloads here
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ApplicationWindow_newTableSII,newTable,1,3);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ApplicationWindow_newMatrixSII,newMatrix,0,3);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ApplicationWindow_newGraphS,newGraph,0,1);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ApplicationWindow_newNoteS,newNote,0,1);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ApplicationWindow_plotTVSII,plot,2,4);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ApplicationWindow_plotTSII,plot,2,4);

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(Graph_addErrorBars,addErrorBars,3,10);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(Graph_exportImage,exportImage,1,2);

// returns the current ApplicationWindow instance
ApplicationWindow& theApp()
{
  using namespace py;
  return extract<ApplicationWindow&>(modDict("__main__")["app"]);

}

boost::shared_ptr<ExponentialFit> newExponentialFit(ApplicationWindow& a,Graph& g, const QString& s)
{
  boost::shared_ptr<ExponentialFit> r(new ExponentialFit(&a,&g,s));
  return r;
}

boost::shared_ptr<ExponentialFit> newExponentialFit2(Graph& g, const QString& s)
{return newExponentialFit(theApp(),g,s);}

BOOST_PYTHON_MODULE(scidavis)
{
  // register the Qstring to-python converter
  py::to_python_converter<QString,QString_to_python_str>();
  // register the Qstring from-python converter
  QString_from_python_str();

  classdesc::python_t p;
  p.defineClass<SciDAVis>();
  p.defineClass<ApplicationWindow>();
  p.defineClass<PythonScripting>();
  p.defineClass<PythonScript>();
  p.defineClass<ArrowMarker>();
  p.defineClass<ExponentialFit>();
  p.defineClass<QtNamespace>();
  p.defineClass<Column>();
  python<SciDAVis::ColumnMode>(p,"");
  // redefine Qt as an alias for QtNamespace - unfortunately QtNamespace cannot be called Qt in C++ as Qt is already taken
  modDict("__main__")["Qt"]=modDict("scidavis")["QtNamespace"];
  
  // overload handling
  Table& (ApplicationWindow::*newTable)()=&ApplicationWindow::newTable;
  Table& (ApplicationWindow::*newTableSII)(const std::string&,int,int)=&ApplicationWindow::newTable;
  MultiLayer& (ApplicationWindow::*plotTSII)(Table&,const std::string&, int, int)
    =&ApplicationWindow::plot;
  MultiLayer& (ApplicationWindow::*plotTVSII)(Table&,const pytuple&, int, int)
    =&ApplicationWindow::plot;
  p.getClass<ApplicationWindow>().
    overload("newTable",newTable).
    overload("newTable",newTableSII,ApplicationWindow_newTableSII()).
    overload("newMatrix",&ApplicationWindow::newMatrix,ApplicationWindow_newMatrixSII()).
    overload("newGraph",&ApplicationWindow::newGraph,ApplicationWindow_newGraphS()).
    overload("newNote",&ApplicationWindow::newNote,ApplicationWindow_newNoteS()).
    overload("plot",plotTSII,ApplicationWindow_plotTSII()).
    overload("plot",plotTVSII,ApplicationWindow_plotTVSII());

  bool (Graph::*graph_addErrorBars)
    (const QString&,Table&,const QString&,
     int,int,int,const QColor&,bool,bool,bool)=&Graph::addErrorBars;
  p.getClass<Graph>().
    overload("addErrorBars",graph_addErrorBars,Graph_addErrorBars()).
    overload("exportImage",&Graph::exportImage,Graph_exportImage())
    ;
  p.getClass<MultiLayer>().
    overload("exportImage",&MultiLayer::exportImage,Graph_exportImage())
    ;
  p.getClass<ExponentialFit>().
    def("__init__",py::make_constructor(newExponentialFit)).
    def("__init__",py::make_constructor(newExponentialFit2));
//  p.getClass<ExponentialFit>().
//    def(py::init<ApplicationWindow*,Graph*,const QString&>());
  Column& (Table::*tableColumnInt)(int x) const =&Table::column;
  Column& (Table::*tableColumnString)(const std::string& x) const=&Table::column;
  p.getClass<Table>().
    overload("column",tableColumnInt).
    overload("column",tableColumnString);

  bool (Column::*copy1)(const Column& other)=&Column::copy;
  bool (Column::*copy2)(const Column& source, int source_start, int dest_start, int num_rows)=&Column::copy;
  p.getClass<Column>().
    overload("copy",copy1).
    overload("copy",copy2);
  
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
    Py_SetPythonHome(str(PYTHONHOME));
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

//  scidavismod = PyImport_ImportModule("scidavis");
//  if (scidavismod)
//    {
//      PyDict_SetItemString(globals, "scidavis", scidavismod);
//      PyObject *scidavisDict = PyModule_GetDict(scidavismod);
//      if (!setQObject(d_parent, "app", scidavisDict))
//        QMessageBox::warning
//          (d_parent, tr("Failed to export SciDAVis API"),
//           tr("Accessing SciDAVis functions or objects from Python code won't work." 
//              "Probably your version of SIP differs from the one SciDAVis was compiled against;" 
//              "try updating SIP or recompiling SciDAVis."));
//      PyDict_SetItemString(scidavisDict, "mathFunctions", math);
//      Py_DECREF(scidavismod);
//    } else
//    PyErr_Print();

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
  //	PyEval_AcquireLock();

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

  //	PyEval_ReleaseLock();
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


