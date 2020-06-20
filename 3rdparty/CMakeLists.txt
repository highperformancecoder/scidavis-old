
set( CMAKE_AUTOMOC OFF )
set( CMAKE_AUTOUIC OFF )
set( CMAKE_AUTORCC OFF )

# minigzip: mini gzip implementation
add_library( minigzip "minigzip/minigzip.c" )
target_link_libraries( minigzip ZLIB::ZLIB )

# Qwt5
option( BUILD_QWT "Build Qwt using own code" OFF )

if( NOT BUILD_QWT )
  find_library( QWT_LIB NAMES qwt5-qt5 qwt5-qt55 )
endif()

if( NOT BUILD_QWT AND QWT_LIB )
  message( STATUS "Found Qwt5: ${QWT_LIB}" )
  find_path( QWT_INCLUDE "qwt_global.h" )
else()
  # Build Qwt5
  message( STATUS "Qwt5: using own code" )
  file( GLOB HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "qwt5-qt5/src/*.h" )
  file( GLOB SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "qwt5-qt5/src/*.cpp" )
  add_library( qwt5-qt5 ${SRCS} ${HEADERS} )
  target_link_libraries( qwt5-qt5 Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg )
  set_target_properties( qwt5-qt5 PROPERTIES AUTOMOC ON )
endif()

# QwtPlot3D
option( BUILD_QWTPLOT3D "Build QwtPlot3D using own code" OFF )

if( NOT BUILD_QWTPLOT3D )
  find_library( QWTPLOT3D_LIB NAMES qwtplot3d qwtplot3d-qt5 )
endif()

if( NOT BUILD_QWTPLOT3D AND QWTPLOT3D_LIB )
  message( STATUS "Found QwtPlot3D: ${QWTPLOT3D_LIB}" )
  find_path( QWTPLOT3D_INCLUDE "qwt3d_global.h" )
else()
  # Build QwtPlot3D
  message( STATUS "QwtPlot3D: using own code" )
  file( GLOB HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "qwtplot3d/include/*.h" )
  list( APPEND HEADERS "qwtplot3d/3rdparty/gl2ps/gl2ps.h" )
  file( GLOB SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "qwtplot3d/src/*.cpp" )
  list( APPEND SRCS "qwtplot3d/3rdparty/gl2ps/gl2ps.c" )
  add_library( qwtplot3d ${SRCS} ${HEADERS} )
  target_link_libraries( qwtplot3d Qt5::Core Qt5::Gui Qt5::OpenGL OpenGL::GLU )
  set_target_properties( qwtplot3d PROPERTIES AUTOMOC ON )
  target_include_directories( qwtplot3d PUBLIC "qwtplot3d/include" )
endif()

# liborigin
if( ORIGIN_IMPORT )
  find_package( PkgConfig REQUIRED)
  pkg_search_module( ORIGIN_LIB liborigin )
  if( NOT ORIGIN_LIB_FOUND )
    message( STATUS "liborigin: using own code" )
    set( SRCS
      "liborigin/OriginFile.cpp"
      "liborigin/OriginParser.cpp"
      "liborigin/OriginAnyParser.cpp"
      )
    add_library( origin ${SRCS} )
  endif()
endif()
