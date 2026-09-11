#ifndef C166_TEST_COMPAT_H
#define C166_TEST_COMPAT_H

#if defined(C166_TEST_LLVM)
#define C166_TEST_NOINLINE __attribute__((noinline))
#else
#define C166_TEST_NOINLINE
#endif

typedef unsigned int c166_test_size_t;

#endif
