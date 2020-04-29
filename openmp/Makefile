
GCC = gcc
CFLAGS = -O3 -fopenmp -w

CMAIN=qsort

all: qsort-main.o qsort-sequential.o
	$(GCC) $(CFLAGS) $^ -o $(CMAIN)

%.o: %.c
	$(GCC) -c $(CFLAGS) $^

clean:
	rm -f *.o *~ $(CMAIN)
