PROG1 = parallel_code.x
CC = tau_cc.sh
FLAGS=fopenmp

export TAU_MAKEFILE=/opt/sw/tau/x86_64/lib/Makefile.tau-papi-mpi-pdt-openmp-opari

all: $(PROG1)

%.x: %.o
	$(CC) -$(FLAGS) $< -o $@

%.o: %.cpp
	$(CC) -$(FLAGS) -c $<

clean:
	rm $(PROG1)
