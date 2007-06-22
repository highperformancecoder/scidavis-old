#include <qobject.h>
#include "plot.h"

class Graph;
	
class CanvasPicker: public QObject
{
    Q_OBJECT
public:
    CanvasPicker(Graph *plot);
    virtual bool eventFilter(QObject *, QEvent *);
	void selectPoints(int n);
	int selectedPoints(){return selected_points;};
	void selectPeak(const QPoint& p);

private:
	void drawTextMarker(const QPoint&);
	void drawLineMarker(const QPoint&, bool endArrow);

	//! Called when the user releases the mouse button after a line marker resize action
	/**
	 * \param point the mouse position
	*/
	void resizeLineMarker(const QPoint& point);

	//! Selects and highlights the marker 
	/**
	 * \param point the mouse position
	*/
	bool selectMarker(const QPoint& point);
	void moveMarker(QPoint& );
	void releaseMarker();

	Graph *plot() { return (Graph *)parent(); }
	
	Plot* plotWidget;	
	QPoint startLinePoint, endLinePoint;
	
	//! Tells if the user resizes a line marker via the mouse using the start point
	bool resizeLineFromStart;
	
	//! Tells if the user resizes a line marker via the mouse using the end point
	bool resizeLineFromEnd;	
	
signals:
	void showPieDialog();
	void showPlotDialog(int);
	void viewTextDialog();
	void viewLineDialog();
	void viewImageDialog();
	void drawTextOff();
	void showMarkerPopupMenu();
	void modified();
	void calculateProfile(const QPoint&, const QPoint&);
	void selectPlot();
	void moveGraph(const QPoint&);
	void releasedGraph();
	void highlightGraph();
	
private:
	QPoint presspos;
	int xMouse, yMouse, xMrk, yMrk, n_peaks, selected_points;
	bool moved,	movedGraph, pointSelected, select_peaks;
};
