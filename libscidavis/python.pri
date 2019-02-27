##################### PYTHON + classdesc + boost #####################

  INSTALLS += pythonconfig
  pythonconfig.files += scidavisrc.py
  DEFINES       += PYTHON_CONFIG_PATH="\\\"$$replace(pythonconfig.path," ","\\ ")\\\""

  INSTALLS += pythonutils
  pythonutils.files += scidavisUtil.py

  DEFINES       += PYTHON_UTIL_PATH="\\\"$$replace(pythonutils.path," ","\\ ")\\\""

  TMP_DIR = ../tmp/scidavis

  DEFINES += SCRIPTING_PYTHON
  HEADERS += src/PythonScript.h src/PythonScripting.h
  SOURCES += src/PythonScript.cpp src/PythonScripting.cpp

  CLASSDESC_HEADERS = src/ApplicationWindow.h src/Script.h src/Folder.h \
                    src/Matrix.h src/Note.h src/MyWidget.h src/QtEnums.h \
                    src/Graph.h src/PythonScripting.h src/PythonScript.h \
                    src/ArrowMarker.h src/Table.h src/future/table/TableView.h \
                    src/MultiLayer.h src/QwtSymbol.h \
                    src/future/lib/Interval.h src/future/lib/IntervalAttribute.h
  classdesc.input = CLASSDESC_HEADERS
  classdesc.output = $${TMP_DIR}/${QMAKE_FILE_BASE}.cd
  classdesc.commands = classdesc -qt -respect_private -typeName -use_mbr_pointers -nodef -onbase -i ${QMAKE_FILE_NAME} python >${QMAKE_FILE_OUT}
  classdesc.depends = python.pri
  QMAKE_EXTRA_COMPILERS += classdesc

  PYTHONBIN = $$(PYTHON)
  isEmpty( PYTHONBIN ) {
    PYTHONBIN = python
  }
  PYTHON_VERSION=$$system($${PYTHONBIN} --version|cut -f2 -d' '|cut -f1 -d.)

  LIBS+=-lboost_python-p2_7

  unix {
    INCLUDEPATH += $$system($$PYTHONBIN-config --includes|sed -e 's/-I//g')
    osx_dist {
      DEFINES += PYTHONHOME=/Applications/scidavis.app/Contents/Resources
    } 
  }




##################### SIP generated files #####################

#  HEADERS += $${SIP_DIR}/sipAPIscidavis.h
#
#  SOURCES += $${SIP_DIR}/sipscidaviscmodule.cpp\
#             $${SIP_DIR}/sipscidavisApplicationWindow.cpp\
#             $${SIP_DIR}/sipscidavisGraph.cpp\
#             $${SIP_DIR}/sipscidavisArrowMarker.cpp\
#             $${SIP_DIR}/sipscidavisImageMarker.cpp\
#             $${SIP_DIR}/sipscidavisLegend.cpp\
#             $${SIP_DIR}/sipscidavisMultiLayer.cpp\
#             $${SIP_DIR}/sipscidavisTable.cpp\
#             $${SIP_DIR}/sipscidavisMatrix.cpp\
#             $${SIP_DIR}/sipscidavisMyWidget.cpp\
#             $${SIP_DIR}/sipscidavisScriptEdit.cpp\
#             $${SIP_DIR}/sipscidavisNote.cpp\
#             $${SIP_DIR}/sipscidavisPythonScript.cpp\
#             $${SIP_DIR}/sipscidavisPythonScripting.cpp\
#             $${SIP_DIR}/sipscidavisFolder.cpp\
#             $${SIP_DIR}/sipscidavisFit.cpp \
#             $${SIP_DIR}/sipscidavisExponentialFit.cpp \
#             $${SIP_DIR}/sipscidavisTwoExpFit.cpp \
#             $${SIP_DIR}/sipscidavisThreeExpFit.cpp \
#             $${SIP_DIR}/sipscidavisSigmoidalFit.cpp \
#             $${SIP_DIR}/sipscidavisGaussAmpFit.cpp \
#             $${SIP_DIR}/sipscidavisLorentzFit.cpp \
#             $${SIP_DIR}/sipscidavisNonLinearFit.cpp \
#             $${SIP_DIR}/sipscidavisPluginFit.cpp \
#             $${SIP_DIR}/sipscidavisMultiPeakFit.cpp \
#             $${SIP_DIR}/sipscidavisPolynomialFit.cpp \
#             $${SIP_DIR}/sipscidavisLinearFit.cpp \
#             $${SIP_DIR}/sipscidavisGaussFit.cpp \
#             $${SIP_DIR}/sipscidavisFilter.cpp \
#             $${SIP_DIR}/sipscidavisDifferentiation.cpp \
#             $${SIP_DIR}/sipscidavisIntegration.cpp \
#             $${SIP_DIR}/sipscidavisInterpolation.cpp \
#             $${SIP_DIR}/sipscidavisSmoothFilter.cpp \
#             $${SIP_DIR}/sipscidavisFFTFilter.cpp \
#             $${SIP_DIR}/sipscidavisFFT.cpp \
#             $${SIP_DIR}/sipscidavisCorrelation.cpp \
#             $${SIP_DIR}/sipscidavisConvolution.cpp \
#             $${SIP_DIR}/sipscidavisDeconvolution.cpp \
#             $${SIP_DIR}/sipscidavisAbstractAspect.cpp \
#             $${SIP_DIR}/sipscidavisColumn.cpp \
#				 $${SIP_DIR}/sipscidavisQwtSymbol.cpp \
#				 $${SIP_DIR}/sipscidavisQwtPlotCurve.cpp \
#             $${SIP_DIR}/sipscidavisQwtPlot.cpp \
#				 $${SIP_DIR}/sipscidavisGrid.cpp \

#exists($${SIP_DIR}/sipscidavisQList.cpp) {
## SIP < 4.9
#	SOURCES += $${SIP_DIR}/sipscidavisQList.cpp
#} else {
#	SOURCES += \
#			   $${SIP_DIR}/sipscidavisQList0100QDateTime.cpp\
#			   $${SIP_DIR}/sipscidavisQList0101Folder.cpp\
#			   $${SIP_DIR}/sipscidavisQList0101MyWidget.cpp\
#			   $${SIP_DIR}/sipscidavisQList0101QwtPlotCurve.cpp\
#}

###############################################################
