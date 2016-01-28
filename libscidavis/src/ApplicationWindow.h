/***************************************************************************
    File                 : ApplicationWindow.h
    Project              : SciDAVis
    Description          : SciDAVis's main window
    --------------------------------------------------------------------
    Copyright            : (C) 2006-2009 Knut Franke (knut.franke*gmx.de)
    Copyright            : (C) 2006-2009 Tilman Benkert (thzs*gmx.net)
    Copyright            : (C) 2004-2007 by Ion Vasilief (ion_vasilief*yahoo.fr)
                           (replace * with @ in the email address)

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
#ifndef APPLICATION_H
#define APPLICATION_H

#include <QMainWindow>
#include <Q3ListView>
#include <Q3Header>
#ifdef SEARCH_FOR_UPDATES
#include <QHttp>
#endif
#include <QFile>
#include <QSplitter>
#include <QDesktopServices>
#include <QBuffer>
#include <QLocale>

#include "Table.h"
#include "ScriptingEnv.h"
#include "Script.h"

class QPixmap;
class QCloseEvent;
class QDropEvent;
class QTimerEvent;
class QDragEnterEvent;
class QTranslator;
class QDockWidget;
class QAction;
class QActionGroup;
class QWorkspace;
class QLineEdit;
class QTranslator;
class QToolButton;
class QShortcut;
class QMenu;
class QToolBar;
class QStatusBar;
class QSignalMapper;

class Matrix;
class Table;
class Graph;
class ScalePicker;
class Graph3D;
class Note;
class MultiLayer;
class FunctionDialog;
class Folder;
class FolderListItem;
class FolderListView;
class Plot3DDialog;
class MyWidget;
class TableStatistics;
class CurveRangeDialog;
class Project;
class AbstractAspect;

#ifndef TS_PATH
#define TS_PATH (qApp->applicationDirPath() + "/translations")
#endif

/**
 * \brief SciDAVis's main window.
 *
 * This class contains the main part of the user interface as well as the central project management facilities.
 *
 * It manages all MyWidget MDI Windows in a project, knows about their organization in Folder objects
 * and contains the parts of the project explorer not implemented in Folder, FolderListItem or FolderListView.
 *
 * Furthermore, it is responsible for displaying most MDI Windows' context menus and opening all sorts of dialogs.
 *
 * \section future_plans Future Plans
 * Split out the project management part into a new Project class.
 * If MyWidget maintains a reference to its parent Project, it should be possible to have its subclasses
 * display their own context menus and dialogs.
 * This is necessary for implementing new plot types or even completely new MyWidget subclasses in plug-ins.
 * It will also make ApplicationWindow more manageable by removing those parts not directly related to the main window.
 *
 * Project would also take care of basic project file reading/writing (using Qt's XML framework), but delegate most of
 * the work to MyWidget and its subclasses. This is necessary for providing save/restore of classes implemented in
 * plug-ins. Support for foreign formats on the other hand could go into import/export classes (which could also be
 * implemented in plug-ins). Those would interface directly with Project and the MyWidgets it manages. Thus, in addition
 * to supporting QtXML-based save/restore, Project, MyWidget and subclasses will also have to provide generalized
 * save/restore methods/constructors.
 *
 * Maybe split out the project explorer into a new ProjectExplorer class, depending on how much code is left
 * in ApplicationWindow after the above reorganizations. Think about whether a Model/View approach can be
 * used for Project/ProjectExplorer.
 */
class ApplicationWindow: public QMainWindow, public scripted
{
    Q_OBJECT
public:
    ApplicationWindow();
	ApplicationWindow(const QStringList& l);
	~ApplicationWindow();

	enum ShowWindowsPolicy{HideAll, ActiveFolder, SubFolders};

	QTranslator *appTranslator, *qtTranslator;
	QDockWidget *logWindow;
	QDockWidget *explorerWindow;
	QTextEdit *results;
#ifdef SCRIPTING_CONSOLE
	QDockWidget *consoleWindow;
	QTextEdit *console;
#endif
	QWorkspace* d_workspace;
	QToolBar *file_tools, *graph_tools, *table_tools, *plot_tools, *graph_3D_tools, *edit_tools, *matrix_plot_tools;
    FolderListView *lv;
    FolderListView *folders;
	QToolButton *btnResults;
    QWidgetList *hiddenWindows;
    QWidgetList *outWindows;
	QWidget *lastModified;

public:
	/*! Generates a new unique name starting with string /param name.
	You can force the output to be a name different from /param name,
	even if 'name' is not used in the project, by setting /param increment = true (the default)
	*/
	QString generateUniqueName(const QString& name, bool increment = true);

public slots:
	//! Copy the status bar text to the clipboard
	void copyStatusBarText();
	//! Show the context menu for the status bar
	void showStatusBarContextMenu( const QPoint & pos );
	//! \name Projects and Project Files
	//@{
	void open();
	ApplicationWindow* open(const QString& fn);
	//! Returns temporary file ready for reading uncompressed content
	/**
	 * Close and delete after you're done with it.
	 */
	QFile * openCompressedFile(const QString& fn);
	ApplicationWindow* openProject(const QString& fn);
	ApplicationWindow* importOPJ(const QString& filename);
	void showHistory();

	/**
	 * \brief Create a new project from a data file.
	 *
	 * \param fn is read as a data file with the default column separator (as set by the user)
	 * and inserted as a table into a new, empty project.
	 * This table is then plotted with the Graph::LineSymbols style.
	 */
	ApplicationWindow * plotFile(const QString& fn);

