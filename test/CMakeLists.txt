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

  add_test( NAME unittest COMMAND unittest )

  set( TestDataFiles
    "3dplot.sciprj"
    "3dplotTable.sciprj"
    "testProject.sciprj"
    "Histo.opj"
    "USstates.opj"
    )
  file( COPY ${TestDataFiles} DESTINATION . )

  if( NOT (SEARCH_FOR_UPDATES OR DOWNLOAD_LINKS) )

    set( menu_files defaultMenus_en.menudat appWithGraphen.menudat )

    foreach( menu ${menu_files} )
      get_filename_component( test_name ${menu} NAME_WLE )
      add_test( NAME menuData_${test_name} COMMAND diff -q ${menu} ${CMAKE_CURRENT_SOURCE_DIR}/menuData/${menu} )
      set_tests_properties( menuData_${test_name} PROPERTIES DEPENDS unittest )
    endforeach()

  endif()

  if( ENABLE_Python3 )

    file( COPY pythonTests/ DESTINATION ./tmp )
    file( COPY ../scidavis-logo.png DESTINATION ./tmp )

    file( GLOB pythonTests pythonTests/*.py )

    foreach( pythonTest ${pythonTests} )
      get_filename_component( test_name ${pythonTest} NAME_WLE )
      add_test( NAME python_${test_name}
        COMMAND scidavis -l=en -x ${pythonTest}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tmp
      )
    endforeach()

  endif()

else( ENABLE_Testing )

  message( STATUS "Testing not enabled" )

endif( ENABLE_Testing )
