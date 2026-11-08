#define _GNU_SOURCE
#include "jmalloclib.h"
#include "jmallocclz.h"
#include "../anon.h"
#include "jmalloclikely.h"
#include <sys/mman.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>
#include <dlfcn.h>

typedef int(*c_posix_memalign_t)(void **membptr, size_t alignment, size_t size);
typedef void*(*c_aligned_alloc_t)(size_t alignment, size_t size);
typedef void*(*c_valloc_t)(size_t size);
typedef void*(*c_memalign_t)(size_t alignment, size_t size);
typedef void*(*c_pvalloc_t)(size_t size);
typedef void*(*c_realloc_t)(void *ptr, size_t size);
typedef int(*c_free_t)(void *ptr);

c_posix_memalign_t c_posix_memalign;
c_aligned_alloc_t c_aligned_alloc;
c_valloc_t c_valloc;
c_memalign_t c_memalign;
c_pvalloc_t c_pvalloc;
c_realloc_t c_realloc;
c_free_t c_free;
int has_c = 0;

void get_c_impl(void)
{
  c_posix_memalign = (c_posix_memalign_t)dlsym(RTLD_NEXT, "posix_memalign");
  c_aligned_alloc = (c_aligned_alloc_t)dlsym(RTLD_NEXT, "aligned_alloc");
  c_valloc = (c_valloc_t)dlsym(RTLD_NEXT, "valloc");
  c_memalign = (c_memalign_t)dlsym(RTLD_NEXT, "memalign");
  c_pvalloc = (c_pvalloc_t)dlsym(RTLD_NEXT, "pvalloc");
  c_realloc = (c_realloc_t)dlsym(RTLD_NEXT, "realloc");
  c_free = (c_free_t)dlsym(RTLD_NEXT, "free");
  has_c = 1;
}

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define ALIGN (_Alignof(max_align_t))
#else
static inline size_t get_max_align_compatibility(void)
{
  size_t res = 8;
  if (sizeof(void*) > res) res *= 2;
  if (sizeof(void*) > res) res *= 2;
  if (sizeof(void*) > res) res *= 2;
  if (sizeof(void*) > res) res *= 2;
  if (sizeof(void(*)(void)) > res) res *= 2;
  if (sizeof(void(*)(void)) > res) res *= 2;
  if (sizeof(void(*)(void)) > res) res *= 2;
  if (sizeof(void(*)(void)) > res) res *= 2;
  if (sizeof(ptrdiff_t) > res) res *= 2;
  if (sizeof(ptrdiff_t) > res) res *= 2;
  if (sizeof(ptrdiff_t) > res) res *= 2;
  if (sizeof(ptrdiff_t) > res) res *= 2;
  if (sizeof(off_t) > res) res *= 2;
  if (sizeof(off_t) > res) res *= 2;
  if (sizeof(off_t) > res) res *= 2;
  if (sizeof(off_t) > res) res *= 2;
  if (sizeof(size_t) > res) res *= 2;
  if (sizeof(size_t) > res) res *= 2;
  if (sizeof(size_t) > res) res *= 2;
  if (sizeof(size_t) > res) res *= 2;
  if (sizeof(ssize_t) > res) res *= 2;
  if (sizeof(ssize_t) > res) res *= 2;
  if (sizeof(ssize_t) > res) res *= 2;
  if (sizeof(ssize_t) > res) res *= 2;
  if (sizeof(long long) > res) res *= 2;
  if (sizeof(long long) > res) res *= 2;
  if (sizeof(long long) > res) res *= 2;
  if (sizeof(long long) > res) res *= 2;
  if (sizeof(unsigned long long) > res) res *= 2;
  if (sizeof(unsigned long long) > res) res *= 2;
  if (sizeof(unsigned long long) > res) res *= 2;
  if (sizeof(unsigned long long) > res) res *= 2;
  if (sizeof(double) > res) res *= 2;
  if (sizeof(double) > res) res *= 2;
  if (sizeof(double) > res) res *= 2;
  if (sizeof(double) > res) res *= 2;
  if (sizeof(long double) > res) res *= 2;
  if (sizeof(long double) > res) res *= 2;
  if (sizeof(long double) > res) res *= 2;
  if (sizeof(long double) > res) res *= 2;
  return res;
}
#define ALIGN (get_max_align_compatibility())
#endif