	/**
	 * \brief Create a new project from a script file.
	 *
	 * \param fn is read as a Python script file and loaded in the command script window.
	 * \param execute specifies if the script should be executed after opening.
	 */
	ApplicationWindow * loadScript(const QString& fn, bool execute = false);

	QWidgetList * windowsList();
	void updateWindowLists(MyWidget *w);
	/*!
    Arranges all the visible project windows in a cascade pattern.
    */
	void cascade();

	void saveProjectAs();
	bool saveProject();

	//! Set the project status to modifed
	void modifiedProject();
	//! Set the project status to saved (not modified)
	void savedProject();
	//! Set the project status to modified and save 'w' as the last modified widget
	void modifiedProject(QWidget *w);
	//@}

	//! \name Settings
	//@{
	void readSettings();
	void saveSettings();
	void applyUserSettings();
	void setSaveSettings(bool autoSaving, int min);
	void changeAppStyle(const QString& s);
	void changeAppFont(const QFont& f);
	void updateAppFonts();
	void setAppColors(const QColor& wc,const QColor& pc,const QColor& tpc);
	//@}

	//! \name Multilayer Plots
	//@{
	MultiLayer* multilayerPlot(int c, int r, int style);
	MultiLayer* multilayerPlot(Table* w, const QStringList& colList, int style, int startRow = 0, int endRow = -1);
	//! used when restoring a plot from a project file
	MultiLayer* multilayerPlot(const QString& caption);
	//! used by the plot wizard
	MultiLayer* multilayerPlot(const QStringList& colList);
	void connectMultilayerPlot(MultiLayer *g);
	void addLayer();
	void deleteLayer();

	//! Creates a new spectrogram graph
  	MultiLayer* plotSpectrogram(Matrix *m, Graph::CurveType type);
  	void plotGrayScale();
  	MultiLayer* plotGrayScale(Matrix *m);
  	void plotContour();
  	MultiLayer* plotContour(Matrix *m);
  	void plotColorMap();
  	MultiLayer* plotColorMap(Matrix *m);

	//! Rearrange the layersin order to fit to the size of the plot window
  	void autoArrangeLayers();
	void initMultilayerPlot(MultiLayer* g, const QString& name);
	void initBareMultilayerPlot(MultiLayer* g, const QString& name);
	void polishGraph(Graph *g, int style);
	void plot2VerticalLayers();
	void plot2HorizontalLayers();
	void plot4Layers();
	void plotStackedLayers();
	void plotStackedHistograms();
	//@}

	//! \name 3D Data Plots
	//@{
	Graph3D* openMatrixPlot3D(const QString& caption, const QString& matrix_name,
							 double xl,double xr,double yl,double yr,double zl,double zr);
	Graph3D* dataPlot3D(Table* table,const QString& colName);
	Graph3D* dataPlotXYZ(Table* table,const QString& zColName, int type);
	Graph3D* dataPlot3D(const QString& caption,const QString& formula,
						double xl, double xr, double yl, double yr, double zl, double zr);
	Graph3D* dataPlotXYZ(const QString& caption,const QString& formula,
 						double xl, double xr, double yl, double yr, double zl, double zr);
	//@}

	//! \name Surface Plots
	//@{
	Graph3D* newPlot3D();
	Graph3D* newPlot3D(const QString& formula, double xl, double xr,
					   double yl, double yr, double zl, double zr);
	Graph3D* newPlot3D(const QString& caption,const QString& formula,
					   double xl, double xr,double yl, double yr, double zl, double zr);
	void connectSurfacePlot(Graph3D *plot);
	void newSurfacePlot();
	void editSurfacePlot();
	void remove3DMatrixPlots(Matrix *m);
	void updateMatrixPlots(QWidget *);
	void add3DData();
	void change3DData();
	void change3DData(const QString& colName);
	void change3DMatrix();
	void change3DMatrix(const QString& matrix_name);
	void insertNew3DData(const QString& colName);
	void add3DMatrixPlot();
	void insert3DMatrixPlot(const QString& matrix_name);
	void initPlot3D(Graph3D *plot);
	void customPlot3D(Graph3D *plot);
	void setPlot3DOptions();

	void plot3DWireframe();
	void plot3DHiddenLine();
	void plot3DPolygons();
	void plot3DWireSurface();

	void plot3DMatrix(int style);

	void plot3DRibbon();
	void plot3DScatter();
	void plot3DTrajectory();
	void plot3DBars();
	//@}

	//! \name User-defined Functions
	//@{
	bool newFunctionPlot(int type,QStringList &formulas, const QString& var,QList<double> &ranges, int points);

	FunctionDialog* functionDialog();
	void showFunctionDialog();
	void showFunctionDialog(Graph * g, int curve);
	void addFunctionCurve();
	void clearFitFunctionsList();
	void saveFitFunctionsList(const QStringList& l);
	void clearSurfaceFunctionsList();
	void clearLogInfo();
	void clearParamFunctionsList();
	void clearPolarFunctionsList();
	void updateFunctionLists(int type, QStringList &formulas);
	void updateSurfaceFuncList(const QString& s);
	//@}

	//! \name Matrices
	//@{
	//! Creates a new empty matrix
	Matrix* newMatrix(int rows = 32, int columns = 32);
	//! To be used when opening a project file only!
	Matrix* newMatrix(const QString& caption, int r, int c);
	Matrix* matrix(const QString& name);
	Matrix* convertTableToMatrix();
	void initMatrix(Matrix* m);
	void invertMatrix();
	void matrixDeterminant();
	//@}

