#ifndef C166_CLOSURE_HELPER
#error C166_CLOSURE_HELPER must select one runtime helper
#endif

#define C166_CLOSURE_ROOT                                                      \
  __attribute__((noinline, used, section(".runtime_closure_root")))

#if C166_CLOSURE_HELPER == 0
extern double __adddf3(double, double);
C166_CLOSURE_ROOT double runtime_closure_root(double left, double right) {
  return __adddf3(left, right);
}
#elif C166_CLOSURE_HELPER == 1
extern double __subdf3(double, double);
C166_CLOSURE_ROOT double runtime_closure_root(double left, double right) {
  return __subdf3(left, right);
}
#elif C166_CLOSURE_HELPER == 2
extern double __muldf3(double, double);
C166_CLOSURE_ROOT double runtime_closure_root(double left, double right) {
  return __muldf3(left, right);
}
#elif C166_CLOSURE_HELPER == 3
extern double __divdf3(double, double);
C166_CLOSURE_ROOT double runtime_closure_root(double left, double right) {
  return __divdf3(left, right);
}
#elif C166_CLOSURE_HELPER == 4
extern long __ledf2(double, double);
C166_CLOSURE_ROOT long runtime_closure_root(double left, double right) {
  return __ledf2(left, right);
}
#elif C166_CLOSURE_HELPER == 5
extern long __gedf2(double, double);
C166_CLOSURE_ROOT long runtime_closure_root(double left, double right) {
  return __gedf2(left, right);
}
#elif C166_CLOSURE_HELPER == 6
extern long __unorddf2(double, double);
C166_CLOSURE_ROOT long runtime_closure_root(double left, double right) {
  return __unorddf2(left, right);
}
#elif C166_CLOSURE_HELPER == 7
extern float __floatsisf(long);
C166_CLOSURE_ROOT float runtime_closure_root(long value) {
  return __floatsisf(value);
}
#elif C166_CLOSURE_HELPER == 8
extern float __floatunsisf(unsigned long);
C166_CLOSURE_ROOT float runtime_closure_root(unsigned long value) {
  return __floatunsisf(value);
}
#elif C166_CLOSURE_HELPER == 9
extern float __addsf3(float, float);
C166_CLOSURE_ROOT float runtime_closure_root(float left, float right) {
  return __addsf3(left, right);
}
#elif C166_CLOSURE_HELPER == 10
extern float __subsf3(float, float);
C166_CLOSURE_ROOT float runtime_closure_root(float left, float right) {
  return __subsf3(left, right);
}
#elif C166_CLOSURE_HELPER == 11
extern float __mulsf3(float, float);
C166_CLOSURE_ROOT float runtime_closure_root(float left, float right) {
  return __mulsf3(left, right);
}
#elif C166_CLOSURE_HELPER == 12
extern float __divsf3(float, float);
C166_CLOSURE_ROOT float runtime_closure_root(float left, float right) {
  return __divsf3(left, right);
}
#elif C166_CLOSURE_HELPER == 13
C166_CLOSURE_ROOT long runtime_closure_root(double left, double right) {
  return left < right;
}
#elif C166_CLOSURE_HELPER == 14
C166_CLOSURE_ROOT float runtime_closure_root(double value) {
  return (float)value;
}
#elif C166_CLOSURE_HELPER == 15
C166_CLOSURE_ROOT double runtime_closure_root(float value) {
  return (double)value;
}
#elif C166_CLOSURE_HELPER == 16
C166_CLOSURE_ROOT long runtime_closure_root(double value) {
  return (long)value;
}
#elif C166_CLOSURE_HELPER == 17
C166_CLOSURE_ROOT double runtime_closure_root(long value) {
  return (double)value;
}
#elif C166_CLOSURE_HELPER == 18
C166_CLOSURE_ROOT double runtime_closure_root(unsigned long value) {
  return (double)value;
}
#elif C166_CLOSURE_HELPER == 19
C166_CLOSURE_ROOT unsigned long runtime_closure_root(double value) {
  return (unsigned long)value;
}
#elif C166_CLOSURE_HELPER == 20
C166_CLOSURE_ROOT unsigned long runtime_closure_root(float value) {
  return (unsigned long)value;
}
#elif C166_CLOSURE_HELPER == 21
C166_CLOSURE_ROOT long runtime_closure_root(long left, long right) {
  return left / right;
}
#elif C166_CLOSURE_HELPER == 22
C166_CLOSURE_ROOT long runtime_closure_root(long left, long right) {
  return left % right;
}
#elif C166_CLOSURE_HELPER == 23
C166_CLOSURE_ROOT unsigned long runtime_closure_root(unsigned long left,
                                                     unsigned long right) {
  return left / right;
}
#elif C166_CLOSURE_HELPER == 24
extern long __lesf2(float, float);
C166_CLOSURE_ROOT long runtime_closure_root(float left, float right) {
  return __lesf2(left, right);
}
#elif C166_CLOSURE_HELPER == 25
extern long __gesf2(float, float);
C166_CLOSURE_ROOT long runtime_closure_root(float left, float right) {
  return __gesf2(left, right);
}
#elif C166_CLOSURE_HELPER == 26
extern double __floatsidf(long);
C166_CLOSURE_ROOT double runtime_closure_root(long value) {
  return __floatsidf(value);
}
#elif C166_CLOSURE_HELPER == 27
extern double __floatunsidf(unsigned long);
C166_CLOSURE_ROOT double runtime_closure_root(unsigned long value) {
  return __floatunsidf(value);
}
#elif C166_CLOSURE_HELPER == 28
C166_CLOSURE_ROOT long runtime_closure_root(float value) { return (long)value; }
#elif C166_CLOSURE_HELPER == 29
C166_CLOSURE_ROOT unsigned long runtime_closure_root(unsigned long left,
                                                     unsigned long right) {
  return left % right;
}
#elif C166_CLOSURE_HELPER == 30
C166_CLOSURE_ROOT unsigned int
runtime_closure_root(_Atomic(unsigned int) *object, unsigned int value) {
  return __c11_atomic_exchange(object, value, __ATOMIC_SEQ_CST);
}
#elif C166_CLOSURE_HELPER == 31
extern void __c166_atomic_rmw(unsigned int, volatile void *, const void *,
                              void *, unsigned int);
