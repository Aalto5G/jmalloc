#ifndef _JMALLOCCORE_H_
#define _JMALLOCCORE_H_

#include <stddef.h>

void *jmalloc(size_t sz);

void jmfree(void *ptr, size_t sz);

#endif
