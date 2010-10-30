.PHONY: asml run clean

ASMLDIR = ../asml

LDFLAGS = -g -L../../../3rdParty/v8 -L$(ASMLDIR)
LDLIBS =  -lasml -lQuantLib -lv8
CXXFLAGS = -g -I$(ASMLDIR)

OBJECTS = example.o MCTest.o

example: $(ASMLDIR)/libasml.a $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $@

$(ASMLDIR)/libasml.a:
	$(MAKE) -C $(ASMLDIR) libasml.a

asml:
	$(MAKE) -C $(ASMLDIR) $(MAKECMDGOALS)

run: example
	./example

clean: asml
	rm -f *.o example

 %.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

ifneq ($(MAKECMDGOALS),clean)
include $(OBJECTS:.o=.d)
endif
