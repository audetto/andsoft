.PHONY = all lib clean headers

OBJFILES = asml/utils.o asml/projection.o asml/linear.o asml/heston.o asml/implied.o asml/numerics.o asml/distribution.o asml/fourierpricing.o asml/fastexp.o asml/hypermatrix.o asml/generator.o asml/brownianmotionwithdrift.o asml/pde.o
DEPENDS = $(OBJFILES:.o=.d)

## It seems QuantLib has been compiled without D_GLIBCXX_DEBUG
CPPFLAGS = -Wall -I.
##  -D_GLIBCXX_DEBUG
CXXFLAGS = -gdwarf-2

LDLIBS = -lgsl -lgslcblas -lm -lQuantLib

AR = ar
RANLIB = ranlib

all: libasml.a

libasml.a: $(OBJFILES)
	$(AR) cr $@ $^
	$(RANLIB) $@

dummy: all dummy.o
	g++ $(CPPFLAGS) $(CXXFLAGS) -o $@ dummy.o -L. -lasml $(LDLIBS)

clean:
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
