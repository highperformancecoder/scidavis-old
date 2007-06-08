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
			  AbstractDateTimeDataSource.h \
			  AbstractDoubleDataSource.h \
			  AbstractStringDataSource.h \
			  AbstractFilter.h \
			  StatisticsFilter.h \
			  AbstractDoubleSimpleFilter.h \
			  TruncatedDoubleDataSource.h \
			  DifferentiationFilter.h \
#			  ReadOnlyTableModel.h \
#			  TableModel.h \
			  DoubleTransposeFilter.h \

SOURCES += StringColumnData.cpp \
			  DoubleColumnData.cpp \
			  DateTimeColumnData.cpp \
			  AbstractFilter.cpp \
			  StatisticsFilter.cpp \
#			  TableModel.cpp \
			  main.cpp \