static inline void *malloc_impl(size_t sz)
{
  size_t realsz;
  char *blk;
  size_t myalign = ALIGN;
  //printf("malloc sz %zu\n", blk+myalign, sz);
  if (myalign < sizeof(size_t))
  {
    myalign = sizeof(size_t);
  }
  if (sz > SIZE_MAX-1 - myalign)
  {
    return NULL;
  }
  realsz = sz + myalign;
  blk = jmalloc(realsz);
  if (!blk)
  {
    return NULL;
  }
  blk += myalign;
  blk -= sizeof(size_t);
  memcpy(&blk[0], &sz, sizeof(size_t));
  blk += sizeof(size_t);
  //printf("malloc ret %p sz %zu\n", blk+myalign, sz);
  return blk;
}
void *malloc(size_t sz)
{
  return malloc_impl(sz);
}
static inline void free_impl(void *ptr)
{
  size_t sz;
  size_t realsz;
  char *blk;
  size_t myalign = ALIGN;
  if (myalign < sizeof(size_t))
  {
    myalign = sizeof(size_t);
  }
  //printf("free given %p\n", ptr);
  if (!ptr)
  {
    return;
  }
  blk = ptr;
  blk -= sizeof(size_t);
  memcpy(&sz, &blk[0], sizeof(size_t));
  blk += sizeof(size_t);
  if (sz == SIZE_MAX)
  {
    size_t alignment2, alignment;
    blk -= 4*sizeof(size_t);
    memcpy(&alignment2, &blk[0*sizeof(size_t)], sizeof(size_t));
    memcpy(&alignment, &blk[1*sizeof(size_t)], sizeof(size_t));
    memcpy(&sz, &blk[2*sizeof(size_t)], sizeof(size_t));
    blk += 4*sizeof(size_t);
    blk -= alignment2;
    if (!has_c) get_c_impl();
    c_free(blk);
    return;
  }
  blk -= myalign;
  //printf("free given %p sz %zu\n", blk+myalign, sz);
  if (sz > SIZE_MAX-1 - myalign)
  {
    abort();
  }
  realsz = sz + myalign;
  jmfree(blk, realsz);
}
void free(void *ptr)
{
  free_impl(ptr);
}
void *calloc(size_t nmemb, size_t size)
{
  void *res;
  if (size == 0 || nmemb == 0)
  {
    return malloc_impl(0);
  }
  if (SIZE_MAX/size < nmemb)
  {
    return NULL;
  }
  res = malloc_impl(size*nmemb);
  if (!res)
  {
    return NULL;
  }
  memset(res, 0, size*nmemb);
  return res;
}
static inline void *realloc_impl(void *ptr, size_t size2)
{
  size_t sz;
  size_t realsz;
  size_t realsz2;
  char *blk;
  char *blk2;
  size_t myalign = ALIGN;
  if (myalign < sizeof(size_t))
  {
    myalign = sizeof(size_t);
  }
  if (size2 > SIZE_MAX-1 - myalign)
  {
    return NULL;
  }
  if (!ptr)
  {
    return malloc_impl(size2);
  }
  if (size2 == 0)
  {
    free_impl(ptr);
    return NULL;
  }
  blk = ptr;
  blk -= sizeof(size_t);
  memcpy(&sz, &blk[0], sizeof(size_t));
  blk += sizeof(size_t);
  if (sz == SIZE_MAX)
  {
    char *ptr2;
    size_t alignment2, alignment;
    blk -= 4*sizeof(size_t);
    memcpy(&alignment2, &blk[0*sizeof(size_t)], sizeof(size_t));
    memcpy(&alignment, &blk[1*sizeof(size_t)], sizeof(size_t));
    memcpy(&sz, &blk[2*sizeof(size_t)], sizeof(size_t));
    blk += 4*sizeof(size_t);
    blk -= alignment2;
    if (!has_c) get_c_impl();
    ptr2 = c_realloc(blk, size2+alignment2);
    if (ptr2 == NULL)
    {
      return NULL;
    }
    ptr2 += alignment2;
    ptr2 -= 4*sizeof(size_t);
    memcpy(&ptr2[2*sizeof(size_t)], &size2, sizeof(size_t));
    ptr2 += 4*sizeof(size_t);
    return ptr2;
  }
  blk -= myalign;
  if (sz > SIZE_MAX-1 - myalign)
  {
    abort();
  }
  realsz = sz + myalign;
  realsz2 = size2 + myalign;
  blk2 = jmalloc(realsz2);
  if (!blk2)
  {
    return NULL;
  }
  memcpy(&blk2[myalign-sizeof(size_t)], &size2, sizeof(size_t));
  memcpy(&blk2[myalign], &blk[myalign], sz < size2 ? sz : size2);
  jmfree(blk, realsz);
  return blk2+myalign;
}
void *realloc(void *ptr, size_t size2)
{
  return realloc_impl(ptr, size2);
}
void *reallocarray(void *ptr, size_t nmemb, size_t size)
{
  if (size == 0 || nmemb == 0)
  {
    return realloc_impl(ptr, 0);
  }
  if (SIZE_MAX/size < nmemb)
  {
    return NULL;
  }
  return realloc_impl(ptr, size*nmemb);
}