	//! \name Tables
	//@{
	//! Creates an empty table
	Table* newTable();
	//! Used when importing an ASCII file
	Table* newTable(const QString& fname, const QString &sep, int lines,
		            bool renameCols, bool stripSpaces, bool simplifySpaces,
						bool convertToNumeric, QLocale numericLocale);
	//! Used when loading a table from a project file
	Table* newTable(const QString& caption,int r, int c);
	Table* newTable(int r, int c, const QString& name = QString(),const QString& legend = QString());
	Table* newTable(const QString& name, const QString& legend, QList<Column *> columns);
	/**
	 * \brief Create a Table which is initially hidden; used to return the result of an analysis operation.
	 *
	 * \param name window name (compare MyWidget::MyWidget)
	 * \param label window label (compare MyWidget::MyWidget)
	 * \param r number of rows
	 * \param c number of columns
     * \param text tab/newline - seperated initial content; may be empty
	 */
	Table* newHiddenTable(const QString& name, const QString& label, QList<Column *> columns);
	Table* table(const QString& name);
	Table* convertMatrixToTable();
	QWidgetList* tableList();

	void connectTable(Table* w);
	void newWrksheetPlot(const QString& name,const QString& label, QList<Column *> columns);
	void initTable(Table* w);
	void customTable(Table* w);
	void customizeTables(const QColor& bgColor,const QColor& textColor,
						const QColor& headerColor,const QFont& textFont,
						const QFont& headerFont, bool showComments);

	void importASCII();
	void importASCII(const QStringList& files, int import_mode, const QString& local_column_separator, int local_ignored_lines,
		 bool local_rename_columns, bool local_strip_spaces, bool local_simplify_spaces, bool local_convert_to_numeric, QLocale local_numeric_locale);
	void exportAllTables(const QString& sep, bool colNames, bool expSelection);
	void exportASCII(const QString& tableName, const QString& sep, bool colNames, bool expSelection);

	TableStatistics *newTableStatistics(Table *base, int type, QList<int>,
	    const QString &caption=QString::null);
	//@}

	//! \name Graphs
	//@{
	void setPreferences(Graph* g);
	void setGraphDefaultSettings(bool autoscale,bool scaleFonts,bool resizeLayers,bool antialiasing);
	void setLegendDefaultSettings(int frame, const QFont& font,
							 const QColor& textCol, const QColor& backgroundCol);
	void setArrowDefaultSettings(int lineWidth,  const QColor& c, Qt::PenStyle style,
								int headLength, int headAngle, bool fillHead);
	void plotPie();
	void plotVectXYXY();
	void plotVectXYAM();
	//@}

	//! \name Image Analysis
	//@{
	void intensityTable();
	void pixelLineProfile();
	void loadImage();
	void loadImage(const QString& fn);
	Matrix* importImage();
  	Matrix* importImage(const QString& fn);
	//@}

	//! \name Export and Print
	//@{
	void exportLayer();
	void exportGraph();
	void exportAllGraphs();
	void exportPDF();
	void print();
	void print(QWidget* w);
	void printAllPlots();
	//@}

	QStringList columnsList(SciDAVis::PlotDesignation plotType);
	QStringList columnsList();

	void undo();
	void redo();

	//! \name MDI Windows
	//@{
	MyWidget* clone();
	MyWidget* clone(MyWidget*);
	void renameActiveWindow();

	//!  Called when the user presses F2 and an item is selected in lv.
	void renameWindow(Q3ListViewItem *item, int, const QString &s);

	//!  Checks weather the new window name is valid and modifies the name.
	bool renameWindow(MyWidget *w, const QString &text);

	void maximizeWindow(Q3ListViewItem * lbi);
	void maximizeWindow();
	void minimizeWindow();
    //! Changes the geometry of the active MDI window
    void setWindowGeometry(int x, int y, int w, int h);

	void updateWindowStatus(MyWidget* );

	bool hidden(QWidget* window);
	void closeActiveWindow();
	void closeWindow(MyWidget* window);

	//!  Does all the cleaning work before actually deleting a window!
	void removeWindowFromLists(MyWidget* w);

	void hideWindow(MyWidget* window);
	void hideActiveWindow();
	void activateWindow();
	void activateWindow(MyWidget *);
	void printWindow();
	//@}

	//! Show about dialog
	static void about();
	//! Return a version string ("SciDAVis x.y.z")
	static QString versionString();
	void windowsMenuAboutToShow();
	void windowsMenuActivated( int id );
	void removeCurves(const QString& name);
	QStringList dependingPlots(const QString& caption);
	QStringList depending3DPlots(Matrix *m);
	QStringList multilayerDependencies(QWidget *w);

	void saveAsTemplate();
	void openTemplate();

	QString windowGeometryInfo(MyWidget *w);
	void restoreWindowGeometry(ApplicationWindow *app, MyWidget *w, const QString s);

	void resizeActiveWindow();
	void resizeWindow();

	//! \name List View in Project Explorer
	//@{
	void setListView(const QString& caption,const QString& view);
	void renameListViewItem(const QString& oldName,const QString& newName);
	void setListViewDate(const QString& caption,const QString& date);
	QString listViewDate(const QString& caption);
	void setListViewLabel(const QString& caption,const QString& label);
	//@}

	void updateColNames(const QString& oldName, const QString& newName);
	void updateTableNames(const QString& oldName, const QString& newName);
	void changeMatrixName(const QString& oldName, const QString& newName);
	void updateCurves(Table *t, const QString& name);

	void showTable(const QString& curve);

	void addColToTable();
	void cutSelection();
	void copySelection();
	void copyMarker();
	void pasteSelection();
	void clearSelection();
	void copyActiveLayer();

