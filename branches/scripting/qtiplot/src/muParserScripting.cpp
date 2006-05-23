#include "muParserScripting.h"
#include "worksheet.h"

#include <qstringlist.h>
#include <qasciidict.h>
#include <qmap.h>

using namespace mu;

const muParserScripting::mathFunction muParserScripting::math_functions[] = {
  { "abs", 1, NULL,NULL,NULL, "abs(x):\n Absolute value of x." },
  { "acos", 1, NULL,NULL,NULL, "acos(x):\n Inverse cos function." },
  { "acosh", 1, NULL,NULL,NULL, "acosh(x):\n Hyperbolic inverse cos function." },
  { "asin", 1, NULL,NULL,NULL, "asin(x):\n Inverse sin function." },
  { "asinh", 1, NULL,NULL,NULL, "asinh(x):\n Hyperbolic inverse sin function." },
  { "atan", 1, NULL,NULL,NULL, "atan(x):\n Inverse tan function." },
  { "atanh", 1, NULL,NULL,NULL, "atanh(x):\n  Hyperbolic inverse tan function." },
  { "avg", -1, NULL,NULL,NULL, "avg(x,y,...):\n  Mean value of all arguments." },
  { "bessel_j0", 1, bessel_J0,NULL,NULL, "bessel_j0(x):\n  Regular cylindrical Bessel function of zeroth order, J_0(x)." },
  { "bessel_j1", 1, bessel_J1,NULL,NULL, "bessel_j1(x):\n  Regular cylindrical Bessel function of first order, J_1(x)." },
  { "bessel_jn", 2, NULL,bessel_Jn,NULL, "bessel_jn(double x, int n):\n Regular cylindrical Bessel function of order n, J_n(x)." },
  { "bessel_jn_zero", 2, NULL,bessel_Jn_zero,NULL, "bessel_jn_zero(double n, unsigned int s):\n s-th positive zero x_s of regular cylindrical Bessel function of order n, J_n(x_s)=0" },
  { "bessel_y0", 1, bessel_Y0, NULL,NULL, "bessel_y0(x):\n Irregular cylindrical Bessel function of zeroth order, Y_0(x), for x>0." },
  { "bessel_y1", 1, bessel_Y1, NULL,NULL, "bessel_y1(x):\n Irregular cylindrical Bessel function of first order, Y_1(x), for x>0." },
  { "bessel_yn", 2, NULL,bessel_Yn,NULL, "bessel_yn(double x, int n):\n Irregular cylindrical Bessel function of order n, Y_n(x), for x>0." },
  { "beta", 2, NULL,beta,NULL, "beta (a,b):\n Computes the Beta Function, B(a,b) = Gamma(a)*Gamma(b)/Gamma(a+b) for a > 0, b > 0." },
  { "cos", 1, NULL,NULL,NULL, "cos (x):\n Calculate cosine." },
  { "cosh", 1, NULL,NULL,NULL, "cosh(x):\n Hyperbolic cos function." },
  { "erf", 1, erf, NULL,NULL, "erf(x):\n  The error function." },
  { "erfc", 1, erfc, NULL,NULL, "erfc(x):\n Complementary error function erfc(x) = 1 - erf(x)." },
  { "erfz", 1, erf_Z, NULL,NULL, "erfz(x):\n The Gaussian probability density function Z(x)." },
  { "erfq", 1, erf_Q, NULL,NULL, "erfq(x):\n The upper tail of the Gaussian probability function Q(x)." },
  { "exp", 1, NULL,NULL,NULL, "exp(x):\n Exponential function: e raised to the power of x." },
  { "gamma", 1, gamma, NULL,NULL, "gamma(x):\n Computes the Gamma function, subject to x not being a negative integer." },
  { "gammaln", 1, lngamma,NULL,NULL, "gammaln(x):\n Computes the logarithm of the Gamma function, subject to x not a being negative integer. For x<0, log(|Gamma(x)|) is returned." },
  { "hazard", 1, hazard,NULL,NULL, "hazard(x):\n Computes the hazard function for the normal distribution h(x) = erfz(x)/erfq(x)." },
  { "if", 3, NULL,NULL,NULL, "if(e1, e2, e3):	if e1 then e2 else e3." },
  { "ln", 1, NULL,NULL,NULL, "ln(x):\n Calculate natural logarithm." },
  { "log", 1, NULL,NULL,NULL, "log(x):\n Calculate decimal logarithm." },
  { "log2", 1, NULL,NULL,NULL, "log2(x):\n Calculate 	logarithm to the base 2." },
  { "min", -1, NULL,NULL,NULL, "min(x,y,...):\n Calculate minimum of all arguments." },
  { "max", -1, NULL,NULL,NULL, "max(x,y,...):\n Calculate maximum of all arguments." },
  { "rint", 1, NULL,NULL,NULL, "rint(x):\n Round to nearest integer." },
  { "sign", 1, NULL,NULL,NULL, "sign(x):\n Sign function: -1 if x<0; 1 if x>0." },
  { "sin", 1, NULL,NULL,NULL, "sin(x):\n Calculate sine." },
  { "sinh", 1, NULL,NULL,NULL, "sinh(x):\n Hyperbolic sin function." },
  { "sqrt", 1, NULL,NULL,NULL, "sqrt(x):\n Square root function." },
  { "tan", 1, NULL,NULL,NULL, "tan(x):\n Calculate tangent function." },
  { "tanh", 1, NULL,NULL,NULL, "tanh(x):\n Hyperbolic tan function." },
  {0,0,NULL,NULL,NULL,0}
};
  
