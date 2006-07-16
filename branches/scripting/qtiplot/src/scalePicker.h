#include <qobject.h>

class QwtPlot;
class QwtScaleWidget;
class QwtTextLabel;
class QLabel;
	
class ScalePicker: public QObject
{
    Q_OBJECT
public:
    ScalePicker(QwtPlot *plot);
    virtual bool eventFilter(QObject *, QEvent *);

	//! The rect of a scale without the title
	QRect scaleRect(const QwtScaleWidget *) const;

	void mouseDblClicked(const QwtScaleWidget *, const QPoint &);
	void mouseClicked(const QwtScaleWidget *scale, const QPoint &pos) ;
	void mouseRightClicked(const QwtScaleWidget *scale, const QPoint &pos);

	void refresh();
	
	QwtPlot *plot() {return (QwtPlot *)parent();};

signals:
	void clicked();

	void axisRightClicked(int);
	void axisTitleRightClicked(int);

	void axisDblClicked(int);
	void axisTitleDblClicked(int);

	void xAxisTitleDblClicked();
	void yAxisTitleDblClicked();
	void rightAxisTitleDblClicked();
	void topAxisTitleDblClicked();
	
	void moveGraph(const QPoint&);
	void releasedGraph();
	void highlightGraph();

private:
	bool movedGraph;
	QPoint presspos;
};

class TitlePicker: public QObject
{
    Q_OBJECT
public:
    TitlePicker(QwtPlot *plot);
    virtual bool eventFilter(QObject *, QEvent *);

signals:
	void clicked();
    void doubleClicked();
	void removeTitle();
	void showTitleMenu();

	// moving and highlighting the plot parent
	void moveGraph(const QPoint&);
	void releasedGraph();
	void highlightGraph();

protected:
	QwtTextLabel *title;
	bool movedGraph;
	QPoint presspos;
};
