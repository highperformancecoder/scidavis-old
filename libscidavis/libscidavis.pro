# enable C++11 support
equals(QT_MAJOR_VERSION, 5) { CONFIG += c++11 }
equals(QT_MAJOR_VERSION, 4) { QMAKE_CXXFLAGS += -std=c++11}
equals(QT_MAJOR_VERSION, 3) { QMAKE_CXXFLAGS += -std=c++0x }

TEMPLATE=lib
CONFIG+=staticlib uic
TARGET=scidavis
QMAKE_CLEAN += $$TARGET

include(../config.pri)

INCLUDEPATH += ../scidavis
QMAKE_CXXFLAGS += -ftemplate-backtrace-limit=0

liborigin {
  !packagesExist(liborigin) | !contains(PRESET, linux_package) {
    INCLUDEPATH += ../3rdparty/liborigin
  } else {
    CONFIG += link_pkgconfig
    PKGCONFIG += liborigin
  }
}

CONFIG        += qt warn_on exceptions opengl thread zlib

DEFINES       += QT_PLUGIN
DEFINES       += TS_PATH="\\\"$$replace(translationfiles.path," ","\\ ")\\\""
DEFINES       += DOC_PATH="\\\"$$replace(documentation.path," ","\\ ")\\\""
!isEmpty( manual.path ) {
DEFINES       += MANUAL_PATH="\\\"$$replace(manual.path," ","\\ ")\\\""
}
!isEmpty(plugins.path): DEFINES += PLUGIN_PATH=\\\"$$replace(plugins.path," ","\\ ")\\\"

!mxe {
     win32:DEFINES += QT_DLL QT_THREAD_SUPPORT
}

QT            += opengl network svg xml
equals(QT_MAJOR_VERSION, 5) { QT += printsupport }

MOC_DIR        = ../tmp/scidavis
OBJECTS_DIR    = ../tmp/libscidavis
DESTDIR        = ./

include( sourcefiles.pri )
include( muparser.pri )
python {include( python.pri )}



#############################################################################
#############################################################################
