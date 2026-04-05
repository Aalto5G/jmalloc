#ifndef _JMALLOCLIKELY_H_
#define _JMALLOCLIKELY_H_

#ifdef __has_builtin
  #if __has_builtin (__builtin_expect)
    #define jmalloc_likely(x)       __builtin_expect(!!(x),1)
    #define jmalloc_unlikely(x)     __builtin_expect(!!(x),0)
  #else
    #define likely(x)       (!!(x))
    #define unlikely(x)     (!!(x))
  #endif
#else
  #ifdef __clang__ // LLVM 3.0 is the first to use this, no-op on LLVM 1.0
    #define jmalloc_likely(x)       __builtin_expect(!!(x),1)
    #define jmalloc_unlikely(x)     __builtin_expect(!!(x),0)
  #else
    #ifdef __GNUC__
      #if __GNUC__ >= 3
        #define jmalloc_likely(x)       __builtin_expect(!!(x),1)
        #define jmalloc_unlikely(x)     __builtin_expect(!!(x),0)
      #else
        #define jmalloc_likely(x)       (!!(x))
        #define jmalloc_unlikely(x)     (!!(x))
      #endif
    #else
      #define jmalloc_likely(x)       (!!(x))
      #define jmalloc_unlikely(x)     (!!(x))
    #endif
  #endif
#endif

#endif
