#ifndef TABLE_STATISTICS_H
#define TABLE_STATISTICS_H

#include "worksheet.h"

//! Table that computes and displays statistics on another Table
class TableStatistics : public Table
{
  Q_OBJECT

  public:
    //! supported statistics types
    enum Type { row, column };
    TableStatistics(ScriptingEnv *env, QWidget *parent, Table *base, Type, QValueList<int> targets);
    //! return the type of statistics
    Type type() const { return d_type; }
    //! return the base table of which statistics are displayed
    Table *base() const { return d_base; }
    // saving
    virtual QString saveToString(const QString &geometry);

  public slots:
    //! update statistics after a column has changed (to be connected with Table::modifiedData)
    void update(Table*, const QString& colName);

  private:
    Table *d_base;
    Type d_type;
    QValueList<int> d_targets;
};

#endif