	void newProject();

	//! Creates a new empty multilayer plot
	MultiLayer* newGraph(const QString& caption = tr("Graph"));

	//! \name Reading from a Project File
	//@{
	Matrix* openMatrix(ApplicationWindow* app, const QStringList &flist);
	Table* openTable(ApplicationWindow* app, QTextStream &stream);
	TableStatistics* openTableStatistics(const QStringList &flist);
	Graph3D* openSurfacePlot(ApplicationWindow* app, const QStringList &lst);
	Graph* openGraph(ApplicationWindow* app, MultiLayer *plot, const QStringList &list);

	void openRecentProject();
	//@}

	//! \name Initialization
	//@{
	void insertTranslatedStrings();
	void translateActionsStrings();
    void initFonts();
	void createActions();
	void initMainMenu();
	void initPlotMenu();
	void initTableAnalysisMenu();
	void initPlotDataMenu();
	void initToolBars();
	void initPlot3DToolBar();
	void disableActions();
	void hideToolbars();
	void customToolBars(QWidget* w);
	void customMenu(QWidget* w);
	void windowActivated(QWidget *w);
	//@}

	//! \name Table Tools
	//@{
	void correlate();
	void autoCorrelate();
	void convolute();
	void deconvolute();
	void clearTable();
	//@}

	//! \name Plot Tools
	//@{
	void newLegend();
	void addTimeStamp();
	void drawLine();
	void drawArrow();
	void addText();
	void disableAddText();
	void addImage();
	void zoomIn();
	void zoomOut();
	void setAutoScale();
	void showRangeSelectors();
	void showCursor();
	void showScreenReader();
	void pickPointerCursor();
	void pickDataTool( QAction* action );

	void updateLog(const QString& result);
	//@}

	//! \name Fitting
	//@{
	void deleteFitTables();
	void fitLinear();
	void fitSigmoidal();
	void fitGauss();
	void fitLorentz();
	void fitMultiPeak(int profile);
	void fitMultiPeakGauss();
	void fitMultiPeakLorentz();
	//@}

	//! \name Calculus
	//@{
	void differentiate();
	void analysis(const QString& whichFit);
	void analyzeCurve(Graph *g, const QString& whichFit, const QString& curveTitle);
	void showDataSetDialog(const QString& whichFit);
	//@}

	void addErrorBars();
	void defineErrorBars(const QString& name,int type,const QString& percent,int direction);
	void defineErrorBars(const QString& curveName,const QString& errColumnName, int direction);
	void movePoints();
	void removePoints();

	//! \name Event Handlers
	//@{
	void closeEvent( QCloseEvent*);
	void timerEvent ( QTimerEvent *e);
	void dragEnterEvent( QDragEnterEvent* e );
	void dropEvent( QDropEvent* e );
	void customEvent( QEvent* e);
	//@}

	//! \name Dialogs
	//@{
	void showFindDialogue();
	//! Show plot style dialog for the active MultiLayer / activeGraph / specified curve or the activeGraph options dialog if no curve is specified (curveKey = -1).
	void showPlotDialog(int curveKey = -1);
	QDialog* showScaleDialog();
	QDialog* showPlot3dDialog();
	AxesDialog* showScalePageFromAxisDialog(int axisPos);
	AxesDialog* showAxisPageFromAxisDialog(int axisPos);
	void showAxisDialog();
	void showGridDialog();
	void showGeneralPlotDialog();
	void showResults(bool ok);
	void showResults(const QString& s, bool ok=true);
	void showTextDialog();
	void showLineDialog();
	void showTitleDialog();
	void showExportASCIIDialog();
	void showCurvesDialog();
	void showCurveRangeDialog();
	CurveRangeDialog* showCurveRangeDialog(Graph *g, int curve);
	void showPlotAssociations(int curve);

	void showXAxisTitleDialog();
	void showYAxisTitleDialog();
	void showRightAxisTitleDialog();
	void showTopAxisTitleDialog();

	void showGraphContextMenu();
	void showLayerButtonContextMenu();
	void showWindowContextMenu();
	void showWindowTitleBarMenu();
	void showCurveContextMenu(int curveKey);
	void showCurvePlotDialog();
	void showCurveWorksheet();
    void showCurveWorksheet(Graph *g, int curveIndex);
	void showWindowPopupMenu(Q3ListViewItem *it, const QPoint &p, int);

	//! Connected to the context menu signal from lv; it's called when there are several items selected in the list
	void showListViewSelectionMenu(const QPoint &p);

	//! Connected to the context menu signal from lv; it's called when there are no items selected in the list
	void showListViewPopupMenu(const QPoint &p);

	void showMoreWindows();
	void showMarkerPopupMenu();
	void showPlotWizard();
	void showFitPolynomDialog();
	void showIntegrationDialog();
	void showInterpolationDialog();
	void showExpGrowthDialog();
	void showExpDecayDialog();
	void showExpDecayDialog(int type);
	void showTwoExpDecayDialog();
	void showExpDecay3Dialog();
	void showRowStatistics();
	void showColStatistics();
	void showFitDialog();
	void showImageDialog();
	void showPlotGeometryDialog();
	void showLayerDialog();
	void showPreferencesDialog();
	void showSmoothSavGolDialog();
	void showSmoothFFTDialog();
	void showSmoothAverageDialog();
    void showSmoothDialog(int m);
	void showFilterDialog(int filter);
	void lowPassFilterDialog();
	void highPassFilterDialog();
	void bandPassFilterDialog();
	void bandBlockFilterDialog();
	void showFFTDialog();
	//@}

