.PHONY: all clean

OBJFILES += asml/utils/utils.o asml/utils/error.o
OBJFILES += asml/linearalgebra/projection.o asml/linearalgebra/linear.o  asml/linearalgebra/fastexp.o asml/linearalgebra/hypermatrix.o
OBJFILES += asml/analytics/implied.o asml/analytics/numerics.o 
OBJFILES += asml/distribution/distribution.o asml/distribution/fourierpricing.o  asml/distribution/heston.o
OBJFILES += asml/pde/generator.o asml/pde/pde.o
OBJFILES += asml/process/brownianmotionwithdrift.o
OBJFILES += asml/payoff/AmericanLookback.o asml/payoff/GenericPathOption.o asml/payoff/JSPayoff.o
OBJFILES += asml/marketdata/Stock.o asml/marketdata/RawMarketData.o asml/marketdata/MarketData.o asml/marketdata/Correlation.o

## It seems QuantLib has been compiled without D_GLIBCXX_DEBUG
CPPFLAGS = -Wall -I. -I../../../3rdParty/v8/include
##  -D_GLIBCXX_DEBUG
CXXFLAGS = -g

LDLIBS = -lgsl -lgslcblas -lm -lQuantLib

AR = ar
RANLIB = ranlib

all: libasml.a

libasml.a: $(OBJFILES)
	$(AR) cr $@ $^
	$(RANLIB) $@

dummy: asml/dummy.o libasml.a

clean:
	rm -f dummy 
	rm -f $(OBJFILES)
	rm -f *.a 

include ../build/depend.mk
