.PHONY = all lib clean headers

OBJFILES = utils.o projection.o linear.o heston.o implied.o numerics.o
DEPENDS = $(OBJFILES:.o=.d)

CPPFLAGS = -Wall -I.
CXXFLAGS = -gdwarf-2

LDLIBS = -lgsl -lgslcblas -lm

AR = ar
RANLIB = ranlib

all: headers libasml.a

headers:
	$(MAKE) -C asi all 

libasml.a: $(OBJFILES)
	$(AR) crv $@ $^
	$(RANLIB) $@

dummy: dummy.o $(OBJFILES)
	g++ $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ -L. -lasml $(LDLIBS)

clean:
	$(MAKE) -C asi clean
	rm -rf dummy *.o *.a *.d

#### dependencies

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CXX) -MM -MP $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

include $(DEPENDS)