	void translateCurveHor();
	void translateCurveVert();

	//! Removes the curve identified by a key stored in the data() of actionRemoveCurve.
	void removeCurve();
	void hideCurve();
	void hideOtherCurves();
	void showAllCurves();
	void setCurveFullRange();

	void updateConfirmOptions(bool askTables, bool askMatrixes, bool askPlots2D, bool askPlots3D, bool askNotes);

	//! \name Plot3D Tools
	//@{
	void toggle3DAnimation(bool on = true);
	 //! Turns perspective mode on or off
  	void togglePerspective(bool on = true);
  	//! Resets rotation of 3D plots to default values
  	void resetRotation();
  	//! Finds best layout for the 3D plot
  	void fitFrameToLayer();
	void setFramed3DPlot();
	void setBoxed3DPlot();
	void removeAxes3DPlot();
	void removeGrid3DPlot();
	void setHiddenLineGrid3DPlot();
	void setLineGrid3DPlot();
	void setPoints3DPlot();
	void setCrosses3DPlot();
	void setCones3DPlot();
	void setBars3DPlot();
	void setFilledMesh3DPlot();
	void setEmptyFloor3DPlot();
	void setFloorData3DPlot();
	void setFloorIso3DPlot();
	void setFloorGrid3DPlot(bool on);
	void setCeilGrid3DPlot(bool on);
	void setRightGrid3DPlot(bool on);
	void setLeftGrid3DPlot(bool on);
	void setFrontGrid3DPlot(bool on);
	void setBackGrid3DPlot(bool on);
	void pickPlotStyle( QAction* action );
	void pickCoordSystem( QAction* action);
	void pickFloorStyle( QAction* action);
	void custom3DActions(QWidget *w);
	void custom3DGrids(int grids);
	//@}

	void updateRecentProjectsList();

#ifdef SEARCH_FOR_UPDATES
	//!  connected to the done(bool) signal of the http object
	void receivedVersionFile(bool error);
	//!  called when the user presses the actionCheckUpdates
	void searchForUpdates();
#endif

	//! Open SciDAVis homepage in external browser
	void showHomePage();
	//! Open forums page at SF.net in external browser
	void showForums();
	//! Open bug tracking system at SF.net in external browser
	void showBugTracker();
#ifdef DOWNLOAD_LINKS
	//! Show download page in external browser
	void downloadManual();
#endif

	void parseCommandLineArguments(const QStringList& args);
	void createLanguagesList();
	void switchToLanguage(int param);
	void switchToLanguage(const QString& locale);

	bool alreadyUsedName(const QString& label);
	bool projectHas2DPlots();
	bool projectHas3DPlots();
	bool projectHasMatrices();

	//! Returns a pointer to the window named "name"
	QWidget* window(const QString& name);

	//! Returns a list with the names of all the matrices in the project
	QStringList matrixNames();

	//! \name Notes
	//@{
 	//! Creates a new empty note window
	Note* newNote(const QString& caption = QString());
	Note* openNote(ApplicationWindow* app, const QStringList &flist);
	void initNote(Note* m, const QString& caption);
	void saveNoteAs();
	//@}

	//! \name Folders
	//@{
	//! Returns a to the current folder in the project
	Folder* currentFolder(){return current_folder;};
	//! Adds a new folder to the project
	void addFolder();
	//! Deletes the current folder
	void deleteFolder();

	//! Ask confirmation from user, deletes the folder f if user confirms and returns true, otherwise returns false;
	bool deleteFolder(Folder *f);

	//! Deletes the currently selected items from the list view #lv.
	void deleteSelectedItems();

	//! Sets all items in the folders list view to be deactivated
	void deactivateFolders();

	//! Changes the current folder
	bool changeFolder(Folder *newFolder, bool force = false);

	//! Changes the current folder when the user changes the current item in the QListView "folders"
	void folderItemChanged(Q3ListViewItem *it);
	//! Changes the current folder when the user double-clicks on a folder item in the QListView "lv"
	void folderItemDoubleClicked(Q3ListViewItem *it);

	//!  creates and opens the context menu of a folder list view item
	/**
	 * \param it list view item
	 * \param p mouse global position
	 * \param fromFolders: true means that the user clicked right mouse buttom on an item from QListView "folders"
	 *					   false means that the user clicked right mouse buttom on an item from QListView "lv"
	 */
	void showFolderPopupMenu(Q3ListViewItem *it, const QPoint &p, bool fromFolders);

	//!  connected to the SIGNAL contextMenuRequested from the list views
	void showFolderPopupMenu(Q3ListViewItem *it, const QPoint &p, int);

	//!  starts renaming the selected folder by creating a built-in text editor
	void startRenameFolder();

	//!  starts renaming the selected folder by creating a built-in text editor
	void startRenameFolder(Q3ListViewItem *item);

	//!  checks weather the new folder name is valid and modifies the name
	void renameFolder(Q3ListViewItem *it, int col, const QString &text);

	//!  forces showing all windows in the current folder and subfolders, depending on the user's viewing policy
	void showAllFolderWindows();

	//!  forces hidding all windows in the current folder and subfolders, depending on the user's viewing policy
	void hideAllFolderWindows();

	//!  hides all windows in folder f
	void hideFolderWindows(Folder *f);

	//!  pops up folder information
	void folderProperties();

	//!  pops up information about the selected window item
	void windowProperties();

	//!  pops up information about the current project
	void projectProperties();

