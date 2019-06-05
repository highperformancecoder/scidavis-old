##################### PYTHON + classdesc + boost #####################

  INSTALLS += pythonconfig
  pythonconfig.files += scidavisrc.py
  DEFINES       += PYTHON_CONFIG_PATH="\\\"$$replace(pythonconfig.path," ","\\ ")\\\""

  INSTALLS += pythonutils
  pythonutils.files += scidavisUtil.py

  DEFINES       += PYTHON_UTIL_PATH="\\\"$$replace(pythonutils.path," ","\\ ")\\\""

  TMP_DIR = ../tmp/scidavis

  DEFINES += SCRIPTING_PYTHON
  HEADERS += src/PythonScript.h src/PythonScripting.h src/Qt.h
  SOURCES += src/PythonScript.cpp src/PythonScripting.cpp src/Qt.cpp src/PythonApplicationWindow.cpp

  CLASSDESC_HEADERS = src/ApplicationWindow.h src/ArrowMarker.h \
                    src/future/core/AbstractAspect.h src/future/core/AbstractColumn.h \
                    src/CurveRangeDialog.h  src/future/core/column/Column.h \
                    src/ExponentialFit.h \
                    src/Filter.h src/Fit.h src/Folder.h \
                    src/globals.h src/Graph.h src/Graph3D.h  src/Grid.h \
                    src/ImageMarker.h src/Integration.h src/Interpolation.h \
                    src/future/lib/Interval.h src/future/lib/IntervalAttribute.h \
                    src/Legend.h \
                    src/Matrix.h src/future/matrix/MatrixView.h \
                    src/MultiPeakFit.h src/MultiLayer.h src/MyWidget.h \
                    src/Note.h src/NonLinearFit.h \
                    src/PlotEnrichement.h  src/PolynomialFit.h \
                    src/PythonScripting.h src/PythonScript.h \
                    src/Qt.h src/QtEnums.h src/QwtErrorPlotCurve.h src/QwtSymbol.h \
                    src/SmoothFilter.h src/Script.h src/ScriptingEnv.h \
                    src/Table.h src/future/table/TableView.h 


classdesc.input = CLASSDESC_HEADERS
  classdesc.output = $${TMP_DIR}/${QMAKE_FILE_BASE}.cd
  classdesc.commands = classdesc -qt -respect_private -typeName -use_mbr_pointers -nodef -onbase -overload -i ${QMAKE_FILE_NAME} python >${QMAKE_FILE_OUT}
  classdesc.depends = python.pri
  QMAKE_EXTRA_COMPILERS += classdesc

  PYTHONBIN = $$(PYTHON)
  isEmpty( PYTHONBIN ) {
    PYTHONBIN = python
  }
#  PYTHON_VERSION=$$system($${PYTHONBIN} --version|cut -f2 -d' '|cut -f1 -d.)


  LIBS+=-lboost_python-p2_7

  unix {
    INCLUDEPATH += $$system($$PYTHONBIN-config --includes|sed -e 's/-I//g')
    osx_dist {
      DEFINES += PYTHONHOME=/Applications/scidavis.app/Contents/Resources
    } 
  }

