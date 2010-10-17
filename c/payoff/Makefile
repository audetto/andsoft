
LDFLAGS = -g
LDLIBS =  -lQuantLib
CXXFLAGS = -g

OBJECTS = example.o AmericanLookback.o GenericPathOption.o JSPayoff.o MCTest.o

example: $(OBJECTS)
	$(CC) $(LDFLAGS) $+ $(LOADLIBES) $(LDLIBS) -o $@

run: example
	./example

clean:
	rm -f *.o example

 %.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

include $(OBJECTS:.o=.d)
