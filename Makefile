.PHONY: all

all: jmalloc2 jmalloc lmalloc jmalloc2complex lmalloccomplex

jmalloc2: jmalloc2.c
	cc -O3 -o $@ $<

jmalloc: jmalloc.c
	cc -O3 -o $@ $<

lmalloc: lmalloc.c
	cc -O3 -o $@ $<

jmalloc2complex: jmalloc2complex.c
	cc -O3 -o $@ $<

lmalloccomplex: lmalloccomplex.c
	cc -O3 -o $@ $<
