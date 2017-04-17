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
versionconfig.input = config.h.in
versionconfig.output = config.h
QMAKE_SUBSTITUTES += versionconfig

# following define required to prevent the catastrophic logging when
# large files are imported
DEFINES += NO_CODE_GENERATION_FOR_LOG
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
