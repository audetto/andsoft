TEMPLATE = app
CONFIG 	 = qt warn_on thread release
HEADERS  = divx.h divx_asi.h screen.h dec.h enc.h
SOURCES  = divx.cpp screen.cpp dec.cpp enc.cpp
TARGET   = divx
LIBS	+= -L. -ldivxencore -ldivxdecore -lccvt