	//! Pops up a file dialog and invokes appendProject(const QString&) on the result.
	void appendProject();
	//! Open the specified project file and add it as a subfolder to the current folder.
	void appendProject(const QString& file_name);
	void saveAsProject();
	void saveFolderAsProject(Folder *f);
	void saveFolder(Folder *folder, const QString& fn);
	void rawSaveFolder(Folder *folder, QIODevice *device);

	//!  adds a folder list item to the list view "lv"
	void addFolderListViewItem(Folder *f);

	//!  adds a widget list item to the list view "lv"
	void addListViewItem(MyWidget *w);

	//!  hides or shows windows in the current folder and changes the view windows policy
	void setShowWindowsPolicy(int p);

	//!  returns a pointer to the root project folder
	Folder* projectFolder();

	//!  used by the findDialog
	void find(const QString& s, bool windowNames, bool labels, bool folderNames,
			  bool caseSensitive, bool partialMatch, bool subfolders);

	//!  initializes the list of items dragged by the user
	void dragFolderItems(QList<Q3ListViewItem *> items){draggedItems = items;};

	//!  Drop the objects in the list draggedItems to the folder of the destination item
	void dropFolderItems(Q3ListViewItem *dest);

	//!  moves a folder item to another
	/**
	 * \param src source folder item
	 * \param dest destination folder item
	 */
	void moveFolder(FolderListItem *src, FolderListItem *dest);
	//@}

	//! \name Scripting
	//@{
	//! notify the user that an error occured in the scripting system
	void scriptError(const QString &message, const QString &scriptName, int lineNumber);
	//! execute all notes marked auto-exec
	void executeNotes();
	//! show scripting language selection dialog
	void showScriptingLangDialog();
	//! create a new environment for the current scripting language
	void restartScriptingEnv();
	//! print to scripting console (if available) or to stdout
	void scriptPrint(const QString &text);
	//! switches to the given scripting language; if this is the same as the current one and force is true, restart it
	bool setScriptingLang(const QString &lang, bool force=false);
	//@}

signals:
	void modified();
    
private slots:
    	void showHelp();
	    void chooseHelpFolder();

// TODO: a lot of this stuff should be private
public:
    //! Last selected filter in export image dialog
    QString d_image_export_filter;
    bool d_keep_plot_aspect;
    int d_export_vector_size;
    bool d_export_transparency;
    int d_export_quality;
    int d_export_resolution;
    bool d_export_color;
	 //! Default paper orientation for image exports.
	 int d_export_orientation;
	//! Locale used to specify the decimal separators in imported ASCII files
	QLocale d_ASCII_import_locale;
    //! Last selected filter in import ASCII dialog
    QString d_ASCII_file_filter;
	bool d_convert_to_numeric;
	//! Specifies if only the Tables/Matrices in the current folder should be displayed in the Add/remove curve dialog.
	bool d_show_current_folder;
	bool d_scale_plots_on_print, d_print_cropmarks;
	bool d_show_table_comments;
	bool d_extended_plot_dialog;
	bool d_extended_import_ASCII_dialog;
	bool d_extended_export_dialog;
	bool d_extended_open_dialog;
	bool generateUniformFitPoints;
	bool generatePeakCurves;
	int peakCurvesColor;
	//! User defined size for the Add/Remove curves dialog
	QSize d_add_curves_dialog_size;

	//! Scale the errors output in fit operations with reduced chi^2
	bool fit_scale_errors;

	//! Number of points in a generated fit curve
	int fitPoints;

	//! Calculate only 2 points in a generated linear fit function curve
	bool d_2_linear_fit_points;

	bool pasteFitResultsToPlot;

	//! Write fit output information to Result Log
	bool writeFitResultsToLog;

	//! precision used for the output of the fit operations
	int fit_output_precision;

	//! default precision to be used for all other operations than fitting
	int d_decimal_digits;

	char d_default_numeric_format;

	//! pointer to the current folder in the project
	Folder *current_folder;
	//! Describes which windows are shown when the folder becomes the current folder
	ShowWindowsPolicy show_windows_policy;
	enum {MaxRecentProjects = 10};
	//! File version code used when opening project files (= maj * 100 + min * 10 + patch)
	int d_file_version;

	QColor workspaceColor, panelsColor, panelsTextColor;
	QString appStyle, workingDir;

	//! Path to the folder where the last template file was opened/saved
	QString templatesDir;
	bool smooth3DMesh, autoScaleFonts, autoResizeLayers;
#ifdef SEARCH_FOR_UPDATES
	bool autoSearchUpdates;
#endif
	bool confirmCloseTable, confirmCloseMatrix, confirmClosePlot2D, confirmClosePlot3D;
	bool confirmCloseFolder, confirmCloseNotes;
	bool canvasFrameOn, titleOn, autoSave, drawBackbones, allAxesOn, autoscale2DPlots, antialiasing2DPlots;
	int majTicksStyle, minTicksStyle, legendFrameStyle, autoSaveTime, axesLineWidth, canvasFrameWidth;
	QColor legendBackground, legendTextColor, defaultArrowColor;
	int defaultArrowLineWidth, defaultArrowHeadLength, defaultArrowHeadAngle;
	bool defaultArrowHeadFill;
	Qt::PenStyle defaultArrowLineStyle;
	int majTicksLength, minTicksLength, defaultPlotMargin;
	int defaultCurveStyle, defaultCurveLineWidth, defaultSymbolSize;
	int undoLimit;
	QFont appFont, plot3DTitleFont, plot3DNumbersFont, plot3DAxesFont;
	QFont tableTextFont, tableHeaderFont, plotAxesFont, plotLegendFont, plotNumbersFont, plotTitleFont;
	QColor tableBkgdColor, tableTextColor, tableHeaderColor;
	QString projectname,columnSeparator, appLanguage;
	QString configFilePath, logInfo, fitPluginsPath, asciiDirPath, imagesDirPath;
	int logID,asciiID,closeID, exportID, printAllID, ignoredLines, savingTimerId, plot3DResolution;
	bool renameColumns, copiedLayer, strip_spaces, simplify_spaces;
	QStringList recentProjects;
	QStringList tableWindows();
	bool saved, showPlot3DProjection, showPlot3DLegend, orthogonal3DPlots, autoscale3DPlots;
	QStringList plot3DColors, locales;
	QStringList functions; //user-defined functions;
	QStringList xFunctions, yFunctions, rFunctions,thetaFunctions; // user functions for parametric and polar plots
	QStringList fitFunctions; //user-defined fit functions;
	QStringList surfaceFunc; //user-defined surface functions;

