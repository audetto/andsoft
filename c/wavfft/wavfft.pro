TEMPLATE = app
CONFIG 	 = warn_on release
HEADERS  = stuff.hpp index.hpp wavelet.hpp encoder.hpp
SOURCES  = wavfft.cpp stuff.cpp encoder.cpp
TARGET   = wavfft
LIBS	 = -lfftw3f -lpopt
