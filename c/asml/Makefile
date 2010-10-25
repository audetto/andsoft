.PHONY = all lib clean headers

OBJFILES = asml/utils/utils.o asml/linearalgebra/projection.o asml/linearalgebra/linear.o asml/distribution/heston.o asml/analytics/implied.o asml/analytics/numerics.o asml/distribution/distribution.o asml/distribution/fourierpricing.o asml/linearalgebra/fastexp.o asml/linearalgebra/hypermatrix.o asml/pde/generator.o asml/process/brownianmotionwithdrift.o asml/pde/pde.o
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

dummy: all asml/dummy.o
	g++ $(CPPFLAGS) $(CXXFLAGS) -o $@ asml/dummy.o -L. -lasml $(LDLIBS)

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
