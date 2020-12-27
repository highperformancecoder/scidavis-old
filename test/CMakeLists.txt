if( ENABLE_Testing )

  message( STATUS "Testing enabled" )

  set( CMAKE_AUTOMOC OFF )
  set( CMAKE_AUTOUIC OFF )
  set( CMAKE_AUTORCC OFF )

  set( SRCS
    "main.cpp"
    "applicationWindow.cpp"
    "readWriteProject.cpp"
    "fft.cpp"
    "testPaintDevice.cpp"
    "3dplot.cpp"
    "menus.cpp"
    "arrowMarker.cpp"
    )

  set( TestDataFiles
    "${CMAKE_CURRENT_SOURCE_DIR}/3dplot.sciprj"
    "${CMAKE_CURRENT_SOURCE_DIR}/3dplotTable.sciprj"
    "${CMAKE_CURRENT_SOURCE_DIR}/testProject.sciprj"
    "${CMAKE_CURRENT_SOURCE_DIR}/Histo.opj"
    "${CMAKE_CURRENT_SOURCE_DIR}/USstates.opj"
    )

  add_executable( unittest ${SRCS} )

  # Find UnitTest++ library and its headers
  find_library( UNITTEST_LIBRARY UnitTest++ REQUIRED )
  find_path ( UNITTEST_INCLUDE_DIR
    NAMES UnitTest++.h
    PATH_SUFFIXES UnitTest++
    REQUIRED
  )
  # Find Boost-regex library
  find_package(Boost REQUIRED COMPONENTS regex)

  target_link_libraries( unittest libscidavis ${UNITTEST_LIBRARY} Boost::regex pthread )

  target_include_directories( unittest PRIVATE
    "${CMAKE_BINARY_DIR}/libscidavis/libscidavis_autogen/include"
    ${UNITTEST_INCLUDE_DIR}
    )
  if( ORIGIN_IMPORT )
    target_compile_definitions( unittest PRIVATE ORIGIN_IMPORT )
  endif()

  add_custom_command(TARGET unittest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${TestDataFiles} ${CMAKE_CURRENT_BINARY_DIR}/
  )

else( ENABLE_Testing )

  message( STATUS "Testing not enabled" )

endif( ENABLE_Testing )
