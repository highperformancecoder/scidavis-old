#ifndef APPLICATION_H
#define APPLICATION_H

#include <qmainwindow.h>
#include <qlistview.h> 
#include <qhttp.h> 
#include <qfile.h> 
#include <qtextbrowser.h>

#include "graph3D.h"
#include "plot3DDialog.h"
#include "Scripting.h"

class QAction;
class QActionGroup;
class QTextEdit;
class QToolBar;
class QToolButton;
class QPopupMenu;
class QWorkspace;
class QPopupMenu;
class QToolButton;
class QListBoxItem;
class QTextBrowser;
class QLineEdit;
class QDockWindow;
class QTranslator;
class QSplitter;

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
class ScriptingEnv;
class ScriptWindow;
class TableStatistics;

//! QtiPlot's main window
class ApplicationWindow: public QMainWindow, public scripted
{
    Q_OBJECT
public:
    ApplicationWindow();
	ApplicationWindow(const QStringList& l);
	~ApplicationWindow();

	enum ShowWindowsPolicy{HideAll, ActiveFolder, SubFolders};

	ScriptWindow *scriptWindow;
	QTranslator *appTranslator, *qtTranslator;
	QDockWindow *logWindow, *explorerWindow;
	QTextEdit *results;
#ifdef SCRIPTING_CONSOLE
	QDockWindow *consoleWindow;
	QTextEdit *console;
#endif
	QWorkspace* ws;
    QToolBar *fileTools, *plotTools, *tableTools, *plot3DTools, *displayBar, *editTools;
    QPopupMenu *windowsMenu,*view,*graph,*file,*format,*calcul,*edit,*dataMenu,*recent, *exportPlot;
	QPopupMenu *help,*type,*import,*plot2D,*plot3D, *specialPlot, *panels,*stat,*decay, *filter;
	QPopupMenu *matrixMenu, *plot3DMenu, *plotDataMenu, *tableMenu, *tablesDepend; 
	QPopupMenu *smooth, *normMenu, *translateMenu, *fillMenu, *setAsMenu, *multiPeakMenu;
	QPopupMenu *scriptingMenu;
	FolderListView *lv, *folders;
	QToolButton *btnResults;
	QWidgetList *hiddenWindows, *outWindows;
	QLineEdit *info;
	QWidget *lastModified;

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
	Graph *activeGraph,  *lastCopiedLayer;

public:
	/*! Generates a new unique name starting with string /param name.
	You can force the output to be a name different of /param name, 
	even if 'name' is not used in the project, by setting /param increment = true (the default)
	*/
	QString generateUniqueName(const QString& name, bool increment = true);
	
public slots:
	void open();
	ApplicationWindow* open(const QString& fn);
	ApplicationWindow* openProject(const QString& fn);
	ApplicationWindow* importOPJ(const QString& filename);

	ApplicationWindow * plotFile(const QString& fn);
	void updatePlotsTransparency();

	QWidgetList* windowsList();
	void updateWindowLists(QWidget *w);

	void setSaveSettings(bool autoSaving, int min);
	void changeAppStyle(const QString& s);
	void changeAppFont(const QFont& f);
	void updateAppFonts();
	void setAppColors(const QColor& wc,const QColor& pc,const QColor& tpc);

	//multilayer plots
	MultiLayer* copyGraph();
	MultiLayer* multilayerPlot(int c, int r, int style);
	MultiLayer* multilayerPlot(Table* w,const QStringList& colList, int style);
	//!used when restoring a plot from a project file
	MultiLayer* multilayerPlot(const QString& caption);
	//!used by the plot wizard
	MultiLayer* multilayerPlot(const QStringList& colList); 
	void connectMultilayerPlot(MultiLayer *g);
	MultiLayer *plot(const QString& name);
	void addLayer();
	void deleteLayer();
	void initMultilayerPlot(MultiLayer* g, const QString& name);
	void polishGraph(Graph *g, int style);
	void plot2VerticalLayers();
	void plot2HorizontalLayers();
	void plot4Layers();
	void plotStackedLayers();
	void plotStackedHistograms();

