# this file is not part of the liborigin library, which uses the cmake
# system, but must be maintained separately of liborigin

include(../../config.pri)

TEMPLATE = lib
CONFIG += staticlib
TARGET = origin
QMAKE_CLEAN+=${TARGET}

LIBORIGIN_VERSION_MAJOR = 3
LIBORIGIN_VERSION_MINOR = 0
LIBORIGIN_VERSION_BUGFIX = 0

# logging of parse process is disabled by default
# to enable it uncomment following line
# DEFINES += GENERATE_CODE_FOR_LOG
QMAKE_CXXFLAGS += -std=c++11

HEADERS  += \
        config.h \
	OriginObj.h\
	OriginFile.h\
	OriginParser.h\
	tree.hh

SOURCES += \
	OriginFile.cpp\
	OriginParser.cpp\
	OriginAnyParser.cpp

# for converage testing
gcov {
   QMAKE_CXXFLAGS+=-fprofile-arcs -ftest-coverage
}
        
