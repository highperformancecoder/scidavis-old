/***************************************************************************
    File                 : MuParserScript.h
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2009 Knut Franke
    Email (use @ for *)  : Knut.Franke*gmx.net
    Description          : Evaluate mathematical expressions using muParser

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

#ifndef MU_PARSER_SCRIPT_H
#define MU_PARSER_SCRIPT_H

#include "Script.h"
#include "QStringStdString.h"
#include <QtCore/QMap>
#include <muParser.h>

class QByteArray;
class Column;

class MuParserScript : public Script
{
  Q_OBJECT

  struct MuException: public mu::Parser::exception_type
  {
    MuException(const QString& x):
      mu::Parser::exception_type(toString<mu::string_type>(x)) {}
  };
  
public:
  MuParserScript(ScriptingEnv *environment, const QString &code, QObject *context=0, const QString &name="<input>");

public slots:
  bool compile(bool asFunction = true)  override;
  QVariant eval() override;
  bool exec() override { return eval().isValid(); }
  bool setQObject(QObject *value, const char *name) override { Q_UNUSED(value); Q_UNUSED(name); return false; }
  bool setDouble(double value, const char *name) override;
  bool setInt(int value, const char *name) override { return setDouble(static_cast<double>(value), name); }

private:
  static double *variableFactory(const mu::string_type::value_type *name, void *self);
  static double statementSeparator(double a, double b);
  static double tableColumnFunction(const mu::string_type::value_type *columnPath);
  static double tableColumn_Function(double columnIndex);
  static double tableColumn__Function(const mu::string_type::value_type *tableName, double columnIndex);
  static double tableCellFunction(const mu::string_type::value_type *columnPath, double rowIndex);
  static double tableCell_Function(double columnIndex, double rowIndex);
  static double matrixCellFunction(double rowIndex, double columnIndex);

protected:
  Column *resolveColumnPath(const QString &path);
  bool translateLegacyFunctions(QString &input);

private:
  mu::Parser m_parser;
  QMap<QString, double> m_variables;

  static MuParserScript *s_currentInstance;
};

#endif // ifndef MU_PARSER_SCRIPT_H