	//3D data plots
	Graph3D* openMatrixPlot3D(const QString& caption, const QString& matrix_name,
							 double xl,double xr,double yl,double yr,double zl,double zr);
	Graph3D* dataPlot3D(Table* table,const QString& colName);
	Graph3D* dataPlotXYZ(Table* table,const QString& zColName, int type);
		//when reading from .qti file
	Graph3D* dataPlot3D(const QString& caption,const QString& formula,
						double xl, double xr, double yl, double yr, double zl, double zr);
	Graph3D* dataPlotXYZ(const QString& caption,const QString& formula,
 						double xl, double xr, double yl, double yr, double zl, double zr);
	/*!
	* used when plotting from the wizard
	*/
	Graph3D* dataPlotXYZ(const QString& formula);
	Graph3D* dataPlot3D(const QString& formula);

	//surface plots
	Graph3D* newPlot3D(const QString& formula, double xl, double xr,
					   double yl, double yr, double zl, double zr);
	Graph3D* newPlot3D(const QString& caption,const QString& formula, 
					   double xl, double xr,double yl, double yr, double zl, double zr);
	Graph3D* copySurfacePlot();
	void connectSurfacePlot(Graph3D *plot);
	void newSurfacePlot();
	void editSurfacePlot();
	void remove3DMatrixPlots(Matrix *m);
	void update3DMatrixPlots(QWidget *w);
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

	//user-defined functions
	void newFunctionPlot();
	void newFunctionPlot(int type,QStringList &formulas, const QString& var,QValueList<double> &ranges, int points);

	FunctionDialog* functionDialog();
	void showFunctionDialog(int curveKey);
	void showFunctionDialog(Graph *g, int curve);
	void addFunctionCurve();
	void clearFitFunctionsList();
	void saveFitFunctionsList(const QStringList& l);
	void clearSurfaceFunctionsList();
	void clearLogInfo();
	void clearParamFunctionsList();
	void clearPolarFunctionsList();
	void updateFunctionLists(int type, QStringList &formulas);
	void updateSurfaceFuncList(const QString& s);

	Matrix* cloneMatrix();
	Matrix* newMatrix();
	Matrix* newMatrix(const QString& caption, int r, int c);
	Matrix* matrix(const QString& name);
	Matrix* createIntensityMatrix(const QPixmap& pic);
	Matrix* convertTableToMatrix();
	void initMatrix(Matrix* m, const QString& caption);
	void transposeMatrix();
	void invertMatrix();
	void matrixDeterminant();
	
	//! Creates an empty table
	Table* newTable();
	//! Used when importing an ASCII file
	Table* newTable(const QString& fname, const QString &sep, int lines, 
		            bool renameCols, bool stripSpaces, bool simplifySpaces);
	//! Used when loading a table from a project file 
	Table* newTable(const QString& caption,int r, int c);
	Table* newTable(const QString& caption, int r, int c, const QString& text);
	Table* newHiddenTable(const QString& caption, int r, int c, const QString& text);
	Table* newHiddenTable(const QString& name, const QString& legend, int r, int c);
	Table* table(const QString& name);
	Table* copyTable();
	Table* convertMatrixToTable();
	QWidgetList* tableList();

	void connectTable(Table* w);
	void newWrksheetPlot(const QString& caption,int r, int c, const QString& text);
	void showHistogramTable(const QString& caption, int r, int c, const QString& text);
	void initTable(Table* w, const QString& caption);
	void customTable(Table* w);
	void customizeTables(const QColor& bgColor,const QColor& textColor,
						const QColor& headerColor,const QFont& textFont, const QFont& headerFont);

	TableStatistics *newTableStatistics(Table *base, int type, QValueList<int>,
	    const QString &caption=QString::null);

	void customGraph(Graph* g);
	void setGraphDefaultSettings(bool autoscale,bool scaleFonts,bool resizeLayers);
	void setLegendDefaultSettings(int frame, const QFont& font, 
							 const QColor& textCol, const QColor& backgroundCol);
	void setArrowDefaultSettings(int lineWidth,  const QColor& c, Qt::PenStyle style,
								int headLength, int headAngle, bool fillHead);

