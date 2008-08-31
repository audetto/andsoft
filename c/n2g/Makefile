.PHONY : all clean superclean

OBJS = d1mach.o  dd7upd.o  dg7qsb.o  divset.o  dl7nvr.o  dl7tsq.o  dn2f.o    dparck.o  dr7tvm.o  ds7dmp.o  dv2nrm.o  dv7scp.o  i7copy.o  r7mdc.o \
da7sst.o  df7dhb.o  dg7qts.o  dl7itv.o  dl7sqr.o  dl7tvm.o  dn2gb.o   dq7apl.o  drldst.o  ds7ipr.o  dv7cpy.o  dv7shf.o  i7mdcn.o  stopx.o \
dc7vfn.o  df7hes.o  dh2rfa.o  dl7ivm.o  dl7srt.o  dl7vml.o  dn2lrd.o  dq7rad.o  drn2gb.o  ds7lup.o  dv7dfl.o  dv7vmp.o  i7pnvr.o  v2nrm.o \
dd7mlp.o  dg7itb.o  dh2rfg.o  dl7msb.o  dl7svn.o  dn2cvp.o  dn2rdp.o  dq7rsh.o  drn2g.o   ds7lvm.o  dv7ipr.o  i7shft.o  dd7tpr.o  dg7lit.o  \
ditsum.o  dl7mst.o  dl7svx.o  dn2fb.o   do7prd.o  dr7mdc.o  ds7bqn.o  dv2axy.o  dv7scl.o  i1mach.o  r1mach.o

FORTRAN_DIR = /home/andrea/projects/maths/fortran

CPPFLAGS = -I$(FORTRAN_DIR)/libf2c

CFLAGS = -g
CXXFLAGS = -g

LDFLAGS = -g

LDLIBS = -L$(FORTRAN_DIR)/libf2c -lf2c -lm

CC = g++

AR = ar
RANLIB = ranlib

%.c:	%.f
	$(FORTRAN_DIR)/f2c/f2c -C++ $^

%.f:
	wget http://www.netlib.org/port/$@

# we need to switch off rule that compiles fortran files.
%.o: 	%.f

all:	libn2g.a

libn2g.a:	$(OBJS)
	$(AR) crv $@ $^
	$(RANLIB) $@

nl2sol.f:
	cp nl2sol.ff nl2sol.f

clean:
	rm -f *.o
	rm -f *.c
	rm -f *.a

superclean: clean
	rm -f *.f

# so we keep intermeriary files
.SECONDARY:
