#ifndef ABI_STACKPARM_TYPES_H
#define ABI_STACKPARM_TYPES_H

#include "c166_test.h"

typedef unsigned char abi_u8;
typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

struct abi_stackparm_pair {
  abi_u16 lo;
  abi_u16 hi;
};

typedef union {
  float value;
  abi_u16 words[2];
} abi_float_words;

typedef union {
  double value;
  abi_u16 words[4];
} abi_double_words;

typedef abi_u16 C166_STACKPARM abi_stackparm_function(
    abi_u8 a, abi_u16 b, abi_u32 c, const abi_u16 C166_FAR *p,
    struct abi_stackparm_pair pair, abi_u16 d);
typedef float C166_STACKPARM abi_stackparm_float_function(float value,
                                                          abi_u16 tail);
typedef double C166_STACKPARM abi_stackparm_double_function(double value,
                                                            abi_u16 tail);

abi_u16 C166_STACKPARM stackparm_callee(
    abi_u8, abi_u16, abi_u32, const abi_u16 C166_FAR *,
    struct abi_stackparm_pair, abi_u16);
float C166_STACKPARM stackparm_float_callee(float, abi_u16);
double C166_STACKPARM stackparm_double_callee(double, abi_u16);

abi_u16 stackparm_direct(abi_u8, abi_u16, abi_u32,
                         const abi_u16 C166_FAR *,
                         struct abi_stackparm_pair, abi_u16);
abi_u16 stackparm_indirect(abi_stackparm_function *, abi_u8, abi_u16,
                           abi_u32, const abi_u16 C166_FAR *,
                           struct abi_stackparm_pair, abi_u16);
float stackparm_float_direct(float, abi_u16);
float stackparm_float_indirect(abi_stackparm_float_function *, float,
                               abi_u16);
double stackparm_double_direct(double, abi_u16);
double stackparm_double_indirect(abi_stackparm_double_function *, double,
                                 abi_u16);
void stackparm_routes(
    abi_stackparm_float_function *, abi_stackparm_double_function *,
    const abi_float_words C166_FAR *, const abi_double_words C166_FAR *,
    abi_float_words C166_FAR *, abi_double_words C166_FAR *, abi_u16);

#endif
