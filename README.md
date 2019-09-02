# jmalloc

jmalloc is a high-performance memory allocator on a different location of the
high performance - low fragmentation scale than most allocators out there. It
purposefully has an interface different from malloc. The difference is that
jmalloc does not store the allocated block size anywhere, so the calling
program needs to pass the block size to jmalloc when freeing a block.

Paging is needed to satisfy allocations larger than 2048 bytes, and most
importantly, to allocate the arenas. So, don't use jmalloc on a system that
lacks mmap!

## Usage

How to compile:

```
make
```

How to run:

```
time ./jmalloc2
```

The variant `jmalloc2` is the current varant. The older `jmalloc` uses doubly
linked lists instead of singly linked lists.

You can compare jmalloc performance to Linux allocator or various other
allocators:

```
time ./lmalloc
LD_PRELOAD=../sfmalloc/libsfmalloc.so time ./lmalloc
LD_PRELOAD=../Hoard/src/libhoard.so time ./lmalloc
LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libjemalloc.so.1 time ./lmalloc
LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libtcmalloc.so.4 time ./lmalloc
```

Not the sfmalloc is buggy: it requires changing `-lpthread` to `-pthread` in
the Makefile to actually work.