	void plot3DWireframe();
	void plot3DHiddenLine();
	void plot3DPolygons();
	void plot3DWireSurface();

	void plot3DMatrix(int style);

	void plot3DRibbon();
	void plot3DScatter();
	void plot3DTrajectory();
	void plot3DBars();

	void plotL();
	void plotP();
	void plotLP();
	void plotPie();
	void plotVerticalBars();
	void plotHorizontalBars();
	void plotArea();
	void plotSteps();
	void plotSpline();
	void plotVerticalDropLines();
	void plotHistogram();
	void plotVectXYXY();
	void plotVectXYAM();
	void plotBoxDiagram();
	
	//image analysis
	void intensityTable();
	void pixelLineProfile();
	void loadImage();
	void loadImage(const QString& fn);
	void importImage();
	
    void loadASCII();
	void loadMultiple();
	void loadMultipleASCIIFiles(const QStringList& fileNames, int importFileAs);
	void exportAllTables(const QString& sep, bool colNames, bool expSelection);
	void exportASCII(const QString& tableName, const QString& sep, bool colNames, bool expSelection);

	void exportLayer();
    void exportGraph();
	void exportAllGraphs();
	void exportAllGraphs(const QString& dir, const QString& format, 
									int quality, bool transparency);
	void export2DPlotToFile(MultiLayer *plot, const QString& fileName, const QString& format, 
										int quality, bool transparency);
	void export3DPlotToFile(Graph3D *plot, const QString& fileName, 
										const QString& format);

	void saveProjectAs();
	bool saveProject();

	void readSettings();
	void saveSettings();
	void applyUserSettings();
	void modifiedProject();
	void modifiedProject(QWidget *w);
    void print();
	void print(QWidget* w);
	void printAllPlots();
	void setImportOptions(const QString& sep, int lines, bool rename, bool strip, bool simplify);
    
	void showExplorer();
	QStringList columnsList(Table::PlotDesignation plotType = Table::All);
	
	void undo();
	void redo();
	
	myWidget* copyWindow();
	void rename();
	void renameWindow();

	//!  Called when the user presses F2 and an item is selected in lv.
	void renameWindow(QListViewItem *item, int, const QString &s);

	//!  Checks weather the new window name is valid and modifies the name. 
	bool renameWindow(myWidget *w, const QString &text);

	void maximizeWindow(QListViewItem * lbi);
	void maximizeWindow();
	void minimizeWindow();

	void updateWindowStatus(myWidget* );

	bool hidden(QWidget* window);
	void closeActiveWindow();
	void closeWindow(myWidget* window);

	//!  Does all the cleaning work before actually deleting a window!
	void removeWindowFromLists(QWidget* w);

	void hideWindow(myWidget* window);
	void hideWindow();
	void hideActiveWindow();
	void activateWindow();
	void activateWindow(QWidget *);
	void printWindow();
	void updateTable(const QString& caption,int row,const QString& text);
	void updateTableColumn(const QString& colName, double *dat, int rows);
    void about();
    void windowsMenuAboutToShow();
    void windowsMenuActivated( int id );
	void removeCurves(const QString& name);
	QStringList dependingPlots(const QString& caption);
	QStringList depending3DPlots(Matrix *m);
	QStringList multilayerDependencies(QWidget *w);

	void saveAsTemplate();
	void openTemplate();

	QString windowGeometryInfo(QWidget *w);
	void restoreWindowGeometry(ApplicationWindow *app, QWidget *w, const QString s);

	void resizeActiveWindow();
	void resizeWindow();
	
	// list view in project explorer
	void setListView(const QString& caption,const QString& view);
	void renameListViewItem(const QString& oldName,const QString& newName);
	void setListViewDate(const QString& caption,const QString& date);
	QString listViewDate(const QString& caption);
	void setListViewSize(const QString& caption,const QString& size);
	void setListViewLabel(const QString& caption,const QString& label);
	
	void updateColNames(const QString& oldName, const QString& newName);
	void updateTableNames(const QString& oldName, const QString& newName);
	void changeMatrixName(const QString& oldName, const QString& newName);
	void updateCurves(Table *t, const QString& name);
	
