.PHONY = all headers lib clean asml libs

OO_SDK_HOME=/usr/lib/openoffice.org/basis3.1/sdk
OFFICE_HOME=/usr/lib/openoffice.org
OFFICE_BASE_HOME=/usr/lib/openoffice.org/basis3.1
OO_SDK_URE_HOME=/usr/lib/openoffice.org/ure

OO_SDK_URE_BIN_DIR=$(OO_SDK_URE_HOME)/bin
OO_SDK_URE_LIB_DIR=$(OO_SDK_URE_HOME)/lib
OO_SDK_URE_JAVA_DIR=$(OO_SDK_URE_HOME)/share/java

OOoSDK_PATH = $(OO_SDK_HOME)/bin

ASMLDIR = ../asml

SET_LD = LD_LIBRARY_PATH=$(OO_SDK_URE_LIB_DIR)

REGCOMP = $(OO_SDK_URE_BIN_DIR)/regcomp
REGMERGE = $(OO_SDK_URE_BIN_DIR)/regmerge
IDLC = $(OOoSDK_PATH)/idlc
CPPUMAKER = $(OOoSDK_PATH)/cppumaker

OBJFILES = ASIMaths_impl.o conversion.o ooutils.o
DEPENDS = $(OBJFILES:.o=.d)

CPPFLAGS = -DUNX -DGCC -DLINUX -DCPPU_ENV=gcc3 -I. -I$(OO_SDK_HOME)/include -I$(ASMLDIR) -Wall
# -D_GLIBCXX_DEBUG
CXXFLAGS = -gdwarf-2

LIB_DIR = -L$(OO_SDK_HOME)/lib -L$(OO_SDK_URE_LIB_DIR) -L$(ASMLDIR)
LDLIBS = -luno_cppuhelpergcc3 -lasml -lgsl -lgslcblas -lm

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

all: ASIMaths.oxt

ASIMaths.oxt: libs
	-mkdir oxt/Linux_x86
	cp ASIMaths.rdb oxt/Linux_x86
	cp libASIMaths.so oxt/Linux_x86
	cd oxt && zip ../ASIMaths.oxt -r .

libs: ASIMaths.rdb libASIMaths.so
	LD_LIBRARY_PATH=. $(REGCOMP) -register -r ASIMaths.rdb -c libASIMaths.so

asml:
	$(MAKE) -C ../asml $(MAKECMDGOALS)

headers: ASIMaths.rdb
	$(CPPUMAKER) -BUCR $(FORCPPUMAKER) $(OO_SDK_URE_HOME)/share/misc/types.rdb $(OFFICE_BASE_HOME)/program/offapi.rdb $<

%.urd: %.idl
	$(IDLC) -w -C -I$(OO_SDK_HOME)/idl $<

%.rdb: %.urd
	$(REGMERGE) $@ /UCR $<

libASIMaths.so: asml $(OBJFILES)
	g++ $(CXXFLAGS) -o $@ -Wl,--version-script,component.uno.map -shared $(LIB_DIR) -L$(ASMLDIR) $(OBJFILES) $(LDLIBS)
	chcon -t textrel_shlib_t libASIMaths.so

dummy: dummy.o $(OBJFILES)
	g++ -o $@ $^ $(LIB_DIR) $(LDLIBS) -luno_sal -luno_salhelpergcc3 -luno_cppu -lstlport_gcc
	$(SET_LD) ./$@

clean: asml
	-rm -r dummy *.o *.so *.urd *.rdb *.d *.oxt
	-rm -r com org oxt/Linux_x86

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
