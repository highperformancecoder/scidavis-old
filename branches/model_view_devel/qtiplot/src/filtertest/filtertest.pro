TEMPLATE = app
TARGET = 
DEPENDPATH += . ..
INCLUDEPATH += . ..
CONFIG += debug
LIBS += -lgsl

HEADERS += AbstractColumnData.h \
           DateColumnData.h \
           DoubleColumnData.h \
           StringColumnData.h \
           TimeColumnData.h \
			  AbstractDataSource.h \
			  AbstractDateDataSource.h \
			  AbstractDoubleDataSource.h \
			  AbstractStringDataSource.h \
			  AbstractTimeDataSource.h \
			  AbstractFilter.h \
			  StatisticsFilter.h \
			  AbstractDoubleSimpleFilter.h \
			  TruncatedDoubleDataSource.h \
			  DifferentiationFilter.h \

SOURCES += StringColumnData.cpp \
			  DoubleColumnData.cpp \
			  TimeColumnData.cpp \
			  DateColumnData.cpp \
			  AbstractFilter.cpp \
			  StatisticsFilter.cpp \
			  main.cpp \

