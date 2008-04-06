/***************************************************************************
    File                 : ProjectWindow.cpp
    Project              : SciDAVis
    Description          : Standard view on a Project; main window.
    --------------------------------------------------------------------
    Copyright            : (C) 2007-2008 Knut Franke (knut.franke*gmx.de)
    Copyright            : (C) 2007-2008 Tilman Hoener zu Siederdissen (thzs*gmx.net)
    Copyright            : (C) 2007 by Knut Franke, Tilman Hoener zu Siederdissen
                           (some parts taken from former ApplicationWindow class
						    (C) 2004-2007 by Ion Vasilief (ion_vasilief*yahoo.fr))
                           (replace * with @ in the email addresses) 

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
#include "ProjectWindow.h"

#include "Project.h"
#include "AspectTreeModel.h"
#include "AbstractPart.h"
#include "PartMdiView.h"
#include "ProjectExplorer.h"
#include "interfaces.h"
#include "ImportDialog.h"
#include "AbstractImportFilter.h"
#include "lib/ActionManager.h"
#include "lib/ShortcutsDialog.h"

#include <QMenuBar>
#include <QMenu>
#include <QMdiArea>
#include <QDockWidget>
#include <QToolBar>
#include <QApplication>
#include <QUndoStack>
#include <QUndoView>
#include <QToolButton>
#include <QPluginLoader>
#include <QSignalMapper>
#include <QStatusBar>

ActionManager * ProjectWindow::action_manager = 0;

ProjectWindow::ProjectWindow(Project* project)
	: d_project(project)
{
	init();
}

void ProjectWindow::init()
{
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowIcon(QIcon(":/appicon"));

	if (!action_manager)
		action_manager = new ActionManager();

	action_manager->setTitle(tr("global", "global/ProjectWindow keyboard shortcuts"));

	d_mdi_area = new QMdiArea();
	setCentralWidget(d_mdi_area);
	connect(d_mdi_area, SIGNAL(subWindowActivated(QMdiSubWindow*)),
			this, SLOT(handleCurrentSubWindowChanged(QMdiSubWindow*)));
	d_current_aspect = d_project;
	d_current_folder = d_project;

	initDockWidgets();
	initActions();
	initToolBars();
	initMenus();
	d_buttons.new_aspect->setMenu(d_menus.new_aspect);
	// TODO: move all strings to one method to be called on a language change
	
	connect(d_project, SIGNAL(aspectDescriptionChanged(const AbstractAspect *)), 
		this, SLOT(handleAspectDescriptionChanged(const AbstractAspect *)));
	connect(d_project, SIGNAL(aspectAdded(const AbstractAspect *, int)), 
		this, SLOT(handleAspectAdded(const AbstractAspect *, int)));
	connect(d_project, SIGNAL(aspectRemoved(const AbstractAspect *, int)), 
		this, SLOT(handleAspectRemoved(const AbstractAspect *, int)));
	connect(d_project, SIGNAL(aspectAboutToBeRemoved(const AbstractAspect *, int)), 
		this, SLOT(handleAspectAboutToBeRemoved(const AbstractAspect *, int)));
	connect(d_project, SIGNAL(statusInfo(const QString&)),
			statusBar(), SLOT(showMessage(const QString&)));

	handleAspectDescriptionChanged(d_project);

	// init action managers
	foreach(QObject *plugin, QPluginLoader::staticInstances()) 
	{
		ActionManagerOwner * manager_owner = qobject_cast<ActionManagerOwner *>(plugin);
		if (manager_owner) 
			manager_owner->initActionManager();
	}
}

ProjectWindow::~ProjectWindow()
{
	disconnect(d_project, 0, this, 0);
}

void ProjectWindow::handleAspectDescriptionChanged(const AbstractAspect *aspect)
{
	if (aspect != static_cast<AbstractAspect *>(d_project)) return;
	setWindowTitle(d_project->caption() + " - SciDAVis");
}

void ProjectWindow::handleAspectAdded(const AbstractAspect *parent, int index)
{
	AbstractPart *part = qobject_cast<AbstractPart*>(parent->child(index));
	if (!part) return;
	PartMdiView *win = part->mdiSubWindow();
	Q_ASSERT(win);
	d_mdi_area->addSubWindow(win);
	win->show();
	d_menus.part->clear();
	d_menus.part->setEnabled(part->fillProjectMenu(d_menus.part)); 
	connect(win, SIGNAL(statusChanged(PartMdiView *, PartMdiView::SubWindowStatus, PartMdiView::SubWindowStatus)), 
		this, SLOT(handleSubWindowStatusChange(PartMdiView *, PartMdiView::SubWindowStatus, PartMdiView::SubWindowStatus)));
	updateMdiWindowVisibility();
}

void ProjectWindow::handleAspectAboutToBeRemoved(const AbstractAspect *parent, int index)
{
	AbstractPart *part = qobject_cast<AbstractPart*>(parent->child(index));
	if (!part) return;
	PartMdiView *win = part->mdiSubWindow();
	Q_ASSERT(win);
	disconnect(win, SIGNAL(statusChanged(PartMdiView *, PartMdiView::SubWindowStatus, PartMdiView::SubWindowStatus)), 
		this, SLOT(handleSubWindowStatusChange(PartMdiView *, PartMdiView::SubWindowStatus, PartMdiView::SubWindowStatus)));
	d_mdi_area->removeSubWindow(win);
	if (d_mdi_area->currentSubWindow() == 0)
	{
		d_menus.part->clear();
		d_menus.part->setEnabled(false);
	}
}

void ProjectWindow::handleAspectRemoved(const AbstractAspect *parent, int index)
{
	Q_UNUSED(index);
	d_project_explorer->setCurrentAspect(parent);
}

void ProjectWindow::initDockWidgets()
{
	// project explorer
	d_project_explorer_dock = new QDockWidget(this);
	d_project_explorer_dock->setWindowTitle(tr("Project Explorer"));
	d_project_explorer = new ProjectExplorer(d_project_explorer_dock);
	d_project_explorer->setModel(new AspectTreeModel(d_project, this));
	d_project_explorer_dock->setWidget(d_project_explorer);
	addDockWidget(Qt::BottomDockWidgetArea, d_project_explorer_dock);
	connect(d_project_explorer, SIGNAL(currentAspectChanged(AbstractAspect *)),
		this, SLOT(handleCurrentAspectChanged(AbstractAspect *)));
	d_project_explorer->setCurrentAspect(d_project);

	// undo history
	d_history_dock = new QDockWidget(this);
	d_history_dock->setWindowTitle(tr("History"));
	d_undo_view = new QUndoView(d_project->undoStack());
	d_history_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	d_history_dock->setWidget(d_undo_view);
	addDockWidget(Qt::RightDockWidgetArea, d_history_dock);
}

void ProjectWindow::initActions()
{
	d_actions.quit = new QAction(tr("&Quit"), this);
	d_actions.quit->setShortcut(tr("Ctrl+Q"));
	action_manager->addAction(d_actions.quit, "quit");
	connect(d_actions.quit, SIGNAL(triggered(bool)), qApp, SLOT(closeAllWindows()));
		
	d_actions.new_folder = new QAction(tr("New F&older"), this);
	action_manager->addAction(d_actions.new_folder, "new_folder");
	d_actions.new_folder->setIcon(QIcon(QPixmap(":/folder_closed.xpm")));
	connect(d_actions.new_folder, SIGNAL(triggered(bool)), this, SLOT(addNewFolder()));

	d_actions.keyboard_shortcuts_dialog = new QAction(tr("&Keyboard Shortcuts"), this);
	action_manager->addAction(d_actions.keyboard_shortcuts_dialog, "keyboard_shortcuts_dialog");
	connect(d_actions.keyboard_shortcuts_dialog, SIGNAL(triggered(bool)), this, SLOT(showKeyboardShortcutsDialog()));

	d_part_maker_map = new QSignalMapper(this);
	connect(d_part_maker_map, SIGNAL(mapped(QObject*)), this, SLOT(addNewAspect(QObject*)));
	foreach(QObject *plugin, QPluginLoader::staticInstances()) {
		PartMaker *maker = qobject_cast<PartMaker*>(plugin);
		if (maker) {
			QAction *make = maker->makeAction(this);
			connect(make, SIGNAL(triggered()), d_part_maker_map, SLOT(map()));
			d_part_maker_map->setMapping(make, plugin);
			d_part_makers << make;
		}
	}

	d_actions.import_aspect = new QAction(tr("&Import"), this);
	action_manager->addAction(d_actions.import_aspect, "import_aspect");
	// TODO: we need a new icon for generic imports
	d_actions.import_aspect->setIcon(QIcon(QPixmap(":/fileopen.xpm")));
	connect(d_actions.import_aspect, SIGNAL(triggered()), this, SLOT(importAspect()));

	d_actions.cascade_windows = new QAction(tr("&Cascade"), this);
	action_manager->addAction(d_actions.cascade_windows, "cascade_windows");
	connect(d_actions.cascade_windows, SIGNAL(triggered()), d_mdi_area, SLOT(cascadeSubWindows()));
	
	d_actions.tile_windows = new QAction(tr("&Tile"), this);
	action_manager->addAction(d_actions.tile_windows, "tile_windows");
	connect(d_actions.tile_windows, SIGNAL(triggered()), d_mdi_area, SLOT(tileSubWindows()));

	d_actions.next_subwindow = new QAction(tr("&Next","next window"), this);
	d_actions.next_subwindow->setIcon(QIcon(QPixmap(":/next.xpm")));
	d_actions.next_subwindow->setShortcut(tr("F5","next window shortcut"));
	action_manager->addAction(d_actions.next_subwindow, "next_subwindow");
	connect(d_actions.next_subwindow, SIGNAL(triggered()), d_mdi_area, SLOT(activateNextSubWindow()));

	d_actions.previous_subwindow = new QAction(tr("&Previous","previous window"), this);
	d_actions.previous_subwindow->setIcon(QIcon(QPixmap(":/prev.xpm")));
	d_actions.previous_subwindow->setShortcut(tr("F6", "previous window shortcut"));
	action_manager->addAction(d_actions.previous_subwindow, "previous_subwindow");
	connect(d_actions.previous_subwindow, SIGNAL(triggered()), d_mdi_area, SLOT(activatePreviousSubWindow()));

	d_actions.close_current_window = new QAction(tr("Close &Window"), this);
	d_actions.close_current_window->setIcon(QIcon(QPixmap(":/close.xpm")));
	d_actions.close_current_window->setShortcut(tr("Ctrl+W", "close window shortcut"));
	action_manager->addAction(d_actions.close_current_window, "close_current_window");
	connect(d_actions.close_current_window, SIGNAL(triggered()), d_mdi_area, SLOT(closeActiveSubWindow()));

	d_actions.close_all_windows = new QAction(tr("Close &All Windows"), this);
	action_manager->addAction(d_actions.close_all_windows, "close_all_windows");
	connect(d_actions.close_all_windows, SIGNAL(triggered()), d_mdi_area, SLOT(closeAllSubWindows()));

	// TODO: duplicate action (or maybe in the part menu?)
}

void ProjectWindow::initMenus()
{
	d_menus.file = menuBar()->addMenu(tr("&File"));
	d_menus.new_aspect = d_menus.file->addMenu(tr("&New"));
	d_menus.new_aspect->addAction(d_actions.new_folder);
	foreach(QAction *a, d_part_makers)
		d_menus.new_aspect->addAction(a);

	d_menus.file->addAction(d_actions.quit);

	d_menus.edit = menuBar()->addMenu(tr("&Edit"));
	d_menus.edit->addAction(d_project->undoAction(d_menus.edit));
	d_menus.edit->addAction(d_project->redoAction(d_menus.edit));
	d_menus.edit->addSeparator();
	d_menus.edit->addAction(d_actions.keyboard_shortcuts_dialog);

	d_menus.view = menuBar()->addMenu(tr("&View"));

	d_menus.toolbars = createToolbarsMenu();
	if(!d_menus.toolbars) d_menus.toolbars = new QMenu(this);
	d_menus.toolbars->setTitle(tr("Toolbars"));
	
	d_menus.dockwidgets = createDockWidgetsMenu();
	if(!d_menus.dockwidgets) d_menus.dockwidgets = new QMenu(this);
	d_menus.dockwidgets->setTitle(tr("Dock Widgets"));

	d_menus.view->addMenu(d_menus.toolbars);
	d_menus.view->addMenu(d_menus.dockwidgets);
	d_menus.view->addSeparator();

	d_menus.part = menuBar()->addMenu(QString());
	d_menus.part->hide();

	d_menus.windows = menuBar()->addMenu(tr("&Windows"));
	connect( d_menus.windows, SIGNAL(aboutToShow()), this, SLOT(handleWindowsMenuAboutToShow()) );
	
	d_menus.win_policy_menu = new QMenu(tr("Show &Windows"));
	QActionGroup * policy_action_group = new QActionGroup(d_menus.win_policy_menu);
	policy_action_group->setExclusive(true);

	d_actions.visibility_folder = new QAction(tr("Current &Folder Only"), policy_action_group);
	action_manager->addAction(d_actions.visibility_folder, "visibility_folder");
	d_actions.visibility_folder->setCheckable(true);
	d_actions.visibility_folder->setData(Project::folderOnly);
	d_actions.visibility_subfolders = new QAction(tr("Current Folder and &Subfolders"), policy_action_group);
	action_manager->addAction(d_actions.visibility_subfolders, "visibility_subfolders");
	d_actions.visibility_subfolders->setCheckable(true);
	d_actions.visibility_subfolders->setData(Project::folderAndSubfolders);
	d_actions.visibility_all = new QAction(tr("&All"), policy_action_group);
	action_manager->addAction(d_actions.visibility_all, "visibility_all");
	d_actions.visibility_all->setCheckable(true);
	d_actions.visibility_all->setData(Project::all);
	connect(policy_action_group, SIGNAL(triggered(QAction*)), this, SLOT(setMdiWindowVisibility(QAction*)));
	d_menus.win_policy_menu->addActions(policy_action_group->actions());
	connect( d_menus.win_policy_menu, SIGNAL(aboutToShow()), this, SLOT(handleWindowsPolicyMenuAboutToShow()) );
	
	d_menus.view->addMenu(d_menus.win_policy_menu);
}

void ProjectWindow::initToolBars()
{
	d_toolbars.file = new QToolBar( tr( "File" ), this );
	d_toolbars.file->setObjectName("file_toolbar"); // this is needed for QMainWindow::restoreState()
	addToolBar( Qt::TopToolBarArea, d_toolbars.file );
	
	d_buttons.new_aspect = new QToolButton(this);
	d_buttons.new_aspect->setPopupMode(QToolButton::InstantPopup);
	d_buttons.new_aspect->setIcon(QPixmap(":/new_aspect.xpm"));
	d_buttons.new_aspect->setToolTip(tr("New Aspect"));
	d_toolbars.file->addWidget(d_buttons.new_aspect);

	d_toolbars.file->addAction(d_actions.import_aspect);

	d_toolbars.edit = new QToolBar( tr("Edit"), this);
	d_toolbars.edit->setObjectName("edit_toolbar");
	addToolBar(Qt::TopToolBarArea, d_toolbars.edit);

	d_toolbars.edit->addAction(d_project->undoAction(d_toolbars.edit));
	d_toolbars.edit->addAction(d_project->redoAction(d_toolbars.edit));
}

void ProjectWindow::addNewFolder()
{
	addNewAspect(new Folder("Folder1"));
}

QMenu * ProjectWindow::createToolbarsMenu()
{
    QMenu *menu = 0;
    QList<QToolBar *> toolbars = qFindChildren<QToolBar *>(this);
    if (toolbars.size())
	{
        menu = new QMenu(this);
		foreach(QToolBar *toolbar, toolbars)
		{
            if (toolbar->parentWidget() == this)
                menu->addAction(toolbar->toggleViewAction());
        }
    }
    return menu;
}

QMenu * ProjectWindow::createDockWidgetsMenu()
{
    QMenu *menu = 0;
    QList<QDockWidget *> dockwidgets = qFindChildren<QDockWidget *>(this);
    if (dockwidgets.size())
	{
        menu = new QMenu(this);
		foreach(QDockWidget *widget, dockwidgets)
		{
            if (widget->parentWidget() == this)
                menu->addAction(widget->toggleViewAction());
        }
    }
    return menu;
}

void ProjectWindow::addNewAspect(QObject* obj)
{
	AbstractAspect *aspect = qobject_cast<AbstractAspect*>(obj);
	if (!aspect) {
		PartMaker *maker = qobject_cast<PartMaker*>(obj);
		if (!maker) return;
		aspect = maker->makePart();
	}
	QModelIndex index = d_project_explorer->currentIndex();

	if(!index.isValid()) 
		d_project->addChild(aspect);
	else
	{
		AbstractAspect * parent_aspect = static_cast<AbstractAspect *>(index.internalPointer());
		Q_ASSERT(parent_aspect->folder()); // every aspect contained in the project should have a folder
		parent_aspect->folder()->addChild(aspect);
	}
}

void ProjectWindow::handleCurrentAspectChanged(AbstractAspect *aspect)
{
	if(!aspect) aspect = d_project; // should never happen, just in case
	if(aspect->folder() != d_current_folder)
	{
		d_current_folder = aspect->folder();
		updateMdiWindowVisibility();
	}
	if(aspect != d_current_aspect)
	{
		AbstractPart * part = qobject_cast<AbstractPart*>(aspect);
		if (part)
			d_mdi_area->setActiveSubWindow(part->mdiSubWindow());
	}
	d_current_aspect = aspect;
}

void ProjectWindow::handleCurrentSubWindowChanged(QMdiSubWindow* win) 
{
	PartMdiView *view = qobject_cast<PartMdiView*>(win);
	d_menus.part->clear();
	d_menus.part->setEnabled(false);
	if (!view) return;
	emit partActivated(view->part());
	if (view->status() == PartMdiView::Visible)
		d_menus.part->setEnabled(view->part()->fillProjectMenu(d_menus.part)); 
	d_project_explorer->setCurrentAspect(view->part());
}

void ProjectWindow::handleSubWindowStatusChange(PartMdiView * view, PartMdiView::SubWindowStatus from, PartMdiView::SubWindowStatus to) 
{
	if (view == d_mdi_area->currentSubWindow())
	{
		d_menus.part->clear();
		if (from == PartMdiView::Hidden && to == PartMdiView::Visible)
			d_menus.part->setEnabled(view->part()->fillProjectMenu(d_menus.part)); 
		else if (to == PartMdiView::Hidden && from == PartMdiView::Visible)
			d_menus.part->setEnabled(false);
	}
}

void ProjectWindow::updateMdiWindowVisibility()
{
	QList<QMdiSubWindow *> windows = d_mdi_area->subWindowList();
	PartMdiView * part_view;
	switch(d_project->mdiWindowVisibility()) 
	{
		case Project::all:
			foreach(QMdiSubWindow *window, windows) 
			{
				part_view = qobject_cast<PartMdiView *>(window);
				Q_ASSERT(part_view);
				part_view->show();
			}
			break;
		case Project::folderOnly:
			foreach(QMdiSubWindow *window, windows) 
			{
				part_view = qobject_cast<PartMdiView *>(window);
				Q_ASSERT(part_view);
				if(part_view->part()->folder() == d_current_folder)
					part_view->show();
				else
					part_view->hide();
			}
			break;
		case Project::folderAndSubfolders:
			foreach(QMdiSubWindow *window, windows) 
			{
				part_view = qobject_cast<PartMdiView *>(window);
				if(part_view->part()->isDescendantOf(d_current_folder))
					part_view->show();
				else
					part_view->hide();
			}
			break;
	}
}

void ProjectWindow::importAspect()
{
	QMap<QString, AbstractImportFilter*> filter_map;

	foreach(QObject * plugin, QPluginLoader::staticInstances()) {
		FileFormat * ff = qobject_cast<FileFormat*>(plugin);
		if (!ff) continue;
		AbstractImportFilter *filter = ff->makeImportFilter();
		filter_map[filter->nameAndPatterns()] = filter;
	}

	ImportDialog *id = new ImportDialog(filter_map, this);
	if (id->exec() != QDialog::Accepted)
		return;

	AbstractImportFilter * filter = filter_map[id->selectedFilter()];
	QFile file;
	switch (id->destination()) {
		case ImportDialog::CurrentProject:
			foreach(QString file_name, id->selectedFiles()) {
				file.setFileName(file_name);
				if (!file.open(QIODevice::ReadOnly)) {
					statusBar()->showMessage(tr("Could not open file \"%1\".").arg(file_name));
					return;
				}
				AbstractAspect * aspect = filter->importAspect(&file);
				file.close();
				if (aspect->inherits("Project")) {
					Folder * folder = new Folder(aspect->name());
					folder->setComment(aspect->comment());
					folder->setCaptionSpec(aspect->captionSpec());
					for (int i=0; i<aspect->childCount(); i++) {
						// TODO: implement reparenting of Aspects
					}
					addNewAspect(folder);
					delete aspect;
				} else
					addNewAspect(aspect);
			}
			break;
		case ImportDialog::NewProjects:
			foreach(QString file_name, id->selectedFiles()) {
				file.setFileName(file_name);
				if (!file.open(QIODevice::ReadOnly)) {
					statusBar()->showMessage(tr("Could not open file \"%1\".").arg(file_name));
					return;
				}
				AbstractAspect * aspect = filter->importAspect(&file);
				file.close();
				if (aspect->inherits("Project")) {
					static_cast<Project*>(aspect)->view()->showMaximized();
				} else {
					Project * project = new Project();
					project->addChild(aspect);
					project->view()->showMaximized();
				}
			}
			break;
	}
	qDeleteAll(filter_map);
	delete id;
}

void ProjectWindow::showKeyboardShortcutsDialog()
{
	QList<ActionManager *> managers;
	managers.append(action_manager);
	// TODO: add action manager for project window first
	foreach(QObject *plugin, QPluginLoader::staticInstances()) 
	{
		ActionManagerOwner * manager_owner = qobject_cast<ActionManagerOwner *>(plugin);
		if (manager_owner) 
			managers.append(manager_owner->actionManager());
	}
	ShortcutsDialog dialog(managers, this);
	dialog.setWindowTitle(tr("Customize Keyboard Shortcuts"));
	dialog.resize(width()-width()/5, height()-height()/5);
	dialog.exec();
}

void ProjectWindow::handleWindowsMenuAboutToShow()
{
	d_menus.windows->clear();
	d_menus.windows->addAction(d_actions.cascade_windows);
	d_menus.windows->addAction(d_actions.tile_windows);
	d_menus.windows->addSeparator();
	d_menus.windows->addAction(d_actions.next_subwindow);
	d_menus.windows->addAction(d_actions.previous_subwindow);
	d_menus.windows->addSeparator();
	d_menus.windows->addAction(d_actions.close_current_window);
	d_menus.windows->addAction(d_actions.close_all_windows);
	d_menus.windows->addSeparator();

	d_actions.close_current_window->setEnabled(d_mdi_area->currentSubWindow() != 0 && d_mdi_area->currentSubWindow()->isVisible());

	QList<QMdiSubWindow *> windows = d_mdi_area->subWindowList();
	foreach(QMdiSubWindow *win, windows)
		if (!win->isVisible()) windows.removeAll(win);

	foreach(QMdiSubWindow *win, windows)
	{
		QAction * action = d_menus.windows->addAction(win->windowTitle());
		connect(action, SIGNAL(triggered()), win, SLOT(setFocus()));
		action->setCheckable(true);
		action->setChecked( d_mdi_area->activeSubWindow() == win );
	}
}

void ProjectWindow::handleWindowsPolicyMenuAboutToShow()
{
	if (d_project->mdiWindowVisibility() == Project::folderOnly) 
		d_actions.visibility_folder->setChecked(true);
	else if (d_project->mdiWindowVisibility() == Project::folderAndSubfolders) 
		d_actions.visibility_subfolders->setChecked(true);
	else
		d_actions.visibility_all->setChecked(true);
}

QMenu *ProjectWindow::createContextMenu() const
{
	QMenu * menu = new QMenu(); // no remove action from AbstractAspect in the project context menu

	menu->addMenu(d_menus.new_aspect);
	menu->addMenu(d_menus.win_policy_menu);
	menu->addSeparator();

	// TODO:
	// Find
	// ----
	// Append Project
	// Save Project As
	// ----
	
	return menu;
}

QMenu *ProjectWindow::createFolderContextMenu(const Folder * folder) const
{
	QMenu * menu = folder->AbstractAspect::createContextMenu();
	Q_ASSERT(menu);
	menu->addSeparator();

	menu->addMenu(d_menus.new_aspect);

	if (d_project->mdiWindowVisibility() == Project::folderOnly) 
		d_actions.visibility_folder->setChecked(true);
	else if (d_project->mdiWindowVisibility() == Project::folderAndSubfolders) 
		d_actions.visibility_subfolders->setChecked(true);
	else
		d_actions.visibility_all->setChecked(true);
	menu->addMenu(d_menus.win_policy_menu);
	menu->addSeparator();

	return menu;
}

void ProjectWindow::setMdiWindowVisibility(QAction * action) 
{
	d_project->setMdiWindowVisibility((Project::MdiWindowVisibility)(action->data().toInt()));
}
		