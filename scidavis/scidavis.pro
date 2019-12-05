# enable C++11 support
equals(QT_MAJOR_VERSION, 5) { CONFIG += c++11 }
equals(QT_MAJOR_VERSION, 4) { QMAKE_CXXFLAGS += -std=c++11 }
equals(QT_MAJOR_VERSION, 3) { QMAKE_CXXFLAGS += -std=c++0x }

INCLUDEPATH += ../libscidavis ../libscidavis/src 
LIBS += -L../libscidavis -lscidavis

POST_TARGETDEPS=../libscidavis/libscidavis.a

include(../config.pri)
include( basic.pri )
python {
  PYTHONBIN = $$(PYTHON)
  isEmpty( PYTHONBIN ) {
    PYTHONBIN = python3
  }
  unix: {
        LIBS+=$$system($$PYTHONBIN findBoostPythonLib.py)
        LIBS+=$$system($$PYTHONBIN-config --ldflags)
      }
  message($$LIBS)  
}

QMAKE_CLEAN += $$TARGET

### this is the program itself
INSTALLS        += target

### README, INSTALL.html, manual (if present in the manual subfolder), etc.
INSTALLS        += documentation

### translations
INSTALLS        += translationfiles

### icon file (for Windows installer)
win32:INSTALLS  += win_icon

liborigin {
  !packagesExist(liborigin) | !contains(PRESET, linux_package) {
    LIBORIGINDIR=../3rdparty/liborigin
    win32 {
      Debug: LIBORIGINDIR=$${LIBORIGINDIR}/debug
      Release: LIBORIGINDIR=$${LIBORIGINDIR}/release
    }
    LIBS += $${LIBORIGINDIR}/liborigin.a
    POST_TARGETDEPS += $${LIBORIGINDIR}/liborigin.a
  } else {
    CONFIG += link_pkgconfig
    PKGCONFIG += liborigin
  }
}

########### Future code backported from the aspect framework ##################
DEFINES += LEGACY_CODE_0_2_x
INCLUDEPATH += ../libscidavis/src/future

###################### DESKTOP INTEGRATION ##################################

unix {
	desktop_entry.files = scidavis.desktop
	desktop_entry.path = "$$INSTALLBASE/share/applications"

	mime_package.files = scidavis.xml
	mime_package.path = "$$INSTALLBASE/share/mime/packages"

	man_page.files = scidavis.1
	man_page.path = "$$INSTALLBASE/share/man/man1"

	#deprecated
	mime_link.files = x-sciprj.desktop
	mime_link.path = "$$INSTALLBASE/share/mimelnk/application"
	
	contains(INSTALLS, icons) {
		# scalable icon
		icons.files = icons/scidavis.svg
		icons.path = "$$INSTALLBASE/share/icons/hicolor/scalable/apps"

		# hicolor icons for different resolutions
		resolutions = 16 22 32 48 64 128
		for(res, resolutions) {
			eval(icon_hicolor_$${res}.files = icons/hicolor-$${res}/scidavis.png)
			eval(icon_hicolor_$${res}.path = "$$INSTALLBASE/share/icons/hicolor/$${res}x$${res}/apps")
			INSTALLS += icon_hicolor_$${res}
		}

		# locolor icons for different resolutions
		resolutions = 16 22 32
		for(res, resolutions) {
			eval(icon_locolor_$${res}.files = icons/locolor-$${res}/scidavis.png)
			eval(icon_locolor_$${res}.path = "$$INSTALLBASE/share/icons/locolor/$${res}x$${res}/apps")

			INSTALLS += icon_locolor_$${res}
		}
	}
}
