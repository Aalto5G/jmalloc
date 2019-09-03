.PHONY: all

all: allocreal allocstd

allocreal.c: allocanalyze.py allocs.txt
	python allocanalyze.py jm > allocreal.c

allocstd.c: allocanalyze.py allocs.txt
	python allocanalyze.py std > allocstd.c

allocreal.o: allocreal.c
	cc -O3 -c -o $@ $<

allocstd.o: allocstd.c
	cc -O3 -c -o $@ $<

jmalloccore.o: jmalloccore.c
	cc -O3 -c -o $@ $<

allocreal: allocreal.o jmalloccore.o
	cc -o $@ $^

allocstd: allocstd.o
	cc -o $@ $^