muParserScript::muParserScript(ScriptingEnv *env, const QString &code, QObject *context, const QString &name)
  : Script(env, code, context, name)
{
  variables.setAutoDelete(true);

  parser.DefineConst("pi", M_PI);
  parser.DefineConst("Pi", M_PI);
  parser.DefineConst("PI", M_PI);

  for (const muParserScripting::mathFunction *i=muParserScripting::math_functions; i->name; i++)
    if (i->numargs == 1 && i->fun1 != NULL)
      parser.DefineFun(i->name, i->fun1);
    else if (i->numargs == 2 && i->fun2 != NULL)
      parser.DefineFun(i->name, i->fun2);
    else if (i->numargs == 3 && i->fun3 != NULL)
      parser.DefineFun(i->name, i->fun3);

  rparser = parser;
}

bool muParserScript::setDouble(double val, const char *name)
{
  double *valptr = variables[name];
  if (!valptr)
  {
    valptr = new double;
    if (!valptr) return false;
    variables.insert(name, valptr);
    parser.DefineVar(name, valptr);
    rparser.DefineVar(name, valptr);
  }
  *valptr = val;
  return  true;
}

bool muParserScript::setInt(int val, const char *name)
{
  return setDouble((double) val, name);
}

bool muParserScript::setQObject(QObject *val, const char *name)
{
  return false;
}

bool muParserScript::compile()
{
  QString aux(Code);
  aux.remove("\n");
  QStringList variables;

  if (Context->isA("Table"))
  {
    Table *table = (Table*) Context;
    // col() hack
    int pos1, pos2, pos3;
    QStringList colNames=table->colNames();
    int n = aux.contains("col(");
    for (int i=0; i<n; i++)
    {
      pos1=aux.find("col(",0,TRUE);
      pos2=pos1+3;
      pos3=aux.find(")",pos2+1);

      QString aux2=aux.mid(pos2+1,pos3-pos2-1);
      if (aux2.contains("col("))
      {
	emit_error(tr("You can not use imbricated columns!"), 0);
	compiled = Script::compileErr;
	return false;
      }

      QStringList items=QStringList::split(",", aux2, FALSE);
      // allow both name and "name" for compatibility
      if (items[0].startsWith("\"") && items[0].endsWith("\""))
	items[0] = items[0].mid(1,items[0].length()-2);
      int index=colNames.findIndex (items[0]);
      QString s="c"+ QString::number(i)+"_"+QString::number(index);
      if (items.count() == 2)
	rowIndexes[s] = items[1];
      else
	rowIndexes[s] = "i";
      aux.replace(pos1,4+pos3-pos2,s);
      columns[s] = "col("+aux2+")";
    }
  }
 
  parser.SetExpr(aux.ascii());
  compiled = Script::isCompiled;
  return true;
}

QVariant muParserScript::eval()
{
  if(compiled != Script::isCompiled && !compile())
    return QVariant();
  double val;
  try {
    if (Context->isA("Table"))
    {
      Table *table = (Table*) Context;
      if (!variables["i"]) return QVariant();
      for (strDict::iterator i=rowIndexes.begin(); i!=rowIndexes.end(); i++) {
	int row;
	if (i.data() != "i")
	{
	  rparser.SetExpr(i.data());
	  row = qRound(rparser.Eval()) - 1;
	} else
	  row = (int) *(variables["i"]) - 1;
	if (row < 0 || row >= table->tableRows())
	  return QVariant();
	QStringList list = QStringList::split("_", i.key(), false);
	int col = list[1].toInt();
	if (table->text(row,col).isEmpty()) return QVariant("");
	setDouble((table->text(row,col)).toDouble(), i.key().ascii());
      }
    }
    val = parser.Eval();
  } catch (mu::ParserError &e) {
    QString errString=e.GetMsg();
    for (strDict::iterator i=columns.begin(); i!=columns.end(); i++)
      errString.replace(i.key(), i.data(), true);
    emit_error(errString, 0);
    return QVariant();
  }
  return QVariant(val);
}

bool muParserScript::exec()
{
  QStringList lines = QStringList::split("\n", Code);
  for (int i=0; i<lines.size(); i++)
  {
    QStringList kv = QStringList::split("=", lines[i]);
    if (kv.size() == 1) break;
    if (kv.size() != 2)
    {
      emit_error(tr("Too many '=' in one line."),i+1);
      return false;
    }
    Code = kv[1];
    if(!compile())
      return false;
    double val;
    try {
      val = parser.Eval();
    } catch (mu::ParserError &e) {
      QString errString=e.GetMsg();
      emit_error(errString, 0);
      return false;
    }
    if (!setDouble(val, kv[0])) return false;
  }
  Code = lines.join("\n");
  return true;
}
 

const QStringList muParserScripting::mathFunctions() const
{
  QStringList l;
  for (const mathFunction *i=math_functions; i->name; i++)
    l << i->name;
  return l;
}

const QString muParserScripting::mathFunctionDoc (const QString &name) const
{
  for (const mathFunction *i=math_functions; i->name; i++)
    if (name==i->name)
      return i->description;
  return QString::null;
}
