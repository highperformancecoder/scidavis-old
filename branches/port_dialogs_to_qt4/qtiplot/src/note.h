#ifndef NOTE_H
#define NOTE_H

#include "widget.h"	
#include <q3textedit.h>

class Note: public myWidget
{
    Q_OBJECT

public:

	Note(const QString& label, QWidget* parent=0, const char* name=0, Qt::WFlags f=0);
	~Note(){};

	Q3TextEdit *te;
		
	void init();

public slots:
	QString saveToString(const QString &info);

	Q3TextEdit* textWidget(){return te;};
	QString text(){return te->text();};
	void setText(const QString &s){te->setText(s);};
	void modifiedNote();
	void print();	
};
   
#endif
