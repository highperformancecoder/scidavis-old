#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <qwidget.h>
//Added by qt3to4:
#include <Q3Frame>

class QPushButton;
class Q3Frame;

class ColorButton : public QWidget
{
  Q_OBJECT

public:
  ColorButton(QWidget *parent);
  void setColor(const QColor& c);
  QColor color() const;

  QPushButton *selectButton;
  Q3Frame *display;

signals:
  void clicked();

protected:
  void init();
};

#endif
