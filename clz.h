#ifndef _CLZ_H_
#define _CLZ_H_

// LLVM 2.6 is first with clang, it supports __has_builtin
// GCC 3.4 is the first GCC with __has_clz
#undef HAS_CLZ

#ifdef __has_builtin
  #if __has_builtin (__builtin_clz)
    #define HAS_CLZ
  #endif
#else
  #ifdef __GNUC__
    #if __GNUC__ == 3 && __GNUC_MINOR__ >= 4
      #define HAS_CLZ
    #else
      #if __GNUC__ > 3
        #define HAS_CLZ
      #endif
    #endif
  #endif
#endif

#endif
