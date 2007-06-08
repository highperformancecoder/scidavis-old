TEMPLATE = app
TARGET = 
DEPENDPATH += . ..
INCLUDEPATH += . ..
CONFIG += debug
LIBS         += -L /usr/lib -lgsl -lgslcblas
#LIBS += -lgsl

HEADERS += AbstractColumnData.h \
           DateTimeColumnData.h \
           DoubleColumnData.h \
           StringColumnData.h \
			  AbstractDataSource.h \
			  Interval.h \
			  AbstractDateTimeDataSource.h \
			  AbstractDoubleDataSource.h \
			  AbstractStringDataSource.h \
			  AbstractFilter.h \
			  StatisticsFilter.h \
			  AbstractSimpleFilter.h \
			  TruncationFilter.h \
			  DifferentiationFilter.h \
			  ReadOnlyTableModel.h \
#			  TableModel.h \
			  DoubleTransposeFilter.h \
			  String2DoubleFilter.h \
			  Double2StringFilter.h \

SOURCES += StringColumnData.cpp \
			  DoubleColumnData.cpp \
			  DateTimeColumnData.cpp \
			  AbstractFilter.cpp \
			  StatisticsFilter.cpp \
			  ReadOnlyTableModel.cpp \
#			  TableModel.cpp \
			  main.cpp \