	//! List of tables and matrices renamed in order to avoid conflicts when appending a project to a folder
	QStringList renamedTables;
	Graph::MarkerType copiedMarkerType;

	//! \name variables used when user copy/paste markers
	//@{
	QString auxMrkText;
	QFont auxMrkFont;
	QColor auxMrkColor, auxMrkBkgColor;
	QPoint auxMrkStart,auxMrkEnd;
	Qt::PenStyle auxMrkStyle;
	QString auxMrkFileName;
	int auxMrkBkg,auxMrkWidth;
	//@}

	bool startArrowOn, endArrowOn, fillArrowHead;
	int arrowHeadLength, arrowHeadAngle, specialPlotMenuID, statMenuID, panelMenuID, plot3dID;
	int plotMenuID, newMenuID, recentMenuID, setAsMenuID, fillMenuID; 
	int translateMenuID, smoothMenuID, filterMenuID, fitExpMenuID, multiPeakMenuID;

#ifdef SEARCH_FOR_UPDATES
	//! Equals true if an automatical search for updates was performed on start-up otherwise is set to false;
	bool autoSearchUpdatesRequest;
#endif

	//! The scripting language to use for new projects.
	QString defaultScriptingLang;
  /// location of translation resources
  QString qmPath;

private:
	//! Show a context menu for the widget
	void showWindowMenu(MyWidget * widget);
	//! Create a menu for toggeling the toolbars
	QMenu *createToolbarsMenu();

	//! Check whether a table is valid for a 3D plot and display an appropriate error if not
	bool validFor3DPlot(Table *table);
	//! Check whether a table is valid for a 2D plot and display an appropriate error if not
	bool validFor2DPlot(Table *table, int type);

	//! Workaround for the new colors introduced in rev 447
	int convertOldToNewColorIndex(int cindex);

	//! Stores the pointers to the dragged items from the FolderListViews objects
	QList<Q3ListViewItem *> draggedItems;
    
    QString helpFilePath;

#ifdef SEARCH_FOR_UPDATES
	//! Used when checking for new versions
	QHttp http;
	//! Used when checking for new versions
	QBuffer version_buffer;
#endif

	Graph *lastCopiedLayer;
	QSplitter *explorerSplitter;

	QMenu *windowsMenu,*view,*graph,*file,*format,*calcul,*edit,*dataMenu,*recent, *exportPlot, *toolbarsMenu;
	QMenu *d_quick_fit_menu;
	QMenu *help,*type,*plot2D,*plot3D, *specialPlot, *panels,*stat,*decay, *filter;
	QMenu *matrixMenu, *plot3DMenu, *plotDataMenu, *tableMenu;
	QMenu *smooth, *translateMenu, *multiPeakMenu;
	QMenu *scriptingMenu;
	QAction *actionCopyStatusBarText;
	QAction *actionEditCurveRange, *actionCurveFullRange, *actionShowAllCurves, *actionHideCurve, *actionHideOtherCurves;
	QAction *actionEditFunction, *actionRemoveCurve, *actionShowCurveWorksheet, *actionShowCurvePlotDialog;
    QAction *actionNewProject, *actionNewNote, *actionNewTable, *actionNewFunctionPlot, *actionNewSurfacePlot, *actionNewMatrix, *actionNewGraph;
    QAction *actionOpen, *actionLoadImage, *actionSaveProject, *actionSaveProjectAs, *actionImportImage;
    QAction *actionLoad, *actionUndo, *actionRedo;
    QAction *actionCopyWindow;
    QAction *actionCutSelection, *actionCopySelection, *actionPasteSelection, *actionClearSelection;
    QAction *locktoolbar;
    QAction *actionShowExplorer, *actionShowLog, *actionAddLayer, *actionShowLayerDialog, *actionAutomaticLayout;
	QAction *actionShowHistory;
#ifdef SCRIPTING_CONSOLE
    QAction *actionShowConsole;
#endif

