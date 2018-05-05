##################### PYTHON + SIP + PyQT #####################

  INSTALLS += pythonconfig
  pythonconfig.files += scidavisrc.py scidavisrc.pyc
  DEFINES       += PYTHON_CONFIG_PATH="\\\"$$replace(pythonconfig.path," ","\\ ")\\\""

  INSTALLS += pythonutils
  pythonutils.files += scidavisUtil.py scidavisUtil.pyc

  compiledScidavisrc.target = scidavisrc.pyc
  compiledScidavisrc.depends = scidavisrc.py
  compiledScidavisrc.commands = python -m py_compile scidavisrc.py
  compiledScidavisUtil.target = scidavisUtil.pyc
  compiledScidavisUtil.depends = scidavisUtil.py
  compiledScidavisUtil.commands = python -m py_compile scidavisUtil.py
  QMAKE_EXTRA_TARGETS += compiledScidavisrc compiledScidavisUtil
  PRE_TARGETDEPS += scidavisrc.pyc scidavisUtil.pyc

  DEFINES       += PYTHON_UTIL_PATH="\\\"$$replace(pythonutils.path," ","\\ ")\\\""

  SIP_DIR = ../tmp/scidavis

  DEFINES += SCRIPTING_PYTHON

  message("Making PyQt bindings via SIP")
  PYTHONBIN = $$(PYTHON)
  isEmpty( PYTHONBIN ) {
    PYTHONBIN = python
  }

  unix {
    INCLUDEPATH += $$system($$PYTHONBIN-config --includes|sed -e 's/-I//')
    osx_dist {
      DEFINES += PYTHONHOME=/Applications/scidavis.app/Contents/Resources
    } else {
      macx {
        LIBS += -framework Python
      } else {
        LIBS += $$system($$PYTHONBIN -c "\"from distutils import sysconfig;import sys; sys.stdout.write('-lpython'+sysconfig.get_config_var('VERSION')+(sysconfig.get_config_var('ABIFLAGS') or ''))\"")
      }
    }     
    LIBS        += -lm
  }

  win32 {
  mxe {
     DEFINES += SIP_STATIC_MODULE
#    QMAKE_LIBPATH += "$$(HOME)/usr/mxe/PyQt4/"
    LIBS += -L"$$(HOME)/usr/mxe/PyQt4/" -lPyQtCore -lPyQtGui -lqpygui -lqpycore -lsip -lpython27
  } else {
    INCLUDEPATH += $$system(call ../python-includepath.py)
    LIBS        += $$system(call ../python-libs-win.py)
  }
}


