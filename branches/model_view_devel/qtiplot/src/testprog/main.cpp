#include <QApplication>
#include <QMainWindow>
#include <QtDebug>
#include <QTime>
#include <QDate>
#include <QString>

#include "StringColumnData.h"
#include "TableView.h"
#include "TableModel.h"


int main(int argc, char **argv)
{
	QApplication a(argc,argv);


	QMainWindow mw;
	mw.resize(800,600);

	TableView tw(&mw, new TableModel(&mw));
	tw.resize(600,400);
	mw.show();

	a.exec();

	return 0;
}
