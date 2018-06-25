#include "ApplicationWindow.h"

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

  void basicWindowTests();
  void deleteSelectedItems();
  void showWindowPopupMenu();

  void fft();

  void menus();
public:
  static unsigned numInfos, numWarnings;
};
