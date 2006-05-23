#ifndef NOTE_H
#define NOTE_H

#include "widget.h"	
#include "scriptedit.h"
#include <qtextedit.h>

class ScriptingEnv;

class Note: public myWidget
{
    Q_OBJECT

public:

	Note(ScriptingEnv *env, const QString& label, QWidget* parent=0, const char* name=0, WFlags f=0);
	~Note(){};

		
	void init(ScriptingEnv *env);

public slots:
	QString saveToString(const QString &info);
	void restore(const QStringList&);

	QTextEdit* textWidget(){return (QTextEdit*)te;};
	QString text(){return te->text();};
	void setText(const QString &s){te->setText(s);};
	bool autoexec() const { return autoExec; }
	void setAutoexec(bool);
	void execute();
	void modifiedNote();
	void print() { te->print(); }

private:
	ScriptEdit *te;
	bool autoExec;
};
   
#endif
