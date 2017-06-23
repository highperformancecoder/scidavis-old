#include "unittests.h"
#include "ApplicationWindow.h"
#include "MultiLayer.h"

#include <iostream>
#include <memory>
#include <stdexcept>
using namespace std;


// Override showHelp() & chooseHelpFolder() to suppress documentation file 
// path not found error. Need to fix this later (importance : high)
void Unittests::showHelp() {}
void Unittests::chooseHelpFolder() {}

QTEST_MAIN(Unittests);

typedef QMessageBox::StandardButton StandardButton;

// override the QmessageBox static methods to turn a failure messages into throws, and ignore warnings
StandardButton QMessageBox::information
(QWidget *, const QString&, const QString& text, StandardButtons,StandardButton) 
{
  cerr << text.toStdString() << endl;
  return QMessageBox::Ok;
}

StandardButton QMessageBox::question
(QWidget *, const QString &, const QString& text, StandardButtons, StandardButton)
{
  cerr << text.toStdString() << endl;
  return QMessageBox::Yes;
}

int QMessageBox::question
(QWidget *, const QString& ,const QString& text, int, int, int)
{
  cerr << text.toStdString() << endl;
  return QMessageBox::Yes;
}

StandardButton QMessageBox::warning
(QWidget *, const QString &, const QString& text, StandardButtons, StandardButton)
{ 
  cerr << text.toStdString() << endl;
  return QMessageBox::Ok;
}
     
StandardButton QMessageBox::critical
(QWidget *, const QString &, const QString& text, StandardButtons, StandardButton)
{throw runtime_error(text.toStdString());}