	void showTable(const QString& curve);
	void showTable(int i);

	void addColToTable();
	void cutSelection();
	void copySelection();
	void copyMarker();
	void pasteSelection();
	void clearSelection();
	void clearCellFromTable(const QString& name,double value);
	void copyActiveLayer();
	
	void newProject();

	//! Creates a new empty multilayer plot
	MultiLayer* newGraph();

	Matrix* openMatrix(ApplicationWindow* app, const QStringList &flist);
	Table* openTable(ApplicationWindow* app, const QStringList &flist);
	TableStatistics* openTableStatistics(const QStringList &flist);
	Graph3D* openSurfacePlot(ApplicationWindow* app, const QStringList &lst);
	void openGraph(ApplicationWindow* app, MultiLayer *plot, const QStringList &list);

	void openRecentProject(int index);
	void insertTranslatedStrings();
	void translateActionsStrings();
	void init();
	void initGlobalConstants();
	void createActions();
	void initMainMenu();
	void initPlotMenu();
	void initTableMenu();
	void initTableAnalysisMenu();
	void initPlotDataMenu();
	void initToolBars();
	void initPlot3DToolBar();
	void disableActions();
	void hideToolbars();
	void customToolBars(QWidget* w);
	void customMenu(QWidget* w);
	void windowActivated(QWidget *w);

	//table tools
	void sortSelection();
	void sortActiveTable();
	void normalizeSelection();
	void normalizeActiveTable();
	void correlate();
	void convolute();
	void deconvolute();
	void clearTable();
	void goToRow();

	// plot tools 
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
	void disableTools();
	void pickDataTool( QAction* action );

	void updateLog(const QString& result);

	//fitting
	void deleteFitTables();
	void fitLinear();
	void fitSigmoidal();
	void fitGauss();
	void fitLorentz();
	void fitMultiPeak(int profile);
	void fitMultiPeakGauss();
	void fitMultiPeakLorentz();
				 
	//calculus
	void differentiate();
	void analysis(const QString& whichFit);
	void analyzeCurve(const QString& whichFit, const QString& curveTitle);
	void showAnalysisDialog(const QString& whichFit);

	void addErrorBars();
	void defineErrorBars(const QString& name,int type,const QString& percent,int direction);
	void defineErrorBars(const QString& curveName,const QString& errColumnName, int direction);
	void movePoints();
	void removePoints();

	// event handlers 
	void closeEvent( QCloseEvent*);
	void timerEvent ( QTimerEvent *e);
	void dragEnterEvent( QDragEnterEvent* e );
	void dropEvent( QDropEvent* e );
	void customEvent( QCustomEvent* e);

	//dialogs
	void showFindDialogue();	
	void showPlotDialog();
	void showPlotDialog(int curveKey);
	QDialog* showScaleDialog();
	QDialog* showPieDialog();
	QDialog* showPlot3dDialog();
	axesDialog* showScalePageFromAxisDialog(int axisPos);
	axesDialog* showAxisPageFromAxisDialog(int axisPos);
	void showAxisDialog();
	void showGridDialog();
	void showGeneralPlotDialog();
	void showResults(bool ok);
	void showResults(const QString& s);
	void showTextDialog();
	void showLineDialog();
	void showTitleDialog();
	void showExportASCIIDialog();
	void showCurvesDialog();
	void showPlotAssociations(int curve);

	void showXAxisTitleDialog();
	void showYAxisTitleDialog();
	void showRightAxisTitleDialog();
	void showTopAxisTitleDialog();
	void showColumnOptionsDialog();
	void showRowsDialog();
	void showColsDialog();
	void showColMenu(int c);
	void showColumnValuesDialog();	
	//! recalculate selected cells of current table
	void recalculateTable();
	void showGraphContextMenu();
	void showTableContextMenu(bool selection);
	void showWindowContextMenu();
	void showWindowTitleBarMenu();
	void showCurveContextMenu(int curveKey);
	void showCurveWorksheet(int curveKey);
	void showWindowPopupMenu(QListViewItem *it, const QPoint &p, int);

