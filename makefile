PROG1 = BigWorker.x
CC = mpicc
FLAGS=fopenmp

all: $(PROG1)

%.x: %.o
	$(CC) -$(FLAGS) $< -o $@

%.o: %.cpp
	$(CC) -$(FLAGS) -c $<

clean:
	rm $(PROG1)
