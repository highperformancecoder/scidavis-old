#include "ApplicationWindow.h"
#include <QTest>

class Unittests: public ApplicationWindow
{
  Q_OBJECT
private slots:
  void readWriteProject();
  void largeOriginImport();

  void exportTestProject();
  void convertMatrix();
  void note();

  void plot3d();
  
  void lockToolbars();
  
  void showHelp();
  void chooseHelpFolder();

  void fft();
};
