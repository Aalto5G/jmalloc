#include "linkedlist.h"
#include "clz.h"
#include "likely.h"
#include <sys/mman.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

#define CONTAINER_OF(ptr, type, member) \
  ((type*)(((char*)ptr) - offsetof(type, member)))

struct jmalloc_block {
  union {
    struct linked_list_node llnode;
  } u;
};

size_t arenaremain;
char *arena;

// 16, 32, 64, 128, 256, 512, 1024, 2048
struct linked_list_head blocks[8] = {
  {.node = {.prev = &blocks[0].node, .next = &blocks[0].node}},
  {.node = {.prev = &blocks[1].node, .next = &blocks[1].node}},
  {.node = {.prev = &blocks[2].node, .next = &blocks[2].node}},
  {.node = {.prev = &blocks[3].node, .next = &blocks[3].node}},
  {.node = {.prev = &blocks[4].node, .next = &blocks[4].node}},
  {.node = {.prev = &blocks[5].node, .next = &blocks[5].node}},
  {.node = {.prev = &blocks[6].node, .next = &blocks[6].node}},
  {.node = {.prev = &blocks[7].node, .next = &blocks[7].node}},
};

static inline size_t topages(size_t limit)
{
  long pagesz = sysconf(_SC_PAGE_SIZE);
  size_t pages, actlimit;
  if (pagesz <= 0)
  {
    abort();
  }
  pages = (limit + (pagesz-1)) / pagesz;
  actlimit = pages * pagesz;
  return actlimit;
}

#ifndef HAS_CLZ
const uint8_t lookup[129] = {
0,0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
};
#endif

__attribute__((noinline)) void *jmalloc(size_t sz)
{
  struct linked_list_head *ls = NULL;
  struct jmalloc_block *blk;
  void *ret;
  int8_t lookupval;
  if (unlikely(sz > 2048))
  {
    ret = mmap(NULL, topages(sz), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if (unlikely(ret == MAP_FAILED))
    {
      return NULL;
    }
    return ret;
  }
#ifdef HAS_CLZ
  lookupval = 28-__builtin_clz(sz ? (sz-1) : 0);
  if (lookupval < 0)
  {
    lookupval = 0;
  }
#else
  lookupval = lookup[(sz+15)/16];
#endif
  if (lookupval == 0 && sizeof(struct jmalloc_block) > 16)
  {
    lookupval = 1;
  }
  if (lookupval == 1 && sizeof(struct jmalloc_block) > 32)
  {
    lookupval = 2;
  }
  if (lookupval == 2 && sizeof(struct jmalloc_block) > 64)
  {
    lookupval = 3;
  }
  if (lookupval == 3 && sizeof(struct jmalloc_block) > 128)
  {
    lookupval = 4;
  }
  if (lookupval == 4 && sizeof(struct jmalloc_block) > 256)
  {
    lookupval = 5;
  }
  if (lookupval == 5 && sizeof(struct jmalloc_block) > 512)
  {
    lookupval = 6;
  }
  if (lookupval == 6 && sizeof(struct jmalloc_block) > 1024)
  {
    lookupval = 7;
  }
  if (sizeof(struct jmalloc_block) > 2048)
  {
    abort();
  }
  ls = &blocks[lookupval];
  sz = 1<<(4+lookupval);

  if (unlikely(linked_list_is_empty(ls)))
  {
    if (unlikely(arenaremain < sz))
    {
      arenaremain = 32*1024*1024;
      arena = mmap(NULL, arenaremain, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
      if (unlikely(arena == MAP_FAILED || arena == NULL))
      {
        abort();
      }
    }
    if (unlikely(arenaremain < sz))
    {
      abort();
    }
    ret = arena;
    arenaremain -= sz;
    arena += sz;
    return ret;
  }
  blk = CONTAINER_OF(ls->node.next, struct jmalloc_block, u.llnode);
  linked_list_delete(&blk->u.llnode);
  return blk;
}

__attribute__((noinline)) void jmfree(void *ptr, size_t sz)
{
  struct linked_list_head *ls = NULL;
  struct jmalloc_block *blk = ptr;
  int8_t lookupval;
  if (unlikely(sz > 2048))
  {
    munmap(ptr, topages(sz));
    return;
  }
#ifdef HAS_CLZ
  lookupval = 28-__builtin_clz(sz ? (sz-1) : 0);
  if (lookupval < 0)
  {
    lookupval = 0;
  }
#else
  lookupval = lookup[(sz+15)/16];
#endif
  if (lookupval == 0 && sizeof(struct jmalloc_block) > 16)
  {
    lookupval = 1;
  }
  if (lookupval == 1 && sizeof(struct jmalloc_block) > 32)
  {
    lookupval = 2;
  }
  if (lookupval == 2 && sizeof(struct jmalloc_block) > 64)
  {
    lookupval = 3;
  }
  if (lookupval == 3 && sizeof(struct jmalloc_block) > 128)
  {
    lookupval = 4;
  }
  if (lookupval == 4 && sizeof(struct jmalloc_block) > 256)
  {
    lookupval = 5;
  }
  if (lookupval == 5 && sizeof(struct jmalloc_block) > 512)
  {
    lookupval = 6;
  }
  if (lookupval == 6 && sizeof(struct jmalloc_block) > 1024)
  {
    lookupval = 7;
  }
  if (sizeof(struct jmalloc_block) > 2048)
  {
    abort();
  }
  ls = &blocks[lookupval];
  sz = 1<<(4+lookupval);
  linked_list_add_head(&blk->u.llnode, ls);
}

static unsigned int g_seed;

static inline void fast_srand(int seed) {
    g_seed = seed;
}

static inline int fast_rand_mod4(void) {
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>30);
}

int main(int argc, char **argv)
{
  size_t i;
  for (i = 0; i < 100*1000*1000; i++)
  {
    size_t sz = 1<<(4 + fast_rand_mod4());
    jmfree(jmalloc(sz), sz);
    jmfree(jmalloc(sz<<1), sz<<1);
    jmfree(jmalloc(sz<<2), sz<<2);
    jmfree(jmalloc(sz<<3), sz<<3);
    jmfree(jmalloc(sz<<4), sz<<4);
    jmfree(jmalloc(sz), sz);
    jmfree(jmalloc(sz<<1), sz<<1);
    jmfree(jmalloc(sz<<2), sz<<2);
    jmfree(jmalloc(sz<<3), sz<<3);
    jmfree(jmalloc(sz<<4), sz<<4);
  }
}
