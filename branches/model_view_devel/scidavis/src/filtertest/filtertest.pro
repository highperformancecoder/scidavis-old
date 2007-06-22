TEMPLATE = app
TARGET = 
DEPENDPATH += . ..
INCLUDEPATH += . ..
CONFIG += debug
#LIBS         += -L /usr/lib -lgsl -lgslcblas
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
			  CopyThroughFilter.h \
			  StatisticsFilter.h \
			  AbstractSimpleFilter.h \
			  TruncationFilter.h \
			  DifferentiationFilter.h \
			  ReadOnlyTableModel.h \
			  TableModel.h \
			  DoubleTransposeFilter.h \
			  String2DoubleFilter.h \
			  Double2StringFilter.h \
			  DateTime2StringFilter.h \
			  String2DateTimeFilter.h \
			  Double2DateTimeFilter.h \

SOURCES += StringColumnData.cpp \
			  DoubleColumnData.cpp \
			  DateTimeColumnData.cpp \
			  AbstractFilter.cpp \
			  StatisticsFilter.cpp \
			  ReadOnlyTableModel.cpp \
			  String2DateTimeFilter.cpp \
			  TableModel.cpp \
			  main.cpp \

