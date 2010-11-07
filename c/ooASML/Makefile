.PHONY : all clean asml libs install

OFFICE_HOME=/usr/lib/openoffice
OFFICE_BASE_HOME=$(OFFICE_HOME)/basis3.2
OO_SDK_HOME=$(OFFICE_HOME)/basis3.2/sdk
OO_SDK_URE_HOME=/usr/lib/ure

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

OBJFILES = ASIMaths_impl.o ASIMaths.o conversion.o ooutils.o Cache.o

CPPFLAGS = -DUNX -DGCC -DLINUX -DCPPU_ENV=gcc3 -I. -I$(OO_SDK_HOME)/include -I$(ASMLDIR) -Wall
# -D_GLIBCXX_DEBUG
CXXFLAGS = -g

LIB_DIR = -L$(OO_SDK_HOME)/lib -L$(OO_SDK_URE_LIB_DIR) -L$(ASMLDIR)
LDLIBS = -luno_cppuhelpergcc3 -lasml -lgsl -lgslcblas -lboost_regex -lm

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

install: ASIMaths.oxt
	-unopkg remove inc.andsoft.ASIMaths
	unopkg add ASIMaths.oxt

ASIMaths.oxt: ASIMaths.rdb libASIMaths.so
	-mkdir oxt/Linux_x86
# does this step change ASIMaths.rdb??? 
	LD_LIBRARY_PATH=. $(REGCOMP) -register -r ASIMaths.rdb -c libASIMaths.so
	cp libASIMaths.so oxt/Linux_x86
	cp ASIMaths.rdb oxt/Linux_x86
	cd oxt && zip ../ASIMaths.oxt -r .

asml:
	$(MAKE) -C $(ASMLDIR) $(MAKECMDGOALS)

$(ASMLDIR)/libasml.a:
	$(MAKE) -C $(ASMLDIR) libasml.a

%.urd: %.idl
	$(IDLC) -w -C -I$(OO_SDK_HOME)/idl $<

%.rdb: %.urd
	$(REGMERGE) $@ /UCR $<
	$(CPPUMAKER) -BUCR $(FORCPPUMAKER) $(OO_SDK_URE_HOME)/share/misc/types.rdb $(OFFICE_BASE_HOME)/program/offapi.rdb $@

libASIMaths.so: $(ASMLDIR)/libasml.a $(OBJFILES)
	g++ $(CXXFLAGS) -o $@ -Wl,--version-script,component.uno.map -shared $(LIB_DIR) -L$(ASMLDIR) $(OBJFILES) $(LDLIBS)
#	chcon -t textrel_shlib_t libASIMaths.so

dummy: dummy.o $(OBJFILES)
	g++ -o $@ $^ $(LIB_DIR) $(LDLIBS) -luno_sal -luno_salhelpergcc3 -luno_cppu -lstlport_gcc
	$(SET_LD) ./$@

clean: asml
	rm -f dummy
	rm -f $(OBJFILES)
	rm -f *.so *.urd *.rdb *.oxt
	rm -fr com org oxt/Linux_x86

ooutils.h: ASIMaths.idl

include ../build/depend.mk
