#ifndef C166_TEST_H
#define C166_TEST_H

void tap_plan(unsigned int count);
void tap_ok(int condition, const char *name);
void tap_is_u32(unsigned long actual, unsigned long expected, const char *name);

typedef unsigned int c166_test_size_t;

#if defined(C166_TEST_LLVM)
#define C166_NOINLINE __attribute__((noinline))
#define C166_USED __attribute__((used))
#define C166_SECTION(name) __attribute__((section(name)))
#define C166_NEAR __attribute__((c166_near))
#define C166_XNEAR __attribute__((c166_xnear))
#define C166_FAR __attribute__((c166_far))
#define C166_HUGE __attribute__((c166_huge))
#define C166_SHUGE __attribute__((c166_shuge))
#define C166_STACKPARM __attribute__((c166_stackparm))
#define C166_BOOL _Bool
#define C166_TO_BOOL(value) (value)
#define C166_VA_COPY(destination, source) va_copy(destination, source)
#else
/* TASKING only expands functions explicitly declared with _inline. */
#define C166_NOINLINE
#define C166_USED
#define C166_SECTION(name)
#define C166_NEAR _near
#define C166_XNEAR _xnear
#define C166_FAR _far
#define C166_HUGE _huge
#define C166_SHUGE _shuge
#define C166_STACKPARM _stackparm
#define C166_BOOL unsigned char
#define C166_TO_BOOL(value) ((value) != 0)
#define C166_VA_COPY(destination, source) ((destination) = (source))
#endif

#if defined(C166_TEST_LLVM)
#define C166_DPP0_VALUE (*(volatile __sfr unsigned int *)0xfe00U)
#define C166_DPP1_VALUE (*(volatile __sfr unsigned int *)0xfe02U)
#define C166_DPP2_VALUE (*(volatile __sfr unsigned int *)0xfe04U)
#define C166_DPP3_VALUE (*(volatile __sfr unsigned int *)0xfe06U)
#else
#define C166_DPP0_VALUE DPP0
#define C166_DPP1_VALUE DPP1
#define C166_DPP2_VALUE DPP2
#define C166_DPP3_VALUE DPP3
#endif

#define C166_TEST_NOINLINE C166_NOINLINE

#endif
