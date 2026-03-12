#ifndef _LIKELY_H_
#define _LIKELY_H_

#ifdef __has_builtin
  #if __has_builtin (__builtin_expect)
    #define likely(x)       __builtin_expect(!!(x),1)
    #define unlikely(x)     __builtin_expect(!!(x),0)
  #endif
#else
  #ifdef __clang__ // LLVM 3.0 is the first to use this, no-op on LLVM 1.0
    #define likely(x)       __builtin_expect(!!(x),1)
    #define unlikely(x)     __builtin_expect(!!(x),0)
  #else
    #ifdef __GNUC__
      #if __GNUC__ >= 3
        #define likely(x)       __builtin_expect(!!(x),1)
        #define unlikely(x)     __builtin_expect(!!(x),0)
      #else
        #define likely(x)       (!!(x))
        #define unlikely(x)     (!!(x))
      #endif
    #else
      #define likely(x)       (!!(x))
      #define unlikely(x)     (!!(x))
    #endif
  #endif
#endif

#endif
