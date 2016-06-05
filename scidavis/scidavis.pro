# enable C++11 support
greaterThan(QT_MAJOR_VERSION, 4){
  CONFIG += c++11
} else {
  QMAKE_CXXFLAGS += -std=c++0x
}

INCLUDEPATH += ../libscidavis ../libscidavis/src 
LIBS += -L../libscidavis -lscidavis

POST_TARGETDEPS=../libscidavis/libscidavis.a

include(../config.pri)
include( basic.pri )
python {include( python.pri )}

#QMAKE_CLEAN+=${TARGET}
# why doesn't the previous line work???
win32 {
QMAKE_CLEAN+=scidavis.exe
} else {
QMAKE_CLEAN+=scidavis
}

### this is the program itself
INSTALLS        += target

### README, INSTALL.html, manual (if present in the manual subfolder), etc.
INSTALLS        += documentation

### translations
INSTALLS        += translationfiles

### icon file (for Windows installer)
win32:INSTALLS  += win_icon

liborigin {
mxe {
  LIBS += ../3rdparty/liborigin/release/liborigin.a
  POST_TARGETDEPS += ../3rdparty/liborigin/release/liborigin.a
} else {
  LIBS += ../3rdparty/liborigin/liborigin.a
  POST_TARGETDEPS += ../3rdparty/liborigin/liborigin.a
}
}

########### Future code backported from the aspect framework ##################
DEFINES += LEGACY_CODE_0_2_x
INCLUDEPATH += ../libscidavis/src/future