size_t malloc_usable_size(void *ptr)
{
  size_t myalign = ALIGN;
  char *blk;
  size_t sz;
  if (myalign < sizeof(size_t))
  {
    myalign = sizeof(size_t);
  }
  if (!ptr) return 0;
  blk = ptr;
  blk -= sizeof(size_t);
  memcpy(&sz, &blk[0], sizeof(size_t));
  blk += sizeof(size_t);
  if (sz == SIZE_MAX)
  {
    size_t alignment, alignment2;
    blk -= 4*sizeof(size_t);
    memcpy(&alignment2, &blk[0*sizeof(size_t)], sizeof(size_t));
    memcpy(&alignment, &blk[1*sizeof(size_t)], sizeof(size_t));
    memcpy(&sz, &blk[2*sizeof(size_t)], sizeof(size_t));
    blk += 4*sizeof(size_t);
    return sz;
  }
  return sz;
}

int posix_memalign(void **membptr, size_t alignment, size_t size)
{
  size_t myalign = ALIGN;
  size_t alignment2;
  int ret;
  char *res;
  size_t sizemax = SIZE_MAX;
  while (myalign < 4*sizeof(size_t))
  {
    myalign *= 2;
  }
  alignment2 = (alignment < myalign ? myalign : alignment);
  if (!has_c) get_c_impl();
  ret = c_posix_memalign(membptr, alignment, size+alignment2);
  if (ret)
  {
    return ret;
  }
  res = *membptr;
  res += alignment2;
  res -= 4*sizeof(size_t);
  memcpy(&res[0*sizeof(size_t)], &alignment2, sizeof(size_t));
  memcpy(&res[1*sizeof(size_t)], &alignment, sizeof(size_t));
  memcpy(&res[2*sizeof(size_t)], &size, sizeof(size_t));
  memcpy(&res[3*sizeof(size_t)], &sizemax, sizeof(size_t));
  *membptr = res + 4*sizeof(size_t);
  return 0;
}
void *aligned_alloc(size_t alignment, size_t size)
{
  size_t myalign = ALIGN;
  char *res;
  size_t alignment2;
  size_t sizemax = SIZE_MAX;
  while (myalign < 4*sizeof(size_t))
  {
    myalign *= 2;
  }
  alignment2 = (alignment < myalign ? myalign : alignment);
  if (!has_c) get_c_impl();
  res = c_aligned_alloc(alignment, size+alignment2);
  if (!res)
  {
    return NULL;
  }
  res += alignment2;
  res -= 4*sizeof(size_t);
  memcpy(&res[0*sizeof(size_t)], &alignment2, sizeof(size_t));
  memcpy(&res[1*sizeof(size_t)], &alignment, sizeof(size_t));
  memcpy(&res[2*sizeof(size_t)], &size, sizeof(size_t));
  memcpy(&res[3*sizeof(size_t)], &sizemax, sizeof(size_t));
  return res + 4*sizeof(size_t);
}
void *memalign(size_t alignment, size_t size)
{
  size_t myalign = ALIGN;
  char *res;
  size_t alignment2;
  size_t sizemax = SIZE_MAX;
  while (myalign < 4*sizeof(size_t))
  {
    myalign *= 2;
  }
  alignment2 = (alignment < myalign ? myalign : alignment);
  if (!has_c) get_c_impl();
  res = c_memalign(alignment, size+alignment2);
  if (!res)
  {
    return NULL;
  }
  res += alignment2;
  res -= 4*sizeof(size_t);
  memcpy(&res[0*sizeof(size_t)], &alignment2, sizeof(size_t));
  memcpy(&res[1*sizeof(size_t)], &alignment, sizeof(size_t));
  memcpy(&res[2*sizeof(size_t)], &size, sizeof(size_t));
  memcpy(&res[3*sizeof(size_t)], &sizemax, sizeof(size_t));
  return res + 4*sizeof(size_t);
}
void *valloc(size_t size)
{
  return memalign(sysconf(_SC_PAGE_SIZE), size);
}
void *pvalloc(size_t size)
{
  size_t page_size = sysconf(_SC_PAGE_SIZE);
  size_t leftover = 0;
  if (page_size < 1)
  {
    page_size = 1;
  }
  if (size % page_size)
  {
    leftover = page_size - size % page_size;
  }
  if (size > SIZE_MAX - leftover)
  {
    return NULL;
  }
  size += leftover;
  return valloc(size);
}
