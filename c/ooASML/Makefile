.PHONY = all headers lib clean asml

OOoSDK = /usr/lib/openoffice.org/sdk
OOo = /usr/lib/openoffice.org/program
OOoPATH = $(OOoSDK)/linux/bin

ASMLDIR = ../asml

THISDIR := $(shell pwd)

SET_LD = LD_LIBRARY_PATH=$(OOo)

REGCOMP = $(SET_LD) $(OOoPATH)/regcomp
REGMERGE = $(SET_LD) $(OOoPATH)/regmerge
IDLC = $(SET_LD) $(OOoPATH)/idlc
CPPUMAKER = $(SET_LD) $(OOoPATH)/cppumaker

OBJFILES = ASIMaths_impl.o conversion.o ooutils.o
DEPENDS = $(OBJFILES:.o=.d)

CPPFLAGS = -DUNX -DGCC -DLINUX -DCPPU_ENV=gcc3 -I. -I$(OOoSDK)/include -I$(ASMLDIR) -Wall -D_GLIBCXX_DEBUG
CXXFLAGS = -gdwarf-2

LDLIBS = -lcppuhelpergcc3 -lasml -lgsl -lgslcblas -lm

FORCPPUMAKER = -Torg.asi.XMaths \
	-Tcom.sun.star.sheet.XAddIn \
	-Tcom.sun.star.sheet.XCompatibilityNames \
	-Tcom.sun.star.lang.XServiceName \
	-Tcom.sun.star.lang.XInitialization \
	-Tcom.sun.star.lang.XServiceInfo \
	-Tcom.sun.star.lang.XTypeProvider \
	-Tcom.sun.star.uno.XWeak \
	-Tcom.sun.star.uno.XAggregation \
	-Tcom.sun.star.lang.XMultiServiceFactory \
	-Tcom.sun.star.uno.XComponentContext \
	-Tcom.sun.star.lang.XSingleComponentFactory \
	-Tcom.sun.star.lang.XSingleServiceFactory \
	-Tcom.sun.star.lang.IllegalArgumentException \
	-Tcom.sun.star.registry.XRegistryKey \
	-Tcom.sun.star.reflection.XIdlReflection

all: asml ASIMaths.rdb libASIMaths.so
	$(REGCOMP) -register -r ASIMaths.rdb -c $(THISDIR)/libASIMaths.so

asml:
	$(MAKE) -C ../asml $(MAKECMDGOALS)

headers: ASIMaths.rdb
	$(CPPUMAKER) -BUCR $(FORCPPUMAKER) $(OOo)/types.rdb $<

%.urd: %.idl
	$(IDLC) -w -C -I$(OOoSDK)/idl $<

%.rdb: %.urd
	$(REGMERGE) $@ /UCR $<

libASIMaths.so: $(OBJFILES)
	g++ $(CXXFLAGS) -o $@ -Wl,--version-script,component.uno.map -shared -L$(OOo) -L$(ASMLDIR) $^ $(LDLIBS)

dummy: dummy.o $(OBJFILES)
	g++ -o $@ $^ -L$(OOo) -L$(ASMLDIR) $(LDLIBS) -luno_sal -luno_salhelpergcc3 -luno_cppu -lstlport_gcc

clean: asml
	rm -rf dummy com org *.o *.so *.urd *.rdb *.d

#### dependencies
ooutils.h: headers

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CXX) -MM -MP $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

ifneq ($(MAKECMDGOALS),clean)
include $(DEPENDS)
endif
