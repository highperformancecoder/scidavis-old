
set( CMAKE_AUTOMOC OFF )
set( CMAKE_AUTOUIC OFF )
set( CMAKE_AUTORCC OFF )

add_subdirectory( exp_saturation )

add_subdirectory( explin )

add_subdirectory( fitRational0 )

add_subdirectory( fitRational1 )

add_subdirectory( planck_wavelength )

if( WIN32 )
  install( TARGETS exp_saturation explin fitRational0 fitRational1 planck_wavelength
    RUNTIME DESTINATION fitPlugins
    )
else()
  install( TARGETS exp_saturation explin fitRational0 fitRational1 planck_wavelength
    LIBRARY DESTINATION lib/scidavis/plugins
    )
endif()
