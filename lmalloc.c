#include "linkedlist.h"
#include <sys/mman.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

void *volatile ptr;

static unsigned int g_seed;

static inline void fast_srand(int seed) {
    g_seed = seed;
}

static inline int fast_rand_mod4(void) {
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>30);
}

/*
 * I was reminded that large system call count could explain why malloc() is
 * so slow. Run this program with strace ./lmalloc and the brk() calls show up
 * like this:
 *
 * $ strace ./lmalloc
 * [snip]
 * brk(NULL)                               = 0x557db3000000
 * brk(0x557db3021000)                     = 0x557db3021000
 * exit_group(0)                           = ?
 * +++ exited with 0 +++
 *
 * So, the reason for malloc() slowness is not large system call count.
 *
 */

int main(int argc, char **argv)
{
  size_t i;
  for (i = 0; i < 100*1000*1000; i++)
  {
    size_t sz = 1<<(4 + fast_rand_mod4());
    ptr = malloc(sz);
    free(ptr);
    ptr = malloc(sz<<1);
    free(ptr);
    ptr = malloc(sz<<2);
    free(ptr);
    ptr = malloc(sz<<3);
    free(ptr);
    ptr = malloc(sz<<4);
    free(ptr);
    ptr = malloc(sz);
    free(ptr);
    ptr = malloc(sz<<1);
    free(ptr);
    ptr = malloc(sz<<2);
    free(ptr);
    ptr = malloc(sz<<3);
    free(ptr);
    ptr = malloc(sz<<4);
    free(ptr);
  }
}
