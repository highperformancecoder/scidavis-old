#ifndef SCRIPTING_H
#define SCRIPTING_H

#include <qvariant.h>
#include <qstring.h>
#include <qvaluelist.h>
#include <qobject.h>

class ApplicationWindow;
class ScriptingEnv;

class Script : public QObject
{
  // roughly equivalent to QSScript, but can execute itself (via Env in QSA)
  // this is so that you do not have to explicitly pass the ScriptingEnv to everyone who wants to execute a Script
  Q_OBJECT

  public:
    Script(ScriptingEnv *env, const QString &code, QObject *context=0, const QString &name="<input>")
      : Env(env), Code(code), Name(name), compiled(notCompiled)
      { Context = context; EmitErrors=true; }

    const QString code() const { return Code; }
    const QObject* context() const { return Context; }
    const QString name() const { return Name; }
    const bool emitErrors() const { return EmitErrors; }
    virtual void addCode(const QString &code) { Code.append(code); compiled = notCompiled; emit codeChanged(); }
    virtual void setCode(const QString &code) { Code=code; compiled = notCompiled; emit codeChanged(); }
    virtual void setContext(QObject *context) { Context = context; compiled = notCompiled; }
    void setName(const QString &name) { Name = name; compiled = notCompiled; }
    void setEmitErrors(bool yes) { EmitErrors = yes; }

  public slots:
    virtual bool compile() { return false; }
    virtual QVariant eval() { return QVariant(); }
    virtual bool exec() { return false; }

    // local variables
    virtual bool setQObject(const QObject *val, const char *name=0) { return false; } // name should default to val->name()
    virtual bool setInt(int val, const char* name) { return false; }
    virtual bool setDouble(double val, const char* name) { return false; }

  signals:
    void codeChanged();
    void error(const QString & message, const QString & scriptName, int lineNumber);
    
  protected:
    ScriptingEnv *Env;
    QString Code, Name;
    QObject *Context;
    enum compileStatus { notCompiled, isCompiled, compileErr } compiled;
    bool EmitErrors;

    void emit_error(const QString & message, int lineNumber)
      { if(EmitErrors) emit error(message, Name, lineNumber); }
};

class ScriptingEnv : public QObject
{
  // roughly equivalent to QSInterpreter
  Q_OBJECT

  public:
    ScriptingEnv(ApplicationWindow *parent) : Parent(parent) { initialized=false; }
    // Python initialization may fail; or there could be other errors setting up the environment
    bool isInitialized() const { return initialized; }
    virtual bool isRunning() const { return false; }
    
    virtual Script *newScript(const QString &code, QObject *context, const QString &name="<input>") { return 0; }
      
    // global variables
    virtual bool setQObject(QObject *val, const char *name=0) { return false; } // name should default to val->name()
    virtual bool setInt(int val, const char* name) { return false; }
    virtual bool setDouble(double val, const char* name) { return false; }

    virtual QString stackTraceString() { return QString::null; }

    virtual const QStringList mathFunctions() const { return QStringList(); }
    virtual const QString mathFunctionDoc(const QString &name) const { return QString::null; }
//    virtual QSyntaxHighlighter syntaxHighlighter(QTextEdit *textEdit) const;

  public slots:
    virtual void clear() {}
    virtual void stopExecution() {}

  signals:
    void error(const QString & message, const QString & scriptName, int lineNumber);
    
  protected:
    bool initialized;
    ApplicationWindow *Parent;
};

#endif

