#include <sys/mman.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

struct jmalloc_block {
  union {
    char block[0];
    struct jmalloc_block *next;
  } u;
};

size_t arenaremain;
char *arena;

// 16, 32, 64, 128, 256, 512, 1024, 2048
struct jmalloc_block *blocks[8] = {};

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

const uint8_t lookup[129] = {
0,0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
};

__attribute__((noinline)) void *jmalloc(size_t sz)
{
  struct jmalloc_block **ls = NULL;
  struct jmalloc_block *blk;
  void *ret;
  uint8_t lookupval;
  if (sz > 2048)
  {
    ret = mmap(NULL, topages(sz), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if (ret == MAP_FAILED)
    {
      return NULL;
    }
  }
  lookupval = lookup[(sz+15)/16];
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

  if (!*ls)
  {
    if (arenaremain < sz)
    {
      arenaremain = 32*1024*1024;
      arena = mmap(NULL, arenaremain, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
      if (arena == MAP_FAILED || arena == NULL)
      {
        abort();
      }
    }
    if (arenaremain < sz)
    {
      abort();
    }
    ret = arena;
    arenaremain -= sz;
    arena += sz;
    return ret;
  }
  blk = *ls;
  *ls = blk->u.next;
  return blk;
}

__attribute__((noinline)) void jmfree(void *ptr, size_t sz)
{
  struct jmalloc_block **ls = NULL;
  struct jmalloc_block *blk = ptr;
  uint8_t lookupval;
  if (sz > 2048)
  {
    munmap(ptr, topages(sz));
    return;
  }
  lookupval = lookup[(sz+15)/16];
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
  blk->u.next = *ls;
  *ls = blk;
}

int main(int argc, char **argv)
{
  size_t i;
  for (i = 0; i < 100*1000*1000; i++)
  {
    size_t sz = 1<<(4 + rand()%4);
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