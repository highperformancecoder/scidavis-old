#QMAKE_CXX = distcc
QMAKE_CXXFLAGS += -gstabs+
QMAKE_LFLAGS += -g
TEMPLATE = app
TARGET = 
DEPENDPATH += . ..
INCLUDEPATH += . ..
CONFIG += debug

# Input
HEADERS += AbstractColumnData.h \
           DateTimeColumnData.h \
           DoubleColumnData.h \
           StringColumnData.h \
		   AbstractDataSource.h \
		   AbstractFilter.h \
		   AbstractDateTimeDataSource.h \
		   AbstractDoubleDataSource.h \
		   AbstractStringDataSource.h \
           TableModel.h \
           TableView.h \
           CopyThroughFilter.h \
           Double2StringFilter.h \
           String2DoubleFilter.h \
           DateTime2StringFilter.h \
           String2DateTimeFilter.h \
           String2MonthFilter.h \
           String2DayOfWeekFilter.h \
		   tablecommands.h \
		   TableItemDelegate.h \
		   testwrapper.h

SOURCES += main.cpp TableModel.cpp TableView.cpp 

SOURCES += StringColumnData.cpp \
           DateTimeColumnData.cpp \
		   DoubleColumnData.cpp \
		   AbstractFilter.cpp \
           String2DateTimeFilter.cpp \
		   tablecommands.cpp \
		   TableItemDelegate.cpp 