    QAction *actionExportGraph, *actionExportAllGraphs, *actionPrint, *actionPrintAllPlots, *actionShowExportASCIIDialog;
    QAction *actionExportPDF;
    QAction *actionCloseAllWindows, *actionClearLogInfo, *actionShowPlotWizard, *actionShowConfigureDialog;
    QAction *actionShowCurvesDialog, *actionAddErrorBars, *actionAddFunctionCurve, *actionUnzoom, *actionNewLegend, *actionAddImage, *actionAddText;
    QAction *actionPlotL, *actionPlotP, *actionPlotLP, *actionPlotVerticalDropLines, *actionPlotSpline;
    QAction *actionPlotVertSteps, *actionPlotHorSteps, *actionPlotVerticalBars;
	QAction *actionPlotHorizontalBars, *actionPlotArea, *actionPlotPie, *actionPlotVectXYAM, *actionPlotVectXYXY;
    QAction *actionPlotHistogram, *actionPlotStackedHistograms, *actionPlot2VerticalLayers, *actionPlot2HorizontalLayers, *actionPlot4Layers, *actionPlotStackedLayers;
    QAction *actionPlot3DRibbon, *actionPlot3DBars, *actionPlot3DScatter, *actionPlot3DTrajectory;
    QAction *actionShowColStatistics, *actionShowRowStatistics, *actionShowIntDialog;
    QAction *actionDifferentiate, *actionFitLinear, *actionShowFitPolynomDialog;
    QAction *actionShowExpDecayDialog, *actionShowTwoExpDecayDialog, *actionShowExpDecay3Dialog;
    QAction *actionFitExpGrowth, *actionFitSigmoidal, *actionFitGauss, *actionFitLorentz, *actionShowFitDialog;
    QAction *actionShowAxisDialog, *actionShowTitleDialog;
    QAction *actionAbout;
    QAction *actionShowHelp;
#ifdef DYNAMIC_MANUAL_PATH
   	QAction *actionChooseHelpFolder;
#endif
    QAction *actionRename, *actionCloseWindow, *actionConvertTable;
    QAction *actionAddColToTable, *actionDeleteLayer, *actionInterpolate;
    QAction *actionResizeActiveWindow, *actionHideActiveWindow;
    QAction *actionShowMoreWindows, *actionPixelLineProfile, *actionIntensityTable;
    QAction *actionShowLineDialog, *actionShowImageDialog, *actionShowTextDialog;
    QAction *actionActivateWindow, *actionMinimizeWindow, *actionMaximizeWindow, *actionResizeWindow, *actionPrintWindow;
    QAction *actionShowPlotGeometryDialog, *actionEditSurfacePlot, *actionAdd3DData;
	QAction *actionMatrixDeterminant;
	QAction *actionConvertMatrix, *actionInvertMatrix;
	QAction *actionPlot3DWireFrame, *actionPlot3DHiddenLine, *actionPlot3DPolygons, *actionPlot3DWireSurface;
	QAction *actionColorMap, *actionContourMap, *actionGrayMap;
	QAction *actionDeleteFitTables, *actionShowGridDialog, *actionTimeStamp;
	QAction *actionSmoothSavGol, *actionSmoothFFT, *actionSmoothAverage, *actionFFT;
	QAction *actionLowPassFilter, *actionHighPassFilter, *actionBandPassFilter, *actionBandBlockFilter;
	QAction *actionConvolute, *actionDeconvolute, *actionCorrelate, *actionAutoCorrelate;
	QAction *actionTranslateHor, *actionTranslateVert;
	QAction *actionBoxPlot, *actionMultiPeakGauss, *actionMultiPeakLorentz;
#ifdef SEARCH_FOR_UPDATES
	QAction *actionCheckUpdates;
#endif
	QAction *actionHomePage;
#ifdef DOWNLOAD_LINKS
	QAction *actionDownloadManual;
#endif
	QAction *actionHelpForums;
    QAction *actionHelpBugReports;
	QAction *actionShowPlotDialog, *actionShowScaleDialog, *actionOpenTemplate, *actionSaveTemplate;
    QAction *actionNextWindow;
    QAction *actionPrevWindow;
	QAction *actionScriptingLang, *actionRestartScripting, *actionClearTable, *actionGoToCell;
	QAction *actionNoteExecute, *actionNoteExecuteAll, *actionNoteEvaluate, *actionSaveNote;
	QAction *actionAnimate, *actionPerspective, *actionFitFrame, *actionResetRotation;

	QActionGroup* dataTools;
	QAction *btnCursor, *btnSelect, *btnPicker, *btnRemovePoints, *btnMovePoints;
	QAction  *btnZoomIn, *btnZoomOut, *btnPointer, *btnLine, *btnArrow;

	QActionGroup* coord;
	QAction* Box;
    QAction* Frame;
    QAction* None;

	QActionGroup* grids;
    QAction* front;
    QAction* back;
    QAction* right;
    QAction* left;
    QAction* ceil;
    QAction* floor;

	QActionGroup* floorstyle;
    QAction* floordata;
    QAction* flooriso;
    QAction* floornone;

	QActionGroup* plotstyle;
    QAction* wireframe;
    QAction* hiddenline;
    QAction* polygon;
    QAction* filledmesh;
    QAction* pointstyle;
	QAction* barstyle;
	QAction *conestyle, *crossHairStyle;

	//! Manages connection between plot actions and Graph::CurveType values (not used by all plot actions).
	QSignalMapper *d_plot_mapper;

	QLabel *d_status_info;

	Project * d_project;

private slots:
	void removeDependentTableStatistics(const AbstractAspect *aspect);
	//! Set the active window selected from the context menu's dependency list
	/**
	 * See also: http://doc.trolltech.com/4.3/mainwindows-recentfiles-mainwindow-cpp.html
	 */ 
	void setActiveWindowFromAction();
	//! Manage plot type selection.
	/**
	 * If the current window is a Table, generate a new graph from the selected data.
	 * If it is a Graph, change the plot type of the last curve.
	 * For everything else, do nothing.
	 */
	void selectPlotType(int type);

	void handleAspectAdded(const AbstractAspect * aspect, int index);
	void handleAspectAboutToBeRemoved(const AbstractAspect * aspect, int index);
	void lockToolbar(const bool status);
};

#endif

