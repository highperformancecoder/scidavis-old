### General remark: lines prefixed with "unix:" are for Linux/MacOS X/*BSD only, they are ignored on Windows
###                 lines prefixed with "win32:" are Windows only

### a console displaying output of scripts; particularly useful on Windows
### where running SciDAVis from a terminal is inconvenient
DEFINES         += SCRIPTING_CONSOLE
### a dialog for selecting the scripting language on a per-project basis
DEFINES         += SCRIPTING_DIALOG

CONFIG          += release
### use the next line to compile with debugging symbols instead of the line obove
#CONFIG          += debug

### what to install
INSTALLS        += target       # this is the program itself
INSTALLS        += documentation     # README, INSTALL.html, manual (if present in the manual subfolder), etc.
### Comment out the next line if you do not want automatic compilation and installation of the translations
INSTALLS        += translationfiles

### names of the lupdate and lrelease programs (needed if you want to compile and install the translations automatically)
### possibly needs to be adapted if you have an unusual installation
exists(/usr/bin/lupdate-qt4) {
	# Debian, Ubuntu, Fedora
	LUPDATE_BIN = lupdate-qt4
	LRELEASE_BIN = lrelease-qt4
} else {
	# anything else
	LUPDATE_BIN = lupdate
	LRELEASE_BIN = lrelease
}

### 64 Linux only suffix
linux-g++-64: libsuff = 64 

### where to install
unix: INSTALLBASE = /usr           # this is what is called "prefix" when using GNU autotools
win32: INSTALLBASE = ../output
unix: target.path = "$$INSTALLBASE/bin"               # where to install the binary on Linux/MacOS X
win32: target.path = "$$INSTALLBASE"                  # where to install the exe on Windows
unix: documentation.path = "$$INSTALLBASE/share/doc/scidavis"      # where to install the documentation files on Linux/MacOS X
win32: documentation.path = "$$INSTALLBASE"                        # ... on Winodws
### Usually, the manual will be expected in the "manual" subfolder of "documentation.path" (see above).
### You can override this, uncomment and adjust the path behind the '=' in the next line.
# manual.path = $$INSTALLBASE/share/doc/scidavis/manual
### Enables choosing of help folder at runtime, instead of relying on the above path only.
### The downside is that the help folder will be remembered as a configuration option, so a binary
### package cannot easily update the path for its users.
### Dynamic selection of the manual path was the only available option up until SciDAVis 0.2.3.
DEFINES += DYNAMIC_MANUAL_PATH
### Important: translationfiles.runtimepath will be the directory where scidavis expects
### the translation .qm files at runtime. Therefore you need to set it corretly even if 
### you do not use this project file to generate the translation files.
unix {
  translationfiles.path = "$$INSTALLBASE/share/scidavis/translations"
  translationfiles.runtimepath = "$$translationfiles.path"
}
win32 {
  translationfiles.path = "$$INSTALLBASE/translations"
  translationfiles.runtimepath = "translations"
}
### Important (if you use Python): the following two paths are where the application will expect 
### scidavisrc.py and scidavisUtil.py, respectively. Alternatively you can also put scidavisrc.py 
### (or ".scidavis.py") into the users home directory. scidavisUtil.py must be either in the 
### directory specified here or somewhere else in the python path (sys.path) where "import" can find it
unix {
  # where scidavisrc.py is installed
  pythonconfig.path = /etc
  # where scidavisrc.py is searched for at runtime
  pythonconfig.runtimepath = "$$pythonconfig.path"
  # where the scidavisUtil python modules is installed
  pythonutils.path = $$INSTALLBASE/share/scidavis
  # where the scidavisUtil python module is searched for at runtime
  pythonutils.runtimepath = "$$pythonutils.path"
}
win32 {
  pythonconfig.path = "$$INSTALLBASE"
  pythonconfig.runtimepath = .
  pythonutils.path = "$$INSTALLBASE"
  pythonutils.runtimepath = .
}

### (remark: muparser.pri and python.pri must be included after defining INSTALLBASE )
### building without muParser does not work yet (but will in a future version)
include( muparser.pri )
### comment out the following line to deactivate Python scripting support
include( python.pri )

################### start of liborigin block 
############ liborigin support has been discontinued due to the lack 
############ of a developer who maintains the Origin import code;
############ if you are interested in reviving Origin support, please contact 
############ the SciDAVis developers
### Note to packagers: If you want to use systemwide installed liborigin
### instead of the one provided in "3rdparty", uncomment the following 2 lines:
# CONFIG += dynamic_liborigin
# LIBS += -lorigin
### Unfortunately, due to liborigin being in alpha stage, we cannot promise
### that SciDAVis works with any other version that the one in "3rdparty".
################### end of liborigin block 

#############################################################################
### Default settings for Linux / Mac OS X
#############################################################################
### Link statically against Qwt and Qwtplot3D (in order to make sure they
### are compiled against Qt4), dynamically against everything else.
#############################################################################

unix:INCLUDEPATH  += ../3rdparty/qwtplot3d/include
unix:LIBS         += ../3rdparty/qwtplot3d/lib/libqwtplot3d.a

unix:INCLUDEPATH  += ../3rdparty/qwt/src
unix:LIBS         += ../3rdparty/qwt/lib/libqwt.a

unix:LIBS         += -L/usr/lib$${libsuff}
unix:LIBS         += -lgsl -lgslcblas
unix:LIBS         += -lmuparser
unix:INCLUDEPATH  += /usr/include/muParser

#############################################################################
### Link everything dynamically
#############################################################################

#unix:INCLUDEPATH  += /usr/include/qwt5
#unix:LIBS         += -L/usr/lib$${libsuff}
## dynamically link against Qwt(3D) installed system-wide
## WARNING: make sure they are compiled against >= Qt4.2
## Mixing Qt 4.2 and Qt >= 4.3 compiled stuff may also 
## cause problems.
#unix:INCLUDEPATH  += /usr/include/qwtplot3d
#unix:LIBS         += -lqwtplot3d
#unix:LIBS         += -lqwt
##dynamically link against GSL and muparser installed system-wide
#unix:LIBS         += -lgsl -lgslcblas -lmuparser

#############################################################################
### Default settings for Windows
#############################################################################
### Static linking mostly, except Qt, Python and QwtPlot3D.
### The latter seems to be impossible to link statically on Windows.
#############################################################################

win32:INCLUDEPATH       += ../3rdparty/qwtplot3d/include
win32:INCLUDEPATH       += ../3rdparty/qwt/src
win32:INCLUDEPATH       += ../3rdparty/gsl-1.8/include
win32:INCLUDEPATH       += ../3rdparty/muparser/include

win32:LIBS        += ../3rdparty/qwtplot3d/lib/qwtplot3d.dll
win32:LIBS        += ../3rdparty/qwt/lib/libqwt.a
win32:LIBS        += ../3rdparty/gsl-1.8/lib/libgsl.a
win32:LIBS        += ../3rdparty/gsl-1.8/lib/libgslcblas.a
win32:LIBS        += ../3rdparty/muparser/lib/libmuparser.a

#############################################################################
###                    END OF USER-SERVICEABLE PART                       ###
#############################################################################

include( basic.pri )
include( sourcefiles.pri )

#############################################################################
#############################################################################
