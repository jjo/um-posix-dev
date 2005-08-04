TARGETS=matrix-cache-bad matrix-cache-good
CFLAGS=-Wall -g -O2

all : $(TARGETS)
matrix-cache-bad: matrix-cache.c
	gcc -o $(@) $(CFLAGS) -DCACHE_BAD $(^)

matrix-cache-good: matrix-cache.c
	gcc -o $(@) $(CFLAGS) -DCACHE_GOOD $(^)

test: all
	sh -c 'time ./matrix-cache-bad' 
	sh -c 'time ./matrix-cache-good' 
clean:
	rm -fv $(TARGETS) *.o core
