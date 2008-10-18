.PHONY = all headers clean dummy

LN_S = ln -s
top_srcdir = ..

all: header

header: clean
	HEADERLIST="$(top_srcdir)/*.h"; \
	for h in $$HEADERLIST; do \
	  BASENAME=`basename $$h`; \
	  test -r $$BASENAME || $(LN_S) $$h $$BASENAME; \
	done

clean: 
	rm -f *.h

# need to find a better way here
dummy: all