	//! Connected to the context menu signal from lv; it's called when there are several items selected in the list
	void showListViewSelectionMenu(const QPoint &p);

	//! Connected to the context menu signal from lv; it's called when there are no items selected in the list
	void showListViewPopupMenu(const QPoint &p);

	void showScriptWindow();
	void showMoreWindows();
	void showImportDialog();
	void showMarkerPopupMenu();
	void showHelp();
	void chooseHelpFolder();
	void showPlotWizard();
	void showFitPolynomDialog();
	void showIntDialog();
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
	void showMatrixDialog();
	void showMatrixSizeDialog();
	void showMatrixValuesDialog();
	void showSmoothSavGolDialog();
	void showSmoothFFTDialog();
	void showSmoothAverageDialog();
	void lowPassFilterDialog();
	void highPassFilterDialog();
	void bandPassFilterDialog();
	void bandBlockFilterDialog();
	void showFFTDialog();

	void translateCurveHor();
	void translateCurveVert();

	//! Removes the curve identified by key 'curveKey' from a 2D plot
	void removeCurve(int curveKey);

	void setAscValues();
	void setRandomValues();
	void setXCol();
	void setYCol();
	void setZCol();
	void setXErrCol();
	void setYErrCol();
	void disregardCol();

	void updateConfirmOptions(bool askTables, bool askMatrixes, bool askPlots2D, bool askPlots3D, bool askNotes);
	void showAxis(int axis, int type, const QString& labelsColName, bool axisOn, 
				int majTicksType, int minTicksType, bool labelsOn, const QColor& c, 
				int format, int prec, int rotation, int baselineDist, const QString& formula);
	
	//plot3D tools	
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

	void updateRecentProjectsList();
	
	//!  connected to the done(bool) signal of the http object
	void receivedVersionFile(bool error);
	//!  called when the user presses the actionCheckUpdates
	void searchForUpdates();

	void showDonationDialog();
	void showSupportPage();
	void showDonationsPage();
	void showHomePage();
	void downloadManual();
	void downloadTranslation();

	//! Opens an internet browser
	bool open_browser(QWidget* parent, const QString& rUrl);

	void parseCommandLineArgument(const QString& s, int args);
	void createLanguagesList();
	void switchToLanguage(int param);
	void switchToLanguage(const QString& locale);

	bool alreadyUsedName(const QString& label);
	bool projectHas2DPlots();
	bool projectHas3DPlots();

	//! Returns a pointer to the window named "name"
	QWidget* window(const QString& name);

	//! Returns a list with the names of all the matrices in the project
	QStringList matrixNames();

	Note* newNote(const QString& caption = QString::null);
	Note* openNote(ApplicationWindow* app, const QStringList &flist);
	void initNote(Note* m, const QString& caption);
	Note* note(const QString& name);
	void saveNoteAs();
	
	//! Adds a new folder to the project
	void addFolder();
	//! Deletes the current folder
	void deleteFolder();

	//! Ask confirmation from user, deletes the folder f if user confirms and returns true, otherwise returns false;
	bool deleteFolder(Folder *f);

	//! Deletes the currently selected items from the list view lv
	void deleteSelectedItems();

	//! Sets all items in the folders list view to be desactivated (QPixmap = folder_closed_xpm)
	void desactivateFolders();

	//! Changes the current folder
	void changeFolder(Folder *newFolder, bool force = FALSE);

	//! Changes the current folder when the user changes the current item in the QListView "folders"
	void folderItemChanged(QListViewItem *it);
	//! Changes the current folder when the user double-clicks on a folder item in the QListView "lv"
	void folderItemDoubleClicked(QListViewItem *it);

	//!  creates and opens the context menu of a folder list view item
	/**
	 * \param it list view item
	 * \param p mouse global position
	 * \param fromFolders: TRUE means that the user clicked right mouse buttom on an item from QListView "folders"
	 *					   FALSE means that the user clicked right mouse buttom on an item from QListView "lv"
	 */
	void showFolderPopupMenu(QListViewItem *it, const QPoint &p, bool fromFolders);

	//!  connected to the SIGNAL contextMenuRequested from the list views
	void showFolderPopupMenu(QListViewItem *it, const QPoint &p, int);
	
