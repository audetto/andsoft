.PHONY = all lib clean headers

OBJFILES = utils.o projection.o linear.o heston.o implied.o numerics.o distribution.o fourierpricing.o fastexp.o hypermatrix.o generator.o brownianmotionwithdrift.o pde.o
DEPENDS = $(OBJFILES:.o=.d)

## It seems QuantLib has been compiled without D_GLIBCXX_DEBUG
CPPFLAGS = -Wall -I.
##  -D_GLIBCXX_DEBUG
CXXFLAGS = -gdwarf-2

LDLIBS = -lgsl -lgslcblas -lm -lQuantLib

AR = ar
RANLIB = ranlib

all: headers libasml.a

headers:
	$(MAKE) -C asi $(MAKECMDGOALS)

libasml.a: $(OBJFILES)
	$(AR) cr $@ $^
	$(RANLIB) $@

dummy: all dummy.o
	g++ $(CPPFLAGS) $(CXXFLAGS) -o $@ dummy.o -L. -lasml $(LDLIBS)

clean: headers
	rm -rf dummy *.o *.a *.d

#### dependencies

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CXX) -MM -MP $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

ifneq ($(MAKECMDGOALS),clean)
include $(DEPENDS)
endif

