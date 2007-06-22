#include "testwrapper.h"


int main(int argc, char **argv)
{
	QApplication a(argc,argv);

	QWidget mw;
	mw.resize(1000,600);
	QHBoxLayout lo(&mw);

	TableViewTestWrapper tw(0, new TableModel(&mw));
	tw.resize(800,400);
	QUndoView uw(tw.undo_stack, &mw);
	uw.resize(200,400);
	lo.addWidget(&tw);
	lo.addWidget(&uw);
	mw.show();
	tw.show();

	a.exec();

	return 0;
}