	//!  starts renaming the selected folder by creating a built-in text editor
	void startRenameFolder();

	//!  starts renaming the selected folder by creating a built-in text editor
	void startRenameFolder(QListViewItem *item);

	//!  checks weather the new folder name is valid and modifies the name
	void renameFolder(QListViewItem *it, int col, const QString &text);

	//!  forces showing all windows in the current folder and subfolders, depending on the user's viewing policy
	void showAllFolderWindows();

	//!  forces hidding all windows in the current folder and subfolders, depending on the user's viewing policy
	void hideAllFolderWindows();

	//!  hides all windows in folder f
	void hideFolderWindows(Folder *f);

	//!  pop-ups folder information
	void folderProperties();

	//!  pop-ups information about the selected window item
	void windowProperties();

	//!  pop-ups information about the current project
	void projectProperties();

	void appendProject();
	void saveAsProject();
	void saveFolderAsProject(Folder *f);
	void saveFolder(Folder *folder, const QString& fn);

	//!  adds a folder list item to the list view "lv"
	void addFolderListViewItem(Folder *f);

	//!  adds a widget list item to the list view "lv"
	void addListViewItem(myWidget *w);

	//!  hides or shows windows in the current folder and changes the view windows policy
	void setShowWindowsPolicy(int p);

	//!  returns a pointer to the root project folder
	Folder* projectFolder();

	//!  used by the findDialog
	void find(const QString& s, bool windowNames, bool labels, bool folderNames, 
			  bool caseSensitive, bool partialMatch, bool subfolders);

	//!  initializes the list of items dragged by the user
	void dragFolderItems(QPtrList<QListViewItem> items){draggedItems = items;};

	//!  Drop the objects in the list draggedItems to the folder of the destination item 
	void dropFolderItems(QListViewItem *dest);

	//!  moves a folder item to another
	/**
	 * \param src source folder item
	 * \param dest destination folder item
	 */
	void moveFolder(FolderListItem *src, FolderListItem *dest);

	// scripting
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
	//! switches to the given scripting language (if different from the current)
	bool setScriptingLang(const QString &lang);

signals:
	void modified();
	
public:
	bool generateUniformFitPoints;
	bool generatePeakCurves;
	int peakCurvesColor;

	//! Scale the errors output in fit operations with reduced chi^2
	bool fit_scale_errors;

	//! Number of points in a generated fit curve
	int fitPoints;
	bool pasteFitResultsToPlot;

	//! Write fit output information to Result Log
	bool writeFitResultsToLog;

	//! precision used for the output of the fit operations
	int fit_output_precision;

	//! pointer to the current folder in the project
	Folder *current_folder;
	//! Describes which windows are shown when the folder becomes the current folder
	ShowWindowsPolicy show_windows_policy;
	enum {MaxRecentProjects = 5};
	int fileVersion;

	//! Extra version information string (like "alpha", "-2", etc...)
	QString d_extra_version;

	int majVersion, minVersion, patchVersion;
	QColor workspaceColor, panelsColor, panelsTextColor;
	QString appStyle, workingDir;

