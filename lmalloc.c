#include "linkedlist.h"
#include <sys/mman.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

void *volatile ptr;

static unsigned int g_seed;

// Used to seed the generator.
static inline void fast_srand(int seed) {
    g_seed = seed;
}

// Compute a pseudorandom integer.
// Output value in range [0, 32767]
static inline int fast_rand(void) {
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}

int main(int argc, char **argv)
{
  size_t i;
  for (i = 0; i < 100*1000*1000; i++)
  {
    size_t sz = 1<<(4 + fast_rand()%4);
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
