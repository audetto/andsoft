SOURCES = mersenne.cpp normal.cpp foo.cpp timer.cpp montecarlo.cpp matrix.cpp utils.cpp random.cpp basket.cpp
HEADERS = normal.h mersenne.h cycle.h timer.h montecarlo.h utils.h matrix.h random.h basket.h
CONFIG = warn_on debug
LIBS += -lgsl -lgslcblas -L/usr/local/lib -lstlport_gcc_stldebug
INCLUDEPATH = /usr/local/include/stlport /usr/local/include/boost-1_31
DEFINES = _STLP_DEBUG