C166_CLOSURE_ROOT unsigned int
runtime_closure_root(_Atomic(unsigned int) *object, unsigned int value) {
  unsigned int result;
  __c166_atomic_rmw(sizeof(value), object, &value, &result, 0);
  return result;
}
#elif C166_CLOSURE_HELPER == 32
extern void *memmove(void *, const void *, __SIZE_TYPE__);
C166_CLOSURE_ROOT void *runtime_closure_root(void *destination,
                                             const void *source,
                                             __SIZE_TYPE__ count) {
  return memmove(destination, source, count);
}
#elif C166_CLOSURE_HELPER == 33
extern long __ashlsi3(long, unsigned int);
C166_CLOSURE_ROOT long runtime_closure_root(long value, unsigned int count) {
  return __ashlsi3(value, count);
}
#elif C166_CLOSURE_HELPER == 34
extern long __ashrsi3(long, unsigned int);
C166_CLOSURE_ROOT long runtime_closure_root(long value, unsigned int count) {
  return __ashrsi3(value, count);
}
#elif C166_CLOSURE_HELPER == 35
extern unsigned long __lshrsi3(unsigned long, unsigned int);
C166_CLOSURE_ROOT unsigned long runtime_closure_root(unsigned long value,
                                                     unsigned int count) {
  return __lshrsi3(value, count);
}
#elif C166_CLOSURE_HELPER == 36
extern double __adddf3(double, double);
extern double __subdf3(double, double);
extern double __muldf3(double, double);
extern double __divdf3(double, double);
extern long __ledf2(double, double);
extern float __addsf3(float, float);
extern float __subsf3(float, float);
extern float __mulsf3(float, float);
extern float __divsf3(float, float);
extern double __floatsidf(long);
extern double __floatunsidf(unsigned long);

typedef union {
  double value;
  unsigned int word[4];
} c166_closure_double;

typedef union {
  float value;
  unsigned int word[2];
} c166_closure_float;

C166_CLOSURE_ROOT unsigned int
runtime_closure_root(double left, double right, float left_float,
                     float right_float, long signed_value,
                     unsigned long unsigned_value) {
  c166_closure_double double_result;
  c166_closure_float float_result;
  unsigned int result = 0;

#define C166_ACCUMULATE_DOUBLE(expression)                                     \
  do {                                                                         \
    double_result.value = (expression);                                        \
    result ^= double_result.word[0] ^ double_result.word[1] ^                  \
              double_result.word[2] ^ double_result.word[3];                   \
  } while (0)
#define C166_ACCUMULATE_FLOAT(expression)                                      \
  do {                                                                         \
    float_result.value = (expression);                                         \
    result ^= float_result.word[0] ^ float_result.word[1];                     \
  } while (0)

  C166_ACCUMULATE_DOUBLE(__adddf3(left, right));
  C166_ACCUMULATE_DOUBLE(__subdf3(left, right));
  C166_ACCUMULATE_DOUBLE(__muldf3(left, right));
  C166_ACCUMULATE_DOUBLE(__divdf3(left, right));
  result ^= (unsigned int)__ledf2(left, right);
  C166_ACCUMULATE_FLOAT(__addsf3(left_float, right_float));
  C166_ACCUMULATE_FLOAT(__subsf3(left_float, right_float));
  C166_ACCUMULATE_FLOAT(__mulsf3(left_float, right_float));
  C166_ACCUMULATE_FLOAT(__divsf3(left_float, right_float));
  C166_ACCUMULATE_DOUBLE(__floatsidf(signed_value));
  C166_ACCUMULATE_DOUBLE(__floatunsidf(unsigned_value));
  return result;
}
#else
#error unsupported C166_CLOSURE_HELPER
#endif
