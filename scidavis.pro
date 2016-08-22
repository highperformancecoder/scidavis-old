#################################################
# SciDAVis main project file
# run "qmake scidavis.pro" to generate the Makefile
# In most cases you don't have to make changes to
# this file but to the "config.pri" file 
#################################################

# enable C++11 support
greaterThan(QT_MAJOR_VERSION, 4){
  CONFIG += c++11
} else {
  QMAKE_CXXFLAGS += -std=c++0x
}

TEMPLATE = subdirs

# configurations that automatically turn on liborigin - mainly the
# binary releases, as well as the AEGIS build
mxe|osx_dist {CONFIG += liborigin}
aegis {CONFIG += liborigin test}

# liborigin must come first, if it exists
liborigin {SUBDIRS = 3rdparty/liborigin}

SUBDIRS += libscidavis scidavis 

test {SUBDIRS += test}

CONFIG+=ordered

!mxe {
# optional fit plugins
# mxe is a static build, so these are included elsewhere
SUBDIRS += fitPlugins/fitRational0\
           fitPlugins/fitRational1\
           fitPlugins/explin\
           fitPlugins/exp_saturation\
           fitPlugins/planck_wavelength\
}

TRANSLATIONS    = scidavis/translations/scidavis_de.ts \
                  scidavis/translations/scidavis_es.ts \
                  scidavis/translations/scidavis_nl.ts \
                  scidavis/translations/scidavis_fr.ts \
                  scidavis/translations/scidavis_ru.ts \
                  scidavis/translations/scidavis_ja.ts \
                  scidavis/translations/scidavis_sv.ts \
                  scidavis/translations/scidavis_pt-br.ts \
                  scidavis/translations/scidavis_cs.ts \
                  scidavis/translations/scidavis_cs-alt.ts \
