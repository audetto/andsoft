.PHONY: asml run clean

ASMLDIR = ../asml

LDFLAGS = -g -L../../../3rdParty/v8 -L$(ASMLDIR)
LDLIBS =  -lasml -lQuantLib -lv8
CXXFLAGS = -g -I$(ASMLDIR)

OBJECTS = example.o MCTest.o

example: $(ASMLDIR)/libasml.a $(OBJECTS)

$(ASMLDIR)/libasml.a:
	$(MAKE) -C $(ASMLDIR) libasml.a

asml:
	$(MAKE) -C $(ASMLDIR) $(MAKECMDGOALS)

run: example
	./example

clean: asml
	rm -f *.o example

include ../build/depend.mk
