#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <thread>
#include <chrono>
#include <QString>

inline std::ostream &operator<<(std::ostream &o, const QString &s)
{
    return o << s.toStdString();
}

extern unsigned numInfos, numWarnings;

/// waits a second, then emits a carriage return to all open
/// dialogs. Useful if a method call posts a dialog
struct CloseDialog : public std::thread
{
    CloseDialog()
        : std::thread([]() {
              std::this_thread::sleep_for(std::chrono::seconds(1));
              for (auto i : QApplication::topLevelWidgets())
                  if (auto w = dynamic_cast<QDialog *>(i))
                      QApplication::postEvent(
                              w,
                              new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Return, Qt::NoModifier));
          })
    {
    }
    ~CloseDialog() { join(); }
};

#endif
