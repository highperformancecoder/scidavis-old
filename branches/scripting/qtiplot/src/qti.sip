/* vim: set filetype=cpp : */

%Module qti 0
%Import qt/qtmod.sip

class ScriptEdit: QTextEdit
{
%TypeHeaderCode
#include "src/scriptedit.h"
%End
public slots:
    void execute();
    void executeAll();
    void evaluate();
    void print();
    void insertFunction(const QString &);
private:
    ScriptEdit(ScriptEdit&);
};

class myWidget: QWidget
{
%TypeHeaderCode
#include "src/widget.h"
%End
public:
  enum CaptionPolicy{Name = 0, Label = 1, Both = 2};

  const char* windowLabel();
  void setWindowLabel(const QString&);

  CaptionPolicy captionPolicy();
  void setCaptionPolicy(CaptionPolicy);

  virtual const char* saveAsTemplate(const QString& );
  virtual void restore(const QStringList& );

private:
  myWidget(const myWidget&);
};

class Table: myWidget
{
%TypeHeaderCode
#include "src/worksheet.h"

  inline int sipqti_colArg(Table *t, PyObject *c)
  {
    int col;
    PyObject *tmp;
    if (PyInt_Check(c))
      col = (int)PyInt_AsLong(c)-1;
    else
    {
      tmp = PyObject_Str(c);
      col = t->colNames().findIndex(PyString_AsString(tmp));
      Py_XDECREF(tmp);
    }
    return col;
  }
%End
  
public:
  int tableRows() /PyName=rows/;
  int tableCols() /PyName=cols/;
  void resizeRows(int) /PyName=setRows/;
  void resizeCols(int) /PyName=setCols/;

  const char* text(SIP_PYOBJECT, int);
%MethodCode
    sipRes = sipCpp->text(a1-1, sipqti_colArg(sipCpp, a0));
%End
  double cell(SIP_PYOBJECT, int);
%MethodCode
    sipRes = sipCpp->text(a1-1, sipqti_colArg(sipCpp, a0)).toDouble();
%End
  void setText(SIP_PYOBJECT, int, const char*);
%MethodCode
    sipCpp->setText(a1-1, sipqti_colArg(sipCpp, a0), a2);
%End
  void setCell(SIP_PYOBJECT, int, double);
%MethodCode
  int prec;
  char f;
  int col = sipqti_colArg(sipCpp, a0);
  sipCpp->columnNumericFormat(col, f, prec);
  sipCpp->setText(a1-1,col,QString::number(a2, f, prec));
%End
  const char* colName(int);
%MethodCode
    sipRes = sipCpp->colName(a0-1);
%End
  void setColName(SIP_PYOBJECT, const QString&);
%MethodCode
    sipCpp->setColName(sipqti_colArg(sipCpp, a0), *a1);
%End
  
private:
  Table(const Table&);
};

class LineMarker // : QwtPlotMarker
{
%TypeHeaderCode
#include "src/LineMarker.h"
%End
public:
  // virtual properties?
  QPoint startPoint() /PyName=start/;
  void setStartPoint(QPoint) /PyName=setStart/;
  QPoint endPoint() /PyName=end/;
  void setEndPoint(QPoint) /PyName=setEnd/;
private:
  LineMarker(const LineMarker&);
};

class Graph: QWidget
{
%TypeHeaderCode
#include "src/graph.h"
%End
  public:
  bool isPiePlot();
  const char* pieLegendText() /PyName=pieLegend/;

  bool insertCurve(Table*, const QString&, int);
  bool insertCurve(Table*, const QString&, const QString&, int);
  void removeCurve(int);
  void removeCurve(const QString&);
  int curves();

  void addErrorBars(Table *, const QString&, 
      Table *, const QString&,
      int, int, int, const QColor&,
      bool, bool,bool);

  void insertLineMarker(LineMarker*);
  LineMarker* lineMarker(long);
  private:
  Graph(const Graph&);
};

class MultiLayer: myWidget
{
%TypeHeaderCode
#include "src/multilayer.h"
%End
  public:
  Graph *activeGraph() /PyName=activeLayer/;
  void setActiveGraph(Graph*) /PyName=setActiveLayer/;
  // TODO: implement Graph* layer(int num);;
  private:
  MultiLayer(const MultiLayer&);
};

class Note: myWidget
{
%TypeHeaderCode
#include "src/note.h"
%End
  public:
  void execute() const;
  bool autoexec() const;
  private:
  Note(const Note&);
};

class ApplicationWindow: QMainWindow
{
%TypeHeaderCode
#include "src/application.h"
%End
  public:
  Table *table(const QString&);
  Table *newTable(const QString&, int, int);
  MultiLayer *plot(const QString&);
  Note *note(const QString&);
  QTextEdit *results;
  QTextEdit *console;
  private:
  ApplicationWindow(const ApplicationWindow&);
};

