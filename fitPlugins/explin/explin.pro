QMAKE_PROJECT_DEPTH = 0
linux-g++-64: libsuff=64

TARGET = explin
TEMPLATE = lib
CONFIG += dll
CONFIG -= qt
CONFIG += release
DESTDIR = ../

INSTALLS += target
unix:  target.path = /usr/lib$${libsuff}/scidavis/plugins
win32: target.path = c:/scidavis/plugins

win32:INCLUDEPATH += c:/gsl/include
win32:LIBS        += c:/gsl/lib/libgsl.a

unix:LIBS += -L/usr/lib$${libsuff} -lgsl

SOURCES = explin.c

