#ifndef MUPARSER_SCRIPTING_H
#define MUPARSER_SCRIPTING_H

#include "Scripting.h"

#include "../3rdparty/muParser/muParser.h"
#include <gsl/gsl_sf.h>
#include <qasciidict.h>

template <class T1, class T2> class QMap;
typedef QMap<QString,QString> strDict;

class muParserScript: public Script
{
  Q_OBJECT
    
  public:
    muParserScript(ScriptingEnv *env, const QString &code, QObject *context=0, const QString &name="<input>");
	
  public slots:
    bool compile();
    QVariant eval();
    bool exec();
    bool setQObject(QObject *val, const char *name);
    bool setInt(int val, const char* name);
    bool setDouble(double val, const char* name);

  private:
    mu::Parser parser, rparser;
    QAsciiDict<double> variables;
    strDict columns, rowIndexes;
};

class muParserScripting: public ScriptingEnv
{
  Q_OBJECT

  public:
    muParserScripting(ApplicationWindow *parent) : ScriptingEnv(parent) { initialized=true; }

    bool isRunning() const { return true; }
    Script *newScript(const QString &code, QObject *context=0, const QString &name="<input>")
    {
      return new muParserScript(this, code, context, name);
    }

    // we do not support global variables
    bool setQObject(QObject *val, const char *name=0) { return false; }
    bool setInt(int val, const char* name) { return false; }
    bool setDouble(double val, const char* name) { return false; }
    
    const QStringList mathFunctions() const;
    const QString mathFunctionDoc (const QString &name) const;

    struct mathFunction
    {
      char *name;
      int numargs;
      double (*fun1)(double);
      double (*fun2)(double,double);
      double (*fun3)(double,double,double);
      char *description;
    };
    static const mathFunction math_functions[];

  private:
    static double bessel_J0(double x)
      { return gsl_sf_bessel_J0 (x); }
    static double bessel_J1(double x)
      { return gsl_sf_bessel_J1 (x); }
    static double bessel_Jn(double x, double n)
      { return gsl_sf_bessel_Jn ((int)n, x); }
    static double bessel_Yn(double x, double n)
      { return gsl_sf_bessel_Yn ((int)n, x); }
    static double bessel_Jn_zero(double n, double s)
      { return gsl_sf_bessel_zero_Jnu(n, (unsigned int) s); }
    static double bessel_Y0(double x)
      { return gsl_sf_bessel_Y0 (x); }
    static double bessel_Y1(double x)
      { return gsl_sf_bessel_Y1 (x); }
    static double beta(double a, double b)
      { return gsl_sf_beta (a,b); }
    static double erf(double x)
      { return gsl_sf_erf (x); }
    static double erfc(double x)
      { return gsl_sf_erfc (x); }
    static double erf_Z(double x)
      { return gsl_sf_erf_Z (x); }
    static double erf_Q(double x)
      { return gsl_sf_erf_Q (x); }
    static double gamma(double x)
      { return gsl_sf_gamma (x); }
    static double lngamma(double x)
      { return gsl_sf_lngamma (x); }
    static double hazard(double x)
      { return gsl_sf_hazard (x); }
};

#endif
