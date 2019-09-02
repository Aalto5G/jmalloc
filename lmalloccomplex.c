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

static inline int fast_rand_mod2_10(void) {
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>22);
}

struct block {
  void *ptr;
  size_t sz;
};

struct block blks[1024] = {};

int main(int argc, char **argv)
{
  size_t i, j;
  for (i = 0; i < 100*1000*1000; i++)
  {
    size_t idx[10];
    size_t sz;
    idx[0] = fast_rand_mod2_10();
    idx[1] = fast_rand_mod2_10();
    idx[2] = fast_rand_mod2_10();
    idx[3] = fast_rand_mod2_10();
    idx[4] = fast_rand_mod2_10();
    idx[5] = fast_rand_mod2_10();
    idx[6] = fast_rand_mod2_10();
    idx[7] = fast_rand_mod2_10();
    idx[8] = fast_rand_mod2_10();
    idx[9] = fast_rand_mod2_10();
    sz = 1<<(4 + (fast_rand_mod2_10()>>8));
    for (j = 0; j < 5; j++)
    {
      if (blks[idx[j]].ptr)
      {
        free(blks[idx[j]].ptr);
      }
      blks[idx[j]].sz = sz<<j;
      blks[idx[j]].ptr = malloc(sz<<j);
    }
    for (j = 5; j < 10; j++)
    {
      if (blks[idx[j]].ptr)
      {
        free(blks[idx[j]].ptr);
      }
      blks[idx[j]].sz = sz<<(j-5);
      blks[idx[j]].ptr = malloc(sz<<(j-5));
    }
  }
}
