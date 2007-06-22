#ifndef WIDGET_H
#define WIDGET_H

#include <qwidget.h>

class Folder;

class myWidget: public QWidget
{
	Q_OBJECT

public:	
	
	myWidget(const QString& label, QWidget * parent, const char * name, WFlags f);
	~myWidget(){};

	enum CaptionPolicy{Name = 0, Label = 1, Both = 2};
	enum Status{Hidden = -1, Normal = 0, Minimized = 1, Maximized = 2};

	QString windowLabel(){return QString(w_label);};
	void setWindowLabel(const QString& s){w_label = s;};

	CaptionPolicy captionPolicy(){return caption_policy;};
	void setCaptionPolicy(CaptionPolicy policy);

	QString birthDate(){return birthdate;};
	void setBirthDate(const QString& s){birthdate = s;};

	QString aspect();
	Status status(){return w_status;};
	void setStatus(Status s){w_status = s;};

	virtual QString saveAsTemplate(const QString& ){return QString::null;};
	virtual void restore(const QStringList& ){};

	virtual void print(){};
	virtual QString saveToString(const QString &){return QString::null;};
	
	//! Size of the widget as a string
	virtual QString sizeToString();

	//!Notifies a change in the status of a former maximized window after it was shown as normal as a result of an indirect user action (e.g.: another window was maximized)
	void setNormal();

	//!Notifies that a window was hidden by a direct user action
	void setHidden();

	//event handlers
	void closeEvent( QCloseEvent *);
	void askOnCloseEvent(bool ask){askOnClose = ask;};
	bool event( QEvent *e );
	bool eventFilter(QObject *object, QEvent *e);

	void showMaximized();
	//! Tells if a resize event was requested by the user or generated programatically
	bool userRequested(){return user_request;};

	//! Returns the pointer to the parent folder of the window
	Folder* folder(){return parentFolder;};

	//! Initializes the pointer to the parent folder of the window
	void setFolder(Folder* f){parentFolder = f;};

	void reparent(QWidget * parent, WFlags f, const QPoint & p, bool showIt = false);

signals:  
	void closedWindow(myWidget *);
	void hiddenWindow(myWidget *);
	void modifiedWindow(QWidget *);
	void resizedWindow(QWidget *);
	void statusChanged(myWidget *);
	void showTitleBarMenu();

protected:
	QWidget *titleBar;
	
private:
	//!Pointer to the parent folder of the window
	Folder *parentFolder;
	QString w_label, birthdate;
	Status w_status;
	CaptionPolicy caption_policy;
	bool askOnClose;
	//! Tells if the showMaximized action was requested by the user or generated programatically
	bool user_request;
};

#endif
