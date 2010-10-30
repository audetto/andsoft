DEPENDS = $(OBJFILES:.o=.d)

%.d: %.cpp
	@set -e; echo "Dependencies for $<"; rm -f $@; \
	$(CXX) -MM -MP $(CPPFLAGS) -MT '$*.o' $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

ifneq ($(MAKECMDGOALS),clean)
include $(DEPENDS)
endif
