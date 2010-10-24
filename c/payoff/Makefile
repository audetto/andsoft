
LDFLAGS = -g -L/home/andrea/projects/cvs/3rdParty/v8
LDLIBS =  -lQuantLib -lv8
CXXFLAGS = -g -I/home/andrea/projects/cvs/3rdParty/v8/include

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
