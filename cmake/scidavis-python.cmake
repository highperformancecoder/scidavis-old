# File                 : scidavis-python.cmake
# Project              : SciDAVis
# --------------------------------------------------------------------
# Copyright            : (C) 2020 by Miquel Garriga
# Email (use @ for *)  : gbmiquel*gmail.com
# Description          : Module for determining Python related configuration variables

#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor,
#  Boston, MA  02110-1301  USA

# based on:
#  https://github.com/PLplot/PLplot/blob/master/cmake/modules/qt.cmake
#  https://github.com/PLplot/PLplot/blob/master/bindings/qt_gui/pyqt5/CMakeLists.txt
#  http://cmake.3232098.n2.nabble.com/cmake-PyQT-SIP-td5789749.html

if( ENABLE_Python3 )

  find_program(SIP_EXECUTABLE sip REQUIRED)
  message(STATUS "SIP_EXECUTABLE = ${SIP_EXECUTABLE}")
  if(SIP_EXECUTABLE)
    execute_process(
      COMMAND ${SIP_EXECUTABLE} -V
      OUTPUT_VARIABLE SIP_VERSION
      RESULT_VARIABLE SIP_VERSION_ERR
      OUTPUT_STRIP_TRAILING_WHITESPACE
      )
    if(SIP_VERSION_ERR)
      message(AUTHOR_WARNING "sip -V command could not determine sip version")
    else(SIP_VERSION_ERR)
      message(STATUS "SIP_VERSION = ${SIP_VERSION}")
    endif(SIP_VERSION_ERR)

    # Look in standard places for sip files.
    execute_process(
      COMMAND ${Python3_EXECUTABLE} -c "import sys; sys.stdout.write(sys.prefix)"
      OUTPUT_VARIABLE SYS_PREFIX
      RESULT_VARIABLE SYS_PREFIX_ERR
      OUTPUT_STRIP_TRAILING_WHITESPACE
      )

    if(NOT SYS_PREFIX_ERR)
      set(sip_dir_HINTS)
      list(APPEND sip_dir_HINTS "${SYS_PREFIX}/share/python3-sip")
      list(APPEND sip_dir_HINTS "${SYS_PREFIX}/share/sip")
      list(APPEND sip_dir_HINTS "${Python3_SITELIB}")
      set(pyqt_module_name PyQt5)
      set(pyqt_NAMES Py2-Qt5)
      list(APPEND pyqt_NAMES ${pyqt_module_name})
      list(APPEND pyqt_NAMES "PyQt5/bindings")

      find_path( PYQT_SIP_DIR
        NAMES "QtCore/QtCoremod.sip"
        HINTS ${sip_dir_HINTS}
        PATH_SUFFIXES ${pyqt_NAMES}
        REQUIRED
        )
      message(STATUS "PYQT_SIP_DIR = ${PYQT_SIP_DIR}")

      if(PYQT_SIP_DIR)
        execute_process(
          COMMAND ${Python3_EXECUTABLE} -c "from ${pyqt_module_name}.QtCore import PYQT_CONFIGURATION;import sys;sys.stdout.write(PYQT_CONFIGURATION['sip_flags'].replace(' ',';'))"
          OUTPUT_VARIABLE PYQT_SIP_FLAGS
          RESULT_VARIABLE PYQT_SIP_FLAGS_ERR
          OUTPUT_STRIP_TRAILING_WHITESPACE
          )
        if(NOT PYQT_SIP_FLAGS_ERR)
          # Must change from blank-delimited string to CMake list so that sip
          # COMMAND will work properly with these flags later on.
          # string(REGEX REPLACE " " ";" PYQT_SIP_FLAGS "${PYQT_SIP_FLAGS}")
          message(STATUS "${pyqt_module_name}: PYQT_SIP_FLAGS = ${PYQT_SIP_FLAGS}")
        endif(NOT PYQT_SIP_FLAGS_ERR)

      endif(PYQT_SIP_DIR)
    endif(NOT SYS_PREFIX_ERR)
  endif(SIP_EXECUTABLE)


  set(scidavis_pyqt5_HDR ${CMAKE_CURRENT_BINARY_DIR}/sipAPIscidavis.h)

  set_source_files_properties(
    ${scidavis_pyqt5_HDR}
    PROPERTIES
    GENERATED ON
    )

  set(scidavis_pyqt5_SRC
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidaviscmodule.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisApplicationWindow.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisGraph.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisArrowMarker.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisImageMarker.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisLegend.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisMultiLayer.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisTable.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisMatrix.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisMyWidget.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisScriptEdit.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisNote.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisPythonScript.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisPythonScripting.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisFolder.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisFit.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisExponentialFit.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisTwoExpFit.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisThreeExpFit.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisSigmoidalFit.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisGaussAmpFit.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisLorentzFit.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisNonLinearFit.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisPluginFit.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisMultiPeakFit.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisPolynomialFit.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisLinearFit.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisGaussFit.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisFilter.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisDifferentiation.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisIntegration.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisInterpolation.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisSmoothFilter.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisFFTFilter.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisFFT.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisCorrelation.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisConvolution.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisDeconvolution.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisAbstractAspect.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisColumn.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisQwtSymbol.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisQwtPlotCurve.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisQwtPlot.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisGrid.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisQList0100QDateTime.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisQList0101Folder.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisQList0101MyWidget.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipscidavisQList0101QwtPlotCurve.cpp
    )

  set_source_files_properties(
    ${scidavis_pyqt5_SRC}
    PROPERTIES
    GENERATED ON
    )

  add_custom_command(
    OUTPUT ${scidavis_pyqt5_HDR} ${scidavis_pyqt5_SRC}
    COMMAND ${SIP_EXECUTABLE} -c . -b scidavis_pyqt5.sbf -I${PYQT_SIP_DIR} ${PYQT_SIP_FLAGS} ${CMAKE_CURRENT_SOURCE_DIR}/src/scidavis.sip
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/scidavis.sip
    )
  add_custom_target(generate_pyqt5_source
    DEPENDS ${scidavis_pyqt5_HDR} ${scidavis_pyqt5_SRC}
    )

  add_dependencies(libscidavis generate_pyqt5_source)

  target_sources( libscidavis PRIVATE ${scidavis_pyqt5_SRC} ${scidavis_pyqt5_HDR} )

else( ENABLE_Python3 )
  message( STATUS "Python3 support not enabled")
endif( ENABLE_Python3 )
