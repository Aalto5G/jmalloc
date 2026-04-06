#ifndef _ANON_H_
#define _ANON_H_

#include <sys/mman.h>

#ifdef MAP_ANON
#define MY_MAP_ANONYMOUS MAP_ANON
#else
#ifdef MAP_ANONYMOUS
#define MY_MAP_ANONYMOUS MAP_ANONYMOUS
#else
#error "Neither MAP_ANON nor MAP_ANONYMOUS"
#endif
#endif

#endif
