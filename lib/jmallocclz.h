#ifndef _JMALLOCCLZ_H_
#define _JMALLOCCLZ_H_

// LLVM 2.6 is first with clang, it supports __has_builtin
// GCC 3.4 is the first GCC with __has_clz
#undef JMALLOC_HAS_CLZ

#ifdef __has_builtin
  #if __has_builtin (__builtin_clz)
    #define JMALLOC_HAS_CLZ
  #endif
#else
  #ifdef __GNUC__
    #if __GNUC__ == 3 && __GNUC_MINOR__ >= 4
      #define JMALLOC_HAS_CLZ
    #else
      #if __GNUC__ > 3
        #define JMALLOC_HAS_CLZ
      #endif
    #endif
  #endif
#endif

#endif
