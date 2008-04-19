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
unix: LUPDATE_BIN = lupdate-qt4
unix: LRELEASE_BIN = lrelease-qt4
win32: LUPDATE_BIN = lupdate
win32: LRELEASE_BIN = lrelease

### 64 Linux only suffix
linux-g++-64: libsuff = 64 

### where to install
unix: INSTALLBASE = /usr           # this is what is called "prefix" when using GNU autotools
win32: INSTALLBASE = c:/scidavis
unix: target.path = "$$INSTALLBASE/bin"               # where to install the binary on Linux/MacOS X
win32: target.path = "$$INSTALLBASE"                  # where to install the exe on Windows
unix: documentation.path = "$$INSTALLBASE/share/doc/scidavis"      # where to install the documentation files on Linux/MacOS X
win32: documentation.path = "$$INSTALLBASE"                        # ... on Winodws
### Usually, the manual will be expected in the "manual" subfolder of "documentation.path" (see above).
### You can override this, uncomment and adjust the path behind the '=' in the next line.
# manual.path = $$INSTALLBASE/share/doc/scidavis/manual
### Important: translationfiles.path will be the directory where scidavis expects
### the translation .qm files at runtime. Therefore you need to set it corretly even if 
### you do not use this project file to generate the translation files.
unix: translationfiles.path = "$$INSTALLBASE/share/scidavis/translations"
win32: translationfiles.path = "$$INSTALLBASE/translations"
### Important (if you use Python): the following two paths are where the application will expect 
### scidavisrc.py and scidavisUtil.py, respectively. Alternatively you can also put scidavisrc.py 
### (or ".scidavis.py") into the users home directory. scidavisUtil.py must be either in the 
### directory specified here or somewhere else in the python path (sys.path) where "import" can find it
unix: pythonconfig.path = /etc						# where scidavisrc.py is installed
win32: pythonconfig.path = $$INSTALLBASE    
unix: pythonutils.path = $$INSTALLBASE/share/scidavis        # where the scidavisUtil python modules is installed
win32: pythonutils.path = $$INSTALLBASE        

### (remark: muparser.pri and python.pri must be included after defining INSTALLBASE )
### building without muParser does not work yet (but will in a future version)
include( muparser.pri )
### comment out the following line to deactivate Python scripting support
include( python.pri )

### Note to packagers: If you want to use systemwide installed liborigin
### instead of the one provided in "3rdparty", uncomment the following 2 lines:
# CONFIG += dynamic_liborigin
# LIBS += -lorigin
### Unfortunately, due to liborigin being in alpha stage, we cannot promise
### that SciDAVis works with any other version that the one in "3rdparty".

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
unix:LIBS         += -lgsl -lgslcblas -lz -lmuparser

#############################################################################
### Link everything dynamically
#############################################################################

#unix:INCLUDEPATH  += /usr/include/qwt5
#unix:LIBS         += -L/usr/lib$${libsuff}
## dynamically link against Qwt(3D) installed system-wide
## WARNING: make sure they are compiled against >= Qt4.2
## Mixing Qt 4.2 and Qt >= 4.3 compiled stuff may also 
## cause problems.
#unix:LIBS         += -lqwtplot3d
#unix:LIBS         += -lqwt
##dynamically link against GSL and zlib installed system-wide
#unix:LIBS         += -lgsl -lgslcblas -lz -lmuparser

#############################################################################
### Default settings for Windows
#############################################################################
### Static linking mostly, except Qt, Python and QwtPlot3D.
### The latter seems to be impossible to link statically on Windows.
#############################################################################

win32:INCLUDEPATH       += c:/qwtplot3d/include
win32:INCLUDEPATH       += c:/qwt-5.0.2/include
win32:INCLUDEPATH       += c:/gsl/include
win32:INCLUDEPATH       += c:/zlib/include
win32:INCLUDEPATH       += c:/muparser/include

win32:LIBS        += c:/qwtplot3d/lib/qwtplot3d.dll
win32:LIBS        += c:/qwt-5.0.2/lib/libqwt.a
win32:LIBS        += c:/gsl/lib/libgsl.a
win32:LIBS        += c:/gsl/lib/libgslcblas.a
win32:LIBS        += c:/zlib/lib/libz.a
win32:LIBS        += c:/muparser/lib/libmuparser.a

#############################################################################
###                    END OF USER-SERVICEABLE PART                       ###
#############################################################################

include( basic.pri )
include( sourcefiles.pri )

#############################################################################
#############################################################################
