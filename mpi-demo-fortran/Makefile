JULIA_DIR = $(shell julia -e 'print(dirname(Sys.BINDIR))')

JL_SHARE = $(shell julia -e 'print(joinpath(Sys.BINDIR, Base.DATAROOTDIR, "julia"))')
CFLAGS   += $(shell $(JL_SHARE)/julia-config.jl --cflags)
CXXFLAGS += $(shell $(JL_SHARE)/julia-config.jl --cflags)
LDFLAGS  += $(shell $(JL_SHARE)/julia-config.jl --ldflags)
LDLIBS   += $(shell $(JL_SHARE)/julia-config.jl --ldlibs)

all: mpi-demo-fortran

mpi-demo-fortran: mpi-demo-fortran.f90 libtrixi.a
	mpif90 $(LDFLAGS) -O0 -g $< -o $@ libtrixi.a $(LDLIBS)

libtrixi.o: libtrixi.c
	mpicc $(CFLAGS) $(LDFLAGS) -O0 -g -c $< -o $@ $(LDLIBS)

libtrixi.a: libtrixi.o
	ar rcs $@ $^

.PHONY: all clean

clean:
	rm -f mpi-demo-fortran libtrixi.a libtrixi.o