	//! Path to the folder where the last template file was opened/saved 
	QString templatesDir;
	bool smooth3DMesh, autoScaleFonts, autoResizeLayers, askForSupport, autoSearchUpdates;
	bool confirmCloseTable, confirmCloseMatrix, confirmClosePlot2D, confirmClosePlot3D;
	bool confirmCloseFolder, confirmCloseNotes;
	bool canvasFrameOn, titleOn, autoSave, drawBackbones, allAxesOn, autoscale2DPlots;
	int majTicksStyle, minTicksStyle, legendFrameStyle, autoSaveTime, axesLineWidth, canvasFrameWidth;
	QColor legendBackground, legendTextColor, defaultArrowColor;
	int defaultArrowLineWidth, defaultArrowHeadLength, defaultArrowHeadAngle;
	bool defaultArrowHeadFill;
	Qt::PenStyle defaultArrowLineStyle;
	int majTicksLength, minTicksLength, defaultPlotMargin;
	int defaultCurveStyle, defaultCurveLineWidth, defaultSymbolSize;
	QFont appFont, plot3DTitleFont, plot3DNumbersFont, plot3DAxesFont;
	QFont tableTextFont, tableHeaderFont, plotAxesFont, plotLegendFont, plotNumbersFont, plotTitleFont;
	QColor tableBkgdColor, tableTextColor, tableHeaderColor;
	QString projectname, columnSeparator, helpFilePath, appLanguage;
	QString configFilePath, logInfo, fitPluginsPath;
	int logID,asciiID,closeID, exportID, printAllID, ignoredLines, savingTimerId, plot3DResolution;
	bool renameColumns, copiedLayer, strip_spaces, simplify_spaces;
	QStringList recentProjects, tableWindows;
	bool saved, showPlot3DProjection, showPlot3DLegend;
	QStringList plot3DColors, locales;
	QStringList xFunctions, yFunctions, rFunctions,tetaFunctions; // user functions for parametric and polar plots
	QStringList fitFunctions; //user-defined fit functions;
	QStringList surfaceFunc; //user-defined surface functions;

	//!List of tables and matrixes renamed in order to avoid conflicts when appending a project to a folder
	QStringList renamedTables;
	//!active window
	myWidget *aw; 
	Graph::MarkerType copiedMarkerType;
	
//variables used when user copy/paste markers
	QString auxMrkText;
	QFont auxMrkFont;
	QColor auxMrkColor, auxMrkBkgColor;
	QPoint auxMrkStart,auxMrkEnd;
	Qt::PenStyle auxMrkStyle;
	QString auxMrkFileName;
	int auxMrkBkg,auxMrkWidth;
	bool startArrowOn, endArrowOn, fillArrowHead;
	int arrowHeadLength, arrowHeadAngle, specialPlotMenuID, statMenuID, panelMenuID, plot3dID;
	int plotMenuID, importMenuID, newMenuID, recentMenuID, setAsMenuID, fillMenuID, normMenuID;
	int translateMenuID, smoothMenuID, filterMenuID, fitExpMenuID, multiPeakMenuID; 

	//! Equals true if an automatical search for updates was performed on start-up otherwise is set to false;
	bool autoSearchUpdatesRequest;

	//! The scripting language to use for new projects.
	QString defaultScriptingLang;

    QAction *actionNewProject, *actionNewNote, *actionNewTable, *actionNewFunctionPlot, *actionNewSurfacePlot, *actionNewMatrix, *actionNewGraph;
    QAction *actionOpen, *actionLoadImage, *actionSaveProject, *actionSaveProjectAs, *actionImportImage;
    QAction *actionLoad, *actionLoadMultiple, *actionUndo, *actionRedo;
    QAction *actionCopyWindow;
    QAction *actionCutSelection, *actionCopySelection, *actionPasteSelection, *actionClearSelection;
    QAction *actionShowExplorer, *actionShowLog, *actionAddLayer, *actionShowLayerDialog;
#ifdef SCRIPTING_CONSOLE
    QAction *actionShowConsole;
#endif

