.PHONY: all lib clean headers

OBJFILES += asml/utils/utils.o asml/utils/error.o
OBJFILES += asml/linearalgebra/projection.o asml/linearalgebra/linear.o  asml/linearalgebra/fastexp.o asml/linearalgebra/hypermatrix.o
OBJFILES += asml/analytics/implied.o asml/analytics/numerics.o 
OBJFILES += asml/distribution/distribution.o asml/distribution/fourierpricing.o  asml/distribution/heston.o
OBJFILES += asml/pde/generator.o asml/pde/pde.o
OBJFILES += asml/process/brownianmotionwithdrift.o
OBJFILES += asml/payoff/AmericanLookback.o asml/payoff/GenericPathOption.o asml/payoff/JSPayoff.o

DEPENDS = $(OBJFILES:.o=.d)

## It seems QuantLib has been compiled without D_GLIBCXX_DEBUG
CPPFLAGS = -Wall -I. -I../../../3rdParty/v8/include
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
	rm -f dummy 
	rm -f *.a 
	rm -f $(OBJFILES)
	rm -f $(DEPENDS)

#### dependencies

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CXX) -MM -MP $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

ifneq ($(MAKECMDGOALS),clean)
include $(DEPENDS)
endif
