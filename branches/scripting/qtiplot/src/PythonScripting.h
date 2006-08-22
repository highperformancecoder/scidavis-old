#ifndef PYTHON_SCRIPTING_H
#define PYTHON_SCRIPTING_H

#include "Scripting.h"

#include <qobject.h>
#include <qstring.h>

typedef struct _object PyObject;
class PythonScripting;

class PythonScript : public Script
{
  Q_OBJECT
    
  public:
    PythonScript(PythonScripting *env, const QString &code, QObject *context=0, const QString &name="<input>");
    ~PythonScript();
	
  public slots:
    bool compile(bool for_eval=true);
    QVariant eval();
    bool exec();
    bool setQObject(QObject *val, const char *name);
    bool setInt(int val, const char* name);
    bool setDouble(double val, const char* name);
    void setContext(QObject *context);

  private:
    PythonScripting *env() { return (PythonScripting*)Env; }

    PyObject *PyCode, *localDict;
    bool isFunction;
};

class PythonScripting: public ScriptingEnv
{
  Q_OBJECT

  public:
    static const char *langName;
    PythonScripting(ApplicationWindow *parent);
    ~PythonScripting();
    static ScriptingEnv *constructor(ApplicationWindow *parent) { return new PythonScripting(parent); }

    QString toString(PyObject *object, bool decref=false);
    PyObject* eval(const QString &code, PyObject *argDict=NULL, const char *name="<qtiplot>");
    bool exec(const QString &code, PyObject *argDict=NULL, const char *name="<qtiplot>");
    QString errorMsg();
    bool importModule(char *name);

    bool isRunning() const;
    Script *newScript(const QString &code, QObject *context, const QString &name="<input>")
    {
      return new PythonScript(this, code, context, name);
    }

    bool setQObject(QObject*, const char*, PyObject *dict);
    bool setQObject(QObject *val, const char *name) { return setQObject(val,name,NULL); }
    bool setInt(int, const char*, PyObject *dict=NULL);
    bool setDouble(double, const char*, PyObject *dict=NULL);
    
    const QStringList mathFunctions() const;
    const QString mathFunctionDoc (const QString &name) const;

    PyObject *globalDict() { return globals; }

  private:
    PyObject *globals;		// PyDict of global environment
    PyObject *math;		// PyDict of math module
};

#endif