    QAction *actionExportGraph, *actionExportAllGraphs, *actionPrint, *actionPrintAllPlots, *actionShowExportASCIIDialog;
    QAction *actionShowImportDialog;
    QAction *actionCloseAllWindows, *actionClearLogInfo, *actionShowPlotWizard, *actionShowConfigureDialog;
    QAction *actionShowCurvesDialog, *actionAddErrorBars, *actionAddFunctionCurve, *actionUnzoom, *actionNewLegend, *actionAddImage, *actionAddText;
    QAction *actionPlotL, *actionPlotP, *actionPlotLP, *actionPlotVerticalDropLines, *actionPlotSpline, *actionPlotSteps, *actionPlotVerticalBars;
	QAction *actionPlotHorizontalBars, *actionPlotArea, *actionPlotPie, *actionPlotVectXYAM, *actionPlotVectXYXY;
    QAction *actionPlotHistogram, *actionPlotStackedHistograms, *actionPlot2VerticalLayers, *actionPlot2HorizontalLayers, *actionPlot4Layers, *actionPlotStackedLayers;
    QAction *actionPlot3DRibbon, *actionPlot3DBars, *actionPlot3DScatter, *actionPlot3DTrajectory;
    QAction *actionShowColStatistics, *actionShowRowStatistics, *actionShowIntDialog;
    QAction *actionDifferentiate, *actionFitLinear, *actionShowFitPolynomDialog;
    QAction *actionShowExpDecayDialog, *actionShowTwoExpDecayDialog, *actionShowExpDecay3Dialog;
    QAction *actionFitExpGrowth, *actionFitSigmoidal, *actionFitGauss, *actionFitLorentz, *actionShowFitDialog;
    QAction *actionShowCurveFormatDialog, *actionShowAxisDialog, *actionShowTitleDialog;
    QAction *actionShowColumnOptionsDialog, *actionShowColumnValuesDialog, *actionShowColsDialog, *actionShowRowsDialog;
    QAction *actionTableRecalculate;
    QAction *actionAbout, *actionShowHelp, *actionChooseHelpFolder;
    QAction *actionRename, *actionCloseWindow, *actionConvertTable;
    QAction *actionAddColToTable, *actionDeleteLayer, *actionInterpolate;
    QAction *actionResizeActiveWindow, *actionHideActiveWindow;
    QAction *actionShowMoreWindows, *actionPixelLineProfile, *actionIntensityTable;
    QAction *actionShowLineDialog, *actionShowImageDialog, *actionShowTextDialog;
    QAction *actionActivateWindow, *actionMinimizeWindow, *actionMaximizeWindow, *actionHideWindow, *actionResizeWindow, *actionPrintWindow;
    QAction *actionShowPlotGeometryDialog, *actionEditSurfacePlot, *actionAdd3DData;
	QAction *actionMatrixDeterminant, *actionSetMatrixProperties;
	QAction *actionSetMatrixDimensions, *actionConvertMatrix, *actionSetMatrixValues, *actionTransposeMatrix, *actionInvertMatrix;
	QAction *actionPlot3DWireFrame, *actionPlot3DHiddenLine, *actionPlot3DPolygons, *actionPlot3DWireSurface;
	QAction *actionDeleteFitTables, *actionShowGridDialog, *actionTimeStamp;
	QAction *actionSmoothSavGol, *actionSmoothFFT, *actionSmoothAverage, *actionFFT;
	QAction *actionLowPassFilter, *actionHighPassFilter, *actionBandPassFilter, *actionBandBlockFilter;
	QAction *actionSortTable, *actionSortSelection, *actionNormalizeSelection;
	QAction *actionNormalizeTable, *actionConvolute, *actionDeconvolute, *actionCorrelate;
	QAction *actionTranslateHor, *actionTranslateVert, *actionSetAscValues, *actionSetRandomValues;
	QAction *actionSetXCol, *actionSetYCol, *actionSetZCol, *actionDisregardCol, *actionSetXErrCol, *actionSetYErrCol;
	QAction *actionBoxPlot, *actionMultiPeakGauss, *actionMultiPeakLorentz, *actionCheckUpdates;
	QAction *actionDonate, *actionHomePage, *actionDownloadManual, *actionTechnicalSupport, *actionTranslations;
	QAction *actionShowPlotDialog, *actionShowScaleDialog, *actionOpenTemplate, *actionSaveTemplate;
	QAction *actionScriptingLang, *actionRestartScripting, *actionClearTable, *actionGoToRow;
	QAction *actionNoteExecute, *actionNoteExecuteAll, *actionNoteEvaluate, *actionSaveNote;
	QAction *actionShowScriptWindow;

private:
	//!Stores the pointers to the dragged items from the FolderListViews objects
	QPtrList<QListViewItem> draggedItems;
	//! Used when checking for new versions
	QHttp http;
	//! Used when checking for new versions
	QFile versionFile;

	QSplitter *explorerSplitter;
};

//! QtiPlot's help browser
class HelpBrowser: public QTextBrowser
{
    Q_OBJECT

public:
    HelpBrowser(QWidget * parent = 0, const char * name = 0);

public slots:
	void print();
	void open();
};

#endif
