set( RESOURCES
  "appicons.qrc"
  "icons.qrc"
  )

if( WIN32 )
  list( APPEND RESOURCES "scidavis.rc" )
endif()

# Translation Files
file( COPY translations DESTINATION . )
set( TS_FILES
  "${CMAKE_CURRENT_BINARY_DIR}/translations/scidavis_de.ts"
  "${CMAKE_CURRENT_BINARY_DIR}/translations/scidavis_es.ts"
  "${CMAKE_CURRENT_BINARY_DIR}/translations/scidavis_fr.ts"
  "${CMAKE_CURRENT_BINARY_DIR}/translations/scidavis_ru.ts"
  "${CMAKE_CURRENT_BINARY_DIR}/translations/scidavis_ja.ts"
  "${CMAKE_CURRENT_BINARY_DIR}/translations/scidavis_sv.ts"
  "${CMAKE_CURRENT_BINARY_DIR}/translations/scidavis_pt-br.ts"
  "${CMAKE_CURRENT_BINARY_DIR}/translations/scidavis_cs.ts"
  "${CMAKE_CURRENT_BINARY_DIR}/translations/scidavis_cs-alt.ts"
  "${CMAKE_CURRENT_BINARY_DIR}/translations/scidavis_pl.ts"
  "${CMAKE_CURRENT_BINARY_DIR}/translations/scidavis_zh-cn.ts"
  "${CMAKE_CURRENT_BINARY_DIR}/translations/scidavis_zh-tw.ts"
  )
qt5_create_translation( QM_FILES "${CMAKE_SOURCE_DIR}/libscidavis" ${TS_FILES} )

add_executable( scidavis
  WIN32
  "src/main.cpp"
  "${RESOURCES}"
  "${QM_FILES}"
  )

target_link_libraries( scidavis libscidavis )

get_property( MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG )
if( MULTI_CONFIG )
  target_include_directories( scidavis PRIVATE
    "${CMAKE_BINARY_DIR}/libscidavis/libscidavis_autogen/include_$<CONFIG>"
    )
else()
  target_include_directories( scidavis PRIVATE
    "${CMAKE_BINARY_DIR}/libscidavis/libscidavis_autogen/include"
    )
endif()

if( WIN32 )
  install( TARGETS scidavis RUNTIME DESTINATION . )
  install( FILES ${QM_FILES} DESTINATION translations )
else()
  install( TARGETS scidavis RUNTIME DESTINATION bin )
  install( FILES ${QM_FILES}  DESTINATION share/scidavis/translations )
endif()

# Icons

if( UNIX )
  install( FILES icons/scidavis.svg DESTINATION share/icons/hicolor/scalable/apps )

  foreach( res IN ITEMS 16 22 32 48 64 128 )
    install( FILES icons/hicolor-${res}/scidavis.png
      DESTINATION share/icons/hicolor/${res}x${res}/apps )
  endforeach()

  foreach( res IN ITEMS 16 22 32 )
    install( FILES icons/locolor-${res}/scidavis.png
      DESTINATION share/icons/locolor/${res}x${res}/apps )
  endforeach()
endif()

if( WIN32 )
  install( FILES icons/scidavis.ico DESTINATION . )
endif()

# Application Files
if( UNIX )
  install( FILES scidavis.desktop DESTINATION share/applications )
  install( FILES scidavis.xml DESTINATION share/mime/packages )
  install( FILES scidavis.1 DESTINATION share/man/man1 )
  install( FILES scidavis.appdata.xml DESTINATION share/appdata )
  install( FILES x-sciprj.desktop DESTINATION share/mimelnk/application )
endif()
