.PHONY: all

all: allocreal allocstd allocrealset allocstdset

ops.c: allocanalyze.py allocs.txt
	python2 allocanalyze.py ops > ops.c

allocreal.c: allocanalyze.py allocs.txt
	python2 allocanalyze.py jm > allocreal.c

allocrealset.c: allocanalyze.py allocs.txt
	python2 allocanalyze.py jmset > allocrealset.c

allocstd.c: allocanalyze.py allocs.txt
	python2 allocanalyze.py std > allocstd.c

allocstdset.c: allocanalyze.py allocs.txt
	python2 allocanalyze.py stdset > allocstdset.c

allocreal.o: allocreal.c
	cc -O3 -c -o $@ $<

allocstd.o: allocstd.c
	cc -O3 -c -o $@ $<

allocrealset.o: allocrealset.c
	cc -O3 -c -o $@ $<

allocstdset.o: allocstdset.c
	cc -O3 -c -o $@ $<

ops.o: ops.c
	cc -O3 -c -o $@ $<

jmalloccore.o: jmalloccore.c
	cc -O3 -c -o $@ $<

allocreal: allocreal.o jmalloccore.o ops.o
	cc -o $@ $^

allocstd: allocstd.o ops.o
	cc -o $@ $^

allocrealset: allocrealset.o jmalloccore.o ops.o
	cc -o $@ $^

allocstdset: allocstdset.o ops.o
	cc -o $@ $^
