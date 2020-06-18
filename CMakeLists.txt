cmake_minimum_required( VERSION 3.16 )

project( scidavis
  VERSION 2.3.0
  DESCRIPTION "SciDAVis is a free application for Scientific Data Analysis and Visualization."
  HOMEPAGE_URL "https://scidavis.sourceforge.net"
  LANGUAGES CXX C )

set( CMAKE_CXX_STANDARD 17)
set( CMAKE_CXX_STANDARD_REQUIRED TRUE )
set( CMAKE_CXX_EXTENSIONS OFF )

set( CMAKE_C_STANDARD 11)
set( CMAKE_C_STANDARD_REQUIRED TRUE )
set( CMAKE_C_EXTENSIONS OFF )

find_package( Qt5
  COMPONENTS
  Core
  Gui
  Widgets
  Svg
  PrintSupport
  Xml
  OpenGL
  LinguistTools
  REQUIRED
  )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTOUIC ON )
set( CMAKE_AUTORCC ON )

if( MSVC )
  # /wd4456 /wd4457 /wd4458 Silent "declaration of %1 hides %2 ..."
  # /wd4251 Silent dll-related warnings
  # /wd4127 Silent conditional expression is constant (Qt headers)
  # /wd4310 Silent cast truncates constant value (muParser headers)
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /W4 /permissive- \
       /wd4456 /wd4457 /wd4458 /wd4251 /wd4127 /wd4310" )
  set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4 /permissive-" )
  if( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-nonportable-include-path" )
  else()
    add_compile_options( "/MP" )
  endif()
else()
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -Wall -Wextra -pedantic" )
  set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic" )
endif()

# Searching for updates
option( SEARCH_FOR_UPDATES "Enable Searching for updates" ON )

# Download links
option( DOWNLOAD_LINKS "Enable Download links" ON )

if( SEARCH_FOR_UPDATES OR DOWNLOAD_LINKS )
  find_package( Qt5 COMPONENTS Network REQUIRED )
endif()

option( SCRIPTING_MUPARSER "Enable muParser Scripting" ON )

option( ORIGIN_IMPORT "Enable importing OriginLab project files" OFF )

# GSL
find_package( GSL REQUIRED )

# ZLIB
find_package( ZLIB "1.2.11" REQUIRED )

# OpenGL
find_package( OpenGL COMPONENTS OpenGL REQUIRED )

# muParser
find_library( MUPARSER_LIB muparser )
if( NOT MUPARSER_LIB )
  message( SEND_ERROR "muParser library not found" )
else()
  message( STATUS "Found muParser : ${MUPARSER_LIB}" )
endif()

add_subdirectory( 3rdparty )

add_subdirectory( libscidavis )

add_subdirectory( scidavis )

add_subdirectory( fitPlugins )

# Documentation
set( DOC_FILES
  scidavis-logo.png
  scidavis.css
  ChangeLog.md
  README.md
  INSTALL.md
  gpl.txt
  license.rtf
  )

if( WIN32 )
  install( FILES ${DOC_FILES} DESTINATION . )
  install( FILES manual/index.html DESTINATION ./manual )
else()
  install( FILES ${DOC_FILES} DESTINATION share/doc/scidavis )
  install( FILES manual/index.html DESTINATION share/doc/scidavis/manual )
endif()
