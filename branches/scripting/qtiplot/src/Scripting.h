#ifndef SCRIPTING_H
#define SCRIPTING_H

#include <qvariant.h>
#include <qstring.h>
#include <qvaluelist.h>
#include <qstringlist.h>
#include <qobject.h>
#include <qevent.h>

#include "customEvents.h"
class ApplicationWindow;
class Script; // forward declaration; class follows

class ScriptingEnv : public QObject
{
  // roughly equivalent to QSInterpreter
  Q_OBJECT

  public:
    ScriptingEnv(ApplicationWindow *parent, const char *langName);

    // Python initialization may fail; or there could be other errors setting up the environment
    bool isInitialized() const { return initialized; }
    virtual bool isRunning() const { return false; }
    
    virtual Script *newScript(const QString&, QObject*, const QString&) { return 0; }
      
    // global variables
    virtual bool setQObject(QObject*, const char*) { return false; } // name should default to val->name()
    virtual bool setInt(int, const char*) { return false; }
    virtual bool setDouble(double, const char*) { return false; }

    virtual QString stackTraceString() { return QString::null; }

    virtual const QStringList mathFunctions() const { return QStringList(); }
    virtual const QString mathFunctionDoc(const QString&) const { return QString::null; }
//    virtual QSyntaxHighlighter syntaxHighlighter(QTextEdit *textEdit) const;

  public slots:
    virtual void clear() {}
    virtual void stopExecution() {}
    void incref();
    void decref();

  signals:
    void error(const QString & message, const QString & scriptName, int lineNumber);
    
  protected:
    bool initialized;
    ApplicationWindow *Parent;

  private:
    int refcount;
};

class Script : public QObject
{
  // roughly equivalent to QSScript, but can execute itself (via Env in QSA)
  // this is so that you do not have to explicitly pass the ScriptingEnv to everyone who wants to execute a Script
  Q_OBJECT

  public:
    Script(ScriptingEnv *env, const QString &code, QObject *context=0, const QString &name="<input>")
      : Env(env), Code(code), Name(name), compiled(notCompiled)
      { Env->incref(); Context = context; EmitErrors=true; }
    ~Script() { Env->decref(); }

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
    virtual bool compile(bool for_eval=true) { emit_error("Script::compile called!",0); return false; }
    virtual QVariant eval() { emit_error("Script::eval called!",0); return QVariant(); }
    virtual bool exec() { emit_error("Script::exec called!",0); return false; }

    // local variables
    virtual bool setQObject(const QObject*, const char*) { return false; }
    virtual bool setInt(int, const char*) { return false; }
    virtual bool setDouble(double, const char*) { return false; }

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

class ScriptingLangManager
{
  public:
    static ScriptingEnv *newEnv(ApplicationWindow *parent);
    static ScriptingEnv *newEnv(const char *name, ApplicationWindow *parent);
    static QStringList languages();

  private:
    typedef ScriptingEnv*(*ScriptingEnvConstructor)(ApplicationWindow*);
    typedef struct {
      const char *name;
      ScriptingEnvConstructor constructor;
    } ScriptingLang;
//! global registry of available languages
    static ScriptingLang langs[];
};

class ScriptingChangeEvent : public QCustomEvent
{
  public:
    ScriptingChangeEvent(ScriptingEnv *e) : QCustomEvent(SCRIPTING_CHANGE_EVENT), env(e) {}
    ScriptingEnv *scriptingEnv() const { return env; }
  private:
    ScriptingEnv *env;
};

class scripted
{
  public:
    scripted(ScriptingEnv* env);
    ~scripted();
    void scriptingChangeEvent(ScriptingChangeEvent*);

  protected:
    ScriptingEnv *scriptEnv;
};

#endif

