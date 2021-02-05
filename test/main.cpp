/*
  @copyright Russell Standish 2012
  @author Russell Standish

  Open source licensed under the MIT license.

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <QApplication>
#include <QMessageBox>
#include <gtest/gtest.h>
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
using std::string;

int main(int argc, char **argv)
{
    QApplication app { argc, argv };

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

typedef QMessageBox::StandardButton StandardButton;
unsigned numInfos = 0, numWarnings = 0;

#ifndef _WIN32
// override the QmessageBox static methods to turn a failure messages into throws, and ignore
// warnings
StandardButton QMessageBox::information(QWidget *, const QString &, const QString &text,
                                        StandardButtons, StandardButton)
{
    cerr << text.toStdString() << endl;
    numInfos++;
    return QMessageBox::Ok;
}

StandardButton QMessageBox::question(QWidget *, const QString &, const QString &text,
                                     StandardButtons, StandardButton)
{
    cerr << text.toStdString() << endl;
    return QMessageBox::Yes;
}

int QMessageBox::question(QWidget *, const QString &, const QString &text, int, int, int)
{
    cerr << text.toStdString() << endl;
    return QMessageBox::Yes;
}

StandardButton QMessageBox::warning(QWidget *, const QString &, const QString &text,
                                    StandardButtons, StandardButton)
{
    cerr << text.toStdString() << endl;
    numWarnings++;
    return QMessageBox::Ok;
}

StandardButton QMessageBox::critical(QWidget *, const QString &, const QString &text,
                                     StandardButtons, StandardButton)
{
    throw std::runtime_error(text.toStdString());
}
#endif
